#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#include "maps.bpf.h"
#include "xdp.h"

// ================# PRIVATE #================

static __always_inline XdpParseResult __xdp_parse(struct xdp_md* ctx, XdpPacket* pkt) {
  void
    *data     = (void*)(u64)ctx->data,
    *data_end = (void*)(u64)ctx->data_end;

  /* TODO: extract the fields the rules need, safely */
  (void)data; (void)data_end; (void)pkt;
  return XDP_PARSE_SHORT;
}

static __always_inline bool __xdp_match_cidr(const XdpPacket* pkt, u32* rule_id) {
  /* TODO: is the source in a blocked prefix? */
  return false;
}

static __always_inline bool __xdp_match_port(const XdpPacket* pkt, u32* rule_id) {
  /* TODO: is the destination port/protocol denied? */
  return false;
}

static __always_inline bool __xdp_over_rate(const XdpPacket* pkt, u32* rule_id) {
  /* TODO: is the source over its rate? */
  return false;
}

// ================# PROGRAM #================

SEC("xdp")
i32 fw_xdp(struct xdp_md* ctx) {
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

  fw_count(rule_id, 0); /* TODO: packet size */
  /* TODO: log the drop */

  return XDP_DROP;
}
