#ifndef __CONTROLLER_PRIVATE_H__
#define __CONTROLLER_PRIVATE_H__

#include "controller.h"

#include <string.h>
#include <sys/stat.h>

#include <bpf/libbpf.h>
#include <bpf/bpf.h>

/**
 * @file controller_private.h
 * @brief Rule parsing and ring buffer handling
 *
 * - cgroup id: the inode number of the cgroup v2 directory (stat(path).st_ino),
 * which is what bpf_get_current_cgroup_id() returns in the kernel.
 */

#define CONTROLLER_ARENA_MIN KB(64) /* floor; load_rules grows it to fit the file */

enum rule_kind {
  RULE_CIDR = 0,
  RULE_PORT,
  RULE_RATE,
  RULE_CONNECT,
  RULE_BIND
};

typedef enum rule_kind RuleKind;

error8 __controller_parse_line   (Controller* ctl, const str8 line, const u32 rule_id);

error8 __controller_rule_cidr    (Controller* ctl, const vector(str8)* args, const u32 rule_id);
error8 __controller_rule_port    (Controller* ctl, const vector(str8)* args, const u32 rule_id);
error8 __controller_rule_rate    (Controller* ctl, const vector(str8)* args, const u32 rule_id);
error8 __controller_rule_connect (Controller* ctl, const vector(str8)* args, const u32 rule_id);
error8 __controller_rule_bind    (Controller* ctl, const vector(str8)* args, const u32 rule_id);

error8 __controller_cgroup_id    (const str8 path, u64* cgroup_id);

i32    __controller_on_event     (void* ctx, void* data, u64 s_data); /* ring_buffer_sample_fn (u64 == size_t on LP64) */

#endif // !__CONTROLLER_PRIVATE_H__
