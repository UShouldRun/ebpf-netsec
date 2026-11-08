#ifndef __MAPS_BPF_H__
#define __MAPS_BPF_H__

/**
 * @file maps.bpf.h
 * @brief BTF map definitions and shared helpers (kernel side only)
 *
 * * Include after vmlinux.h and <bpf/bpf_helpers.h>.
 *
 * * Why __weak:
 * - Every .bpf.c includes this header, so each object carries its own copy of the maps.
 * - When `bpftool gen object` links them into fw.bpf.o, identical weak definitions merge
 * into one map, so XDP and the cgroup hooks share the same counters and ring buffer.
 *
 * * Each map below is a suggestion in a comment. Declare it as
 *   struct { __uint(type, ...); __uint(max_entries, ...); __type(key, ...); __type(value, ...); } name SEC(".maps") __weak;
 */

#include "maps.h"

// ================# LAYER 1 #================

/* fw_cidr: CIDR blocklist (layer 1)
 * Suggestion:
 *   type         BPF_MAP_TYPE_LPM_TRIE  // longest-prefix match on the source address
 *   map_flags    BPF_F_NO_PREALLOC      // required by LPM tries
 *   max_entries  FW_MAX_CIDR
 *   key          FwCidrKey
 *   value        FwRule                 // which rule matched
 */

/* fw_ports: port/protocol denylist (layer 1)
 * Suggestion:
 *   type         BPF_MAP_TYPE_HASH  // exact match
 *   max_entries  FW_MAX_PORTS
 *   key          FwPortKey
 *   value        FwRule
 */

/* fw_rate_cfg: rate limiter configuration (layer 1)
 * Suggestion:
 *   type         BPF_MAP_TYPE_ARRAY  // single slot, written by the controller
 *   max_entries  1
 *   key          u32
 *   value        FwRateCfg
 */

/* fw_rate: per-source rate state (layer 1)
 * Suggestion:
 *   type         BPF_MAP_TYPE_LRU_PERCPU_HASH  // LRU: spoofed floods can't fill it; per-CPU: no locks, limit is per CPU
 *   max_entries  FW_MAX_RATE
 *   key          u32                           // source address, network order
 *   value        FwRateState
 */

// ================# LAYER 2 #================

/* fw_cg_connect: connect allow-list (layer 2)
 * Suggestion:
 *   type         BPF_MAP_TYPE_HASH
 *   max_entries  FW_MAX_CGROUP
 *   key          FwCgroupKey        // workload + destination
 *   value        FwRule
 */

/* fw_cg_bind: host service ports no workload may bind (layer 2)
 * Suggestion:
 *   type         BPF_MAP_TYPE_HASH
 *   max_entries  FW_MAX_PORTS
 *   key          FwPortKey
 *   value        FwRule
 */

// ================# SHARED #================

/* fw_counters: per-rule counters (both layers)
 * Suggestion:
 *   type         BPF_MAP_TYPE_PERCPU_ARRAY  // indexed by rule id, no contention
 *   max_entries  FW_MAX_RULES
 *   key          u32                        // rule id
 *   value        FwCounter
 */

/* fw_events: drop log to user space (both layers)
 * Suggestion:
 *   type         BPF_MAP_TYPE_RINGBUF
 *   max_entries  FW_RINGBUF_SIZE       // bytes, power of 2
 */

/* Shared helpers for both layers. */

static __always_inline void fw_count(const u32 rule_id, const u32 s_bytes) {
  /* TODO: account one hit (and its bytes) to the rule */
}

static __always_inline void fw_log(const FwEvent* event) {
  /* TODO: hand the event to user space without ever blocking the data path */
}

#endif // !__MAPS_BPF_H__
