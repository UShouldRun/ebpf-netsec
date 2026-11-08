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
 */

#include "maps.h"

// ================# LAYER 1 #================

struct {
  __uint(type, BPF_MAP_TYPE_LPM_TRIE);
  __uint(map_flags, BPF_F_NO_PREALLOC); /* mandatory for LPM */
  __uint(max_entries, FW_MAX_CIDR);
  __type(key, FwCidrKey);
  __type(value, FwRule);
} fw_cidr SEC(".maps") __weak;

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __uint(max_entries, FW_MAX_PORTS);
  __type(key, FwPortKey);
  __type(value, FwRule);
} fw_ports SEC(".maps") __weak;

struct {
  __uint(type, BPF_MAP_TYPE_ARRAY);
  __uint(max_entries, 1);
  __type(key, u32);
  __type(value, FwRateCfg);
} fw_rate_cfg SEC(".maps") __weak;

struct {
  __uint(type, BPF_MAP_TYPE_LRU_PERCPU_HASH); /* per-CPU: the limit is per CPU */
  __uint(max_entries, FW_MAX_RATE);
  __type(key, u32);                           /* saddr, network order */
  __type(value, FwRateState);
} fw_rate SEC(".maps") __weak;

// ================# LAYER 2 #================

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __uint(max_entries, FW_MAX_CGROUP);
  __type(key, FwCgroupKey);                   /* allow-list */
  __type(value, FwRule);
} fw_cg_connect SEC(".maps") __weak;

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __uint(max_entries, FW_MAX_PORTS);
  __type(key, FwPortKey);                     /* host service ports: deny */
  __type(value, FwRule);
} fw_cg_bind SEC(".maps") __weak;

// ================# SHARED #================

struct {
  __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
  __uint(max_entries, FW_MAX_RULES);
  __type(key, u32);                           /* rule id */
  __type(value, FwCounter);
} fw_counters SEC(".maps") __weak;

struct {
  __uint(type, BPF_MAP_TYPE_RINGBUF);
  __uint(max_entries, FW_RINGBUF_SIZE);
} fw_events SEC(".maps") __weak;

/* Shared helpers for both layers. */

static __always_inline void fw_count(const u32 rule_id, const u32 s_bytes) {
  /* TODO: lookup fw_counters[rule_id] (NULL check), packets++, bytes += s_bytes */
}

static __always_inline void fw_log(const FwEvent* event) {
  /* TODO: bpf_ringbuf_reserve(&fw_events, sizeof *event, 0) → copy → submit;
   *       drop silently when the ring is full (never block the data path) */
}

#endif // !__MAPS_BPF_H__
