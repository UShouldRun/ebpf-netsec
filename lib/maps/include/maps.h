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
 */

#include "utils.h"

#define FW_MAX_RULES    1024         /* counter slots, indexed by rule id */
#define FW_MAX_CIDR     1024         /* resized at open: 1k / 10k / 100k */
#define FW_MAX_PORTS    1024
#define FW_MAX_RATE     65536        /* tracked sources (LRU-evicted) */
#define FW_MAX_CGROUP   1024
#define FW_RINGBUF_SIZE (256 * 1024) /* bytes, power of 2 */

typedef enum fw_reason {
  FW_REASON_NONE = 0,
  FW_REASON_CIDR,                    /* layer 1 */
  FW_REASON_PORT,
  FW_REASON_RATE,
  FW_REASON_CONNECT,                 /* layer 2 */
  FW_REASON_BIND
} FwReason;

typedef struct fw_cidr_key   FwCidrKey;
typedef struct fw_port_key   FwPortKey;
typedef struct fw_cgroup_key FwCgroupKey;
typedef struct fw_rule       FwRule;
typedef struct fw_rate_cfg   FwRateCfg;
typedef struct fw_rate_state FwRateState;
typedef struct fw_counter    FwCounter;
typedef struct fw_event      FwEvent;

struct fw_cidr_key {  /* LPM trie key: prefixlen must come first */
  u32 prefixlen;
  u32 addr;           /* network order */
};

struct fw_port_key {
  u16 port;           /* host order */
  u8  proto;          /* IPPROTO_TCP / IPPROTO_UDP */
  u8  pad;
};

struct fw_cgroup_key {
  u64 cgroup_id;      /* inode of the cgroup v2 directory */
  u16 port;           /* host order */
  u8  proto;
  u8  pad[5];
};

struct fw_rule {      /* value of every rule map */
  u32 id;             /* index into fw_counters */
};

struct fw_rate_cfg {  /* single entry, fw_rate_cfg[0] */
  u64 rate;           /* tokens per second */
  u64 burst;          /* bucket size */
};

struct fw_rate_state {
  u64 tokens;
  u64 last_ns;
};

struct fw_counter {
  u64 packets;
  u64 bytes;
};

struct fw_event {     /* ring buffer record, one per sampled drop */
  u64 ts_ns;
  u64 cgroup_id;      /* 0 on layer 1 */
  u32 rule_id;
  u32 saddr;          /* network order */
  u32 daddr;
  u16 sport;          /* host order */
  u16 dport;
  u8  proto;
  u8  reason;         /* FwReason */
  u8  pad[6];
};

_Static_assert(sizeof(FwCgroupKey) == 16, "FwCgroupKey layout");
_Static_assert(sizeof(FwEvent)     == 40, "FwEvent layout");

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
