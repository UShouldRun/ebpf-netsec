#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#include "maps.bpf.h"
#include "cgroup.h"

// ================# PRIVATE #================

static __always_inline bool __cgroup_connect_allowed(const struct bpf_sock_addr* ctx, u32* rule_id) {
  /* TODO: FwCgroupKey key = {
   *         .cgroup_id = bpf_get_current_cgroup_id(),
   *         .port      = bpf_ntohs(ctx->user_port),
   *         .proto     = ctx->protocol,
   *       }; (pads zeroed)
   *       allowed iff bpf_map_lookup_elem(&fw_cg_connect, &key) != NULL
   * TODO: decide the default for cgroups with no entry at all (allow-list vs. open) */
  return true;
}

static __always_inline bool __cgroup_bind_denied(const struct bpf_sock_addr* ctx, u32* rule_id) {
  /* TODO: FwPortKey key = { .port = bpf_ntohs(ctx->user_port), .proto = ctx->protocol };
   *       denied iff bpf_map_lookup_elem(&fw_cg_bind, &key) != NULL */
  return false;
}

// ================# PROGRAMS #================

SEC("cgroup/connect4")
int fw_connect4(struct bpf_sock_addr* ctx) {
  u32 rule_id = 0;

  if (__cgroup_connect_allowed(ctx, &rule_id))
    return CG_ALLOW;

  fw_count(rule_id, 0);
  /* TODO: FwEvent with .cgroup_id, .daddr = ctx->user_ip4, .reason = FW_REASON_CONNECT; fw_log */
  return CG_DENY;
}

SEC("cgroup/bind4")
int fw_bind4(struct bpf_sock_addr* ctx) {
  u32 rule_id = 0;

  if (!__cgroup_bind_denied(ctx, &rule_id))
    return CG_ALLOW;

  fw_count(rule_id, 0);
  /* TODO: FwEvent with .reason = FW_REASON_BIND; fw_log */
  return CG_DENY;
}

/* TODO: cgroup/connect6, cgroup/bind6 (ctx->user_ip6[4]) */
