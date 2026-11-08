#include <bpf/libbpf.h>

#include "maps.h"

u32 maps_percpu_size(const u32 s_value) {
  /* TODO: size of a user-space buffer for one per-CPU value */
  return 0;
}

error8 maps_cidr_parse(const str8 str, FwCidrKey* key) {
  if (str.str == NULL || key == NULL)
    return FW_ERR_NULL;

  /* TODO: parse an IPv4 CIDR into a trie key */
  return FW_ERR_UNSUPPORTED;
}

error8 maps_proto_parse(const str8 str, u8* proto) {
  if (str.str == NULL || proto == NULL)
    return FW_ERR_NULL;

  /* TODO: parse a protocol name */
  return FW_ERR_UNSUPPORTED;
}

error8 maps_counter_sum(const i32 fd, const u32 rule_id, FwCounter* out) {
  if (out == NULL)
    return FW_ERR_NULL;
  if (fd < 0)
    return FW_ERR_INVAL;

  /* TODO: total a rule's counters across CPUs */
  return FW_ERR_UNSUPPORTED;
}
