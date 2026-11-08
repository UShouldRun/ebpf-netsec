#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#include "maps.bpf.h"
#include "cgroup.h"

// ================# PRIVATE #================

static __always_inline bool __cgroup_connect_allowed(const struct bpf_sock_addr* ctx, u32* rule_id) {
  /* TODO: decide whether this workload may connect to this destination */
  return true;
}

static __always_inline bool __cgroup_bind_denied(const struct bpf_sock_addr* ctx, u32* rule_id) {
  /* TODO: decide whether this port is reserved for the host */
  return false;
}

// ================# PROGRAMS #================

SEC("cgroup/connect4")
i32 fw_connect4(struct bpf_sock_addr* ctx) {
  u32 rule_id = 0;

  if (__cgroup_connect_allowed(ctx, &rule_id))
    return CG_ALLOW;

  fw_count(rule_id, 0);
  /* TODO: log the denied connect */
  return CG_DENY;
}

SEC("cgroup/bind4")
i32 fw_bind4(struct bpf_sock_addr* ctx) {
  u32 rule_id = 0;

  if (!__cgroup_bind_denied(ctx, &rule_id))
    return CG_ALLOW;

  fw_count(rule_id, 0);
  /* TODO: log the denied bind */
  return CG_DENY;
}

/* TODO: IPv6 (connect6, bind6) */
