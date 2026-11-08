#include <bpf/libbpf.h>

#include "maps.h"

u32 maps_percpu_size(const u32 s_value) {
  /* TODO: libbpf_num_possible_cpus() * ((s_value + 7) & ~7) */
  return 0;
}

error8 maps_cidr_parse(const str8 str, FwCidrKey* key) {
  if (str.str == NULL || key == NULL)
    return FW_ERR_NULL;

  /* TODO: split on '/', copy the address into char[INET_ADDRSTRLEN] (str8 is not
   *       NUL-terminated), inet_pton(AF_INET), prefixlen in [0, 32],
   *       mask the host bits so 10.1.2.3/8 and 10.0.0.0/8 are the same key */
  return FW_ERR_UNSUPPORTED;
}

error8 maps_proto_parse(const str8 str, u8* proto) {
  if (str.str == NULL || proto == NULL)
    return FW_ERR_NULL;

  /* TODO: string8_compare(str, STR8_LIT("tcp")) == 0 → IPPROTO_TCP, "udp" → IPPROTO_UDP */
  return FW_ERR_UNSUPPORTED;
}

error8 maps_counter_sum(const i32 fd, const u32 rule_id, FwCounter* out) {
  if (out == NULL)
    return FW_ERR_NULL;
  if (fd < 0)
    return FW_ERR_INVAL;

  /* TODO: bpf_map_lookup_elem(fd, &rule_id, buf) with a buffer of
   *       maps_percpu_size(sizeof(FwCounter)) bytes, then sum every CPU */
  return FW_ERR_UNSUPPORTED;
}
