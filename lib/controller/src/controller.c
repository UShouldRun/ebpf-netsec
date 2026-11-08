#include "controller_private.h"

// ================# PUBLIC #================

error8 controller_init(Controller* ctl, Firewall* fw) {
  if (ctl == NULL || fw == NULL)
    return FW_ERR_NULL;

  /* TODO: prepare the controller for this firewall */
  return FW_ERR_UNSUPPORTED;
}

error8 controller_load_rules(Controller* ctl, const str8 path) {
  if (ctl == NULL || path.str == NULL)
    return FW_ERR_NULL;

  /* TODO: turn the rule file into map entries */
  return FW_ERR_UNSUPPORTED;
}

error8 controller_dump_counters(Controller* ctl, FILE* file) {
  if (ctl == NULL || file == NULL)
    return FW_ERR_NULL;

  /* TODO: export the per-rule counters */
  return FW_ERR_UNSUPPORTED;
}

error8 controller_poll(Controller* ctl, const i32 timeout_ms) {
  if (ctl == NULL)
    return FW_ERR_NULL;

  /* TODO: drain pending drop events */
  (void)timeout_ms;
  return FW_ERR_UNSUPPORTED;
}

void controller_free(Controller* ctl) {
  if (ctl == NULL)
    return;

  /* TODO: release what controller_init acquired */
}

// ================# PRIVATE #================

error8 __controller_parse_line(Controller* ctl, const str8 line, const u32 rule_id) {
  /* TODO: recognise the rule kind and hand it to its handler */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_rule_cidr(Controller* ctl, const vector(str8)* args, const u32 rule_id) {
  /* TODO: add a CIDR block rule */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_rule_port(Controller* ctl, const vector(str8)* args, const u32 rule_id) {
  /* TODO: add a port/protocol deny rule */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_rule_rate(Controller* ctl, const vector(str8)* args, const u32 rule_id) {
  /* TODO: configure the rate limit */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_rule_connect(Controller* ctl, const vector(str8)* args, const u32 rule_id) {
  /* TODO: allow a workload to connect */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_rule_bind(Controller* ctl, const vector(str8)* args, const u32 rule_id) {
  /* TODO: reserve a host service port */
  return FW_ERR_UNSUPPORTED;
}

error8 __controller_cgroup_id(const str8 path, u64* cgroup_id) {
  if (path.str == NULL || cgroup_id == NULL)
    return FW_ERR_NULL;

  /* TODO: resolve a cgroup path to the id the kernel reports */
  return FW_ERR_UNSUPPORTED;
}

i32 __controller_on_event(void* ctx, void* data, u64 s_data) {
  if (s_data < sizeof(FwEvent))
    return 0; /* ignore, never stop polling */

  /* TODO: log one drop event */
  (void)ctx; (void)data;
  return 0;
}
