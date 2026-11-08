#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#include "maps.bpf.h"
#include "xdp.h"

// ================# PRIVATE #================

static __always_inline XdpParseResult __xdp_parse(struct xdp_md* ctx, XdpPacket* pkt) {
  void
    *data     = (void*)(long)ctx->data,
    *data_end = (void*)(long)ctx->data_end;

  /* TODO: ethhdr (bounds) → h_proto == bpf_htons(ETH_P_IP)
   *       → iphdr (bounds, then ihl * 4 bounds) → skip if frag_off & 0x1FFF
   *       → tcphdr / udphdr (bounds) → fill pkt */
  (void)data; (void)data_end; (void)pkt;
  return XDP_PARSE_SHORT;
}

static __always_inline bool __xdp_match_cidr(const XdpPacket* pkt, u32* rule_id) {
  /* TODO: FwCidrKey key = { .prefixlen = 32, .addr = pkt->saddr };
   *       bpf_map_lookup_elem(&fw_cidr, &key) */
  return false;
}

static __always_inline bool __xdp_match_port(const XdpPacket* pkt, u32* rule_id) {
  /* TODO: FwPortKey key = { .port = pkt->dport, .proto = pkt->proto }; (pad zeroed)
   *       bpf_map_lookup_elem(&fw_ports, &key) */
  return false;
}

static __always_inline bool __xdp_over_rate(const XdpPacket* pkt, u32* rule_id) {
  /* TODO: token bucket on fw_rate[saddr] with fw_rate_cfg[0];
   *       now = bpf_ktime_get_ns(); refill, then take one token or drop */
  return false;
}

// ================# PROGRAM #================

SEC("xdp")
int fw_xdp(struct xdp_md* ctx) {
  XdpPacket pkt = {0};

  if (__xdp_parse(ctx, &pkt) != XDP_PARSE_OK)
    return XDP_PASS; /* not ours to judge */

  u32 rule_id = 0;
  FwReason reason = FW_REASON_NONE;

  if (__xdp_match_cidr(&pkt, &rule_id))
    reason = FW_REASON_CIDR;
  else if (__xdp_match_port(&pkt, &rule_id))
    reason = FW_REASON_PORT;
  else if (__xdp_over_rate(&pkt, &rule_id))
    reason = FW_REASON_RATE;

  if (reason == FW_REASON_NONE)
    return XDP_PASS;

  fw_count(rule_id, pkt.s_pkt);
  /* TODO: sample (e.g. bpf_get_prandom_u32() % N == 0) before logging */
  /* TODO: build FwEvent from pkt + reason, fw_log(&event) */
  return XDP_DROP;
}
