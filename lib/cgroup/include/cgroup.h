#ifndef __CGROUP_H__
#define __CGROUP_H__

/**
 * @file cgroup.h
 * @brief Layer 2: cgroup/socket hooks (kernel side only)
 *
 * * Attached once to a parent cgroup v2 (e.g. the containers' slice); applies to every child.
 * * Workload identity is bpf_get_current_cgroup_id(), the key into the policy maps,
 * so a new container needs a map entry, never a new attach.
 *
 * * Context: struct bpf_sock_addr.
 * - ctx->user_ip4 and ctx->user_port are in network order (bpf_ntohs the port).
 * - Return CG_ALLOW or CG_DENY; deny makes connect(2)/bind(2) fail with EPERM.
 */

#include "maps.h"

#define CG_DENY  0
#define CG_ALLOW 1

#endif // !__CGROUP_H__
