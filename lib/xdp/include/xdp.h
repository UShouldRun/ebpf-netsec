#ifndef __XDP_H__
#define __XDP_H__

/**
 * @file xdp.h
 * @brief Layer 1: XDP ingress (kernel side only)
 *
 * * Runs in the driver receive path, before an sk_buff exists.
 * * Order of checks: CIDR blocklist → port/proto denylist → rate limit.
 * * First match drops; no match passes the packet to the stack.
 *
 * * Verifier rules that shape this code:
 * - Every read from ctx->data must be bounds-checked against ctx->data_end.
 * - IPv4 ihl is variable: check (ihl * 4) against data_end, not sizeof(struct iphdr).
 * - Map lookups may return NULL: check before every dereference.
 */

#include "maps.h"

#define ETH_P_IP   0x0800 /* macros are not in vmlinux.h */
#define ETH_P_IPV6 0x86DD

typedef struct xdp_packet XdpPacket;

struct xdp_packet {
  u32
    saddr,   // network order
    daddr,   // network order
    s_pkt;   // data_end - data, for the byte counters

  u16
    sport,   // host order, 0 if not TCP/UDP
    dport;

  u8 proto;
};

typedef enum xdp_parse_result {
  XDP_PARSE_OK = 0,
  XDP_PARSE_SHORT,       /* truncated header: let the stack decide */
  XDP_PARSE_NOT_IPV4,    /* TODO: IPv6 */
  XDP_PARSE_FRAGMENT     /* non-first fragment: no L4 header */
} XdpParseResult;

#endif // !__XDP_H__
