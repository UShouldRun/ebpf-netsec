#include "controller_private.h"

// ================# PUBLIC #================

error8 controller_init(Controller* ctl, Firewall* fw) {
  if (ctl == NULL || fw == NULL)
    return FW_ERR_NULL;

  /* TODO: zero ctl, ctl->fw = fw, running = true
   *       ctl->arena = arena_create(CONTROLLER_ARENA_MIN, 1) → FW_ERR_NOMEM
   *       ctl->events = ring_buffer__new(firewall_map_fd(fw, FW_MAP_EVENTS),
   *                                      __controller_on_event, ctl, NULL) → FW_ERR_RB_NEW */
  return FW_ERR_UNSUPPORTED;
}

error8 controller_load_rules(Controller* ctl, const char* path) {
  if (ctl == NULL || path == NULL)
    return FW_ERR_NULL;

  /* TODO: stat(path) → FW_ERR_RULE_OPEN; if the arena is smaller than ~4x the file
   *       (file + line vector + tokens), arena_destroy and arena_create a bigger one
   *       file  = string8_read_file_arena(path, ctl->arena)
   *       lines = string8_split_arena(file, STR8_LIT("\n"), ctl->arena)
   *       for each line: string8_strip_arena, skip empty / starts_with "#",
   *       __controller_parse_line(ctl, line, ctl->s_rules++), stop at FW_MAX_RULES
   *       report the line number on error
   *       arena_reset(ctl->arena) on every exit path */
  return FW_ERR_UNSUPPORTED;
}

error8 controller_dump_counters(Controller* ctl, FILE* file) {
  if (ctl == NULL || file == NULL)
    return FW_ERR_NULL;

  /* TODO: for rule_id in [0, s_rules): maps_counter_sum(counters_fd, rule_id, &c),
   *       print "rule_id packets bytes" (CSV is easiest for the benchmark scripts) */
  return FW_ERR_UNSUPPORTED;
}

error8 controller_poll(Controller* ctl, const i32 timeout_ms) {
  if (ctl == NULL || ctl->events == NULL)
    return FW_ERR_NULL;

  /* TODO: ring_buffer__poll(ctl->events, timeout_ms); -EINTR is not an error */
  (void)timeout_ms;
  return FW_ERR_UNSUPPORTED;
}

void controller_free(Controller* ctl) {
  if (ctl == NULL)
    return;

  /* TODO: ring_buffer__free(ctl->events), arena_destroy(ctl->arena) */
}

// ================# PRIVATE #================

error8 __controller_parse_line(Controller* ctl, const str8 line, const u32 rule_id) {
  /* TODO: args = string8_split_arena(line, STR8_LIT(" "), ctl->arena) (drop empty tokens),
   *       dispatch on string8_compare(args[0], STR8_LIT("cidr")) ... to __controller_rule_*,
   *       passing the remaining tokens */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_rule_cidr(Controller* ctl, const vector(str8)* args, const u32 rule_id) {
  /* TODO: maps_cidr_parse(args[0], &key); bpf_map_update_elem(cidr_fd, &key, &(FwRule){rule_id}, BPF_ANY) */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_rule_port(Controller* ctl, const vector(str8)* args, const u32 rule_id) {
  /* TODO: maps_proto_parse(args[0]), string8_is_num(args[1]) and in [1, 65535], memset key 0, update fw_ports */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_rule_rate(Controller* ctl, const vector(str8)* args, const u32 rule_id) {
  /* TODO: FwRateCfg { rate, burst } into fw_rate_cfg[0]
   *       note: the rate counter shares rule_id with the config line */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_rule_connect(Controller* ctl, const vector(str8)* args, const u32 rule_id) {
  /* TODO: __controller_cgroup_id(args[0]), proto, port → FwCgroupKey (pads zeroed), update fw_cg_connect */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_rule_bind(Controller* ctl, const vector(str8)* args, const u32 rule_id) {
  /* TODO: proto, port → FwPortKey, update fw_cg_bind */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_cgroup_id(const str8 path, u64* cgroup_id) {
  if (path.str == NULL || cgroup_id == NULL)
    return FW_ERR_NULL;

  /* TODO: str8 is not NUL-terminated: copy into char[PATH_MAX] first
   *       struct stat st; stat(buf, &st) → FW_ERR_NOT_FOUND; *cgroup_id = st.st_ino */
  return FW_ERR_UNSUPPORTED;
}

i32 __controller_on_event(void* ctx, void* data, size_t s_data) {
  if (s_data < sizeof(FwEvent))
    return 0; /* ignore, never stop polling */

  /* TODO: print the FwEvent (inet_ntop addrs, reason name) as one log line */
  (void)ctx; (void)data;
  return 0;
}
