#ifndef __MAPS_H__
#define __MAPS_H__

/**
 * @file maps.h
 * @brief Shared map types: the contract between kernel and user space
 *
 * * Include order:
 * - Kernel side: vmlinux.h first, then this header.
 * - User side:   this header directly.
 *
 * * Rules:
 * - Fixed-width types and explicit padding only: both compilers must agree on the layout.
 * - Hash keys are compared byte-for-byte, so pads must be zeroed before every lookup/update.
 *
 * * The struct bodies are suggestions in comments: pick the layouts, then fill them in.
 */

#include "utils.h"

#define FW_MAX_RULES    1024         /* counter slots, indexed by rule id */
#define FW_MAX_CIDR     1024         /* resized at open: 1k / 10k / 100k */
#define FW_MAX_PORTS    1024
#define FW_MAX_RATE     65536        /* tracked sources (LRU-evicted) */
#define FW_MAX_CGROUP   1024
#define FW_RINGBUF_SIZE (256 * 1024) /* bytes, power of 2 */

enum fw_reason {
  FW_REASON_NONE = 0,
  FW_REASON_CIDR,                    /* layer 1 */
  FW_REASON_PORT,
  FW_REASON_RATE,
  FW_REASON_CONNECT,                 /* layer 2 */
  FW_REASON_BIND
};

typedef enum   fw_reason     FwReason;
typedef struct fw_cidr_key   FwCidrKey;
typedef struct fw_port_key   FwPortKey;
typedef struct fw_cgroup_key FwCgroupKey;
typedef struct fw_rule       FwRule;
typedef struct fw_rate_cfg   FwRateCfg;
typedef struct fw_rate_state FwRateState;
typedef struct fw_counter    FwCounter;
typedef struct fw_event      FwEvent;

struct fw_cidr_key {
  /* Suggestion (LPM trie key):
   *   u32
   *     prefixlen,  // prefix length in bits; an LPM trie key must start with it
   *     addr;       // IPv4 address, network order
   */
};

struct fw_port_key {
  /* Suggestion:
   *   u16 port;  // destination port, host order
   *   u8
   *     proto,   // IPPROTO_TCP / IPPROTO_UDP
   *     pad;     // explicit padding, zeroed: hash keys compare byte-for-byte
   */
};

struct fw_cgroup_key {
  /* Suggestion:
   *   u64 cgroup_id;  // workload identity: inode of its cgroup v2 directory
   *   u16 port;       // destination port, host order
   *   u8
   *     proto,        // IPPROTO_TCP / IPPROTO_UDP
   *     pad[5];       // explicit padding to 16 bytes, zeroed
   */
};

struct fw_rule {
  /* Suggestion (value of every rule map):
   *   u32 id;  // rule index: which counter slot a match increments
   */
};

struct fw_rate_cfg {
  /* Suggestion (rate limiter configuration):
   *   u64
   *     rate,   // tokens added per second
   *     burst;  // bucket capacity: the largest burst allowed
   */
};

struct fw_rate_state {
  /* Suggestion (per-source token bucket):
   *   u64
   *     tokens,   // tokens left in this source's bucket
   *     last_ns;  // time of the last refill (bpf_ktime_get_ns)
   */
};

struct fw_counter {
  /* Suggestion (per-rule, per-CPU):
   *   u64
   *     packets,  // packets that matched the rule
   *     bytes;    // bytes of those packets
   */
};

struct fw_event {
  /* Suggestion (one drop log record):
   *   u64
   *     ts_ns,      // when the drop happened
   *     cgroup_id;  // workload that was denied (0 on layer 1)
   *   u32
   *     rule_id,    // rule that matched
   *     saddr,      // source address, network order
   *     daddr;      // destination address, network order
   *   u16
   *     sport,      // source port, host order
   *     dport;      // destination port, host order
   *   u8
   *     proto,      // IPPROTO_*
   *     reason,     // FwReason: which check dropped it
   *     pad[6];     // explicit padding to 40 bytes
   */
};

#ifndef __bpf__

#include <string8/string8.h>

#include "error.h"

/* User-side helpers around the shared types. */

u32    maps_percpu_size  (const u32 s_value);                 /* nr_cpus * round_up(s_value, 8) */
error8 maps_cidr_parse   (const str8 str, FwCidrKey* key);    /* "a.b.c.d/len" */
error8 maps_proto_parse  (const str8 str, u8* proto);         /* "tcp" | "udp" */
error8 maps_counter_sum  (const i32 fd, const u32 rule_id, FwCounter* out);

#endif /* !__bpf__ */

#endif // !__MAPS_H__
