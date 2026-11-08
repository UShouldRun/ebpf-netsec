#include <stddef.h>

#include "error.h"

/* STR8_LIT is a compound literal, which is not a constant initializer: spell the str8 out. */
#define ERROR_STR(s) { .s_str = sizeof(s) - 1, .str = (u8*)(s) }

/* Indexed by error8: a table instead of a switch, one line per code. */
static const str8 error_strs[FW_ERR_MAX] = {
  [FW_OK]                 = ERROR_STR("ok"),

  [FW_ERR_NOMEM]          = ERROR_STR("out of memory"),
  [FW_ERR_INVAL]          = ERROR_STR("invalid argument"),
  [FW_ERR_NULL]           = ERROR_STR("null argument"),
  [FW_ERR_PERM]           = ERROR_STR("permission denied (need CAP_BPF + CAP_NET_ADMIN)"),
  [FW_ERR_NOT_FOUND]      = ERROR_STR("not found"),
  [FW_ERR_UNSUPPORTED]    = ERROR_STR("unsupported"),
  [FW_ERR_IO]             = ERROR_STR("i/o error"),
  [FW_ERR_AGAIN]          = ERROR_STR("try again"),

  [FW_ERR_BPF_OPEN]       = ERROR_STR("cannot open bpf object"),
  [FW_ERR_BPF_RESIZE]     = ERROR_STR("cannot resize bpf map"),
  [FW_ERR_BPF_LOAD]       = ERROR_STR("bpf load failed (verifier)"),

  [FW_ERR_IFINDEX]        = ERROR_STR("unknown interface"),
  [FW_ERR_XDP_ATTACH]     = ERROR_STR("cannot attach xdp program"),
  [FW_ERR_CGROUP_OPEN]    = ERROR_STR("cannot open cgroup"),
  [FW_ERR_CGROUP_ATTACH]  = ERROR_STR("cannot attach cgroup program"),
  [FW_ERR_PIN]            = ERROR_STR("cannot pin to bpffs"),

  [FW_ERR_MAP_UPDATE]     = ERROR_STR("map update failed"),
  [FW_ERR_MAP_LOOKUP]     = ERROR_STR("map lookup failed"),
  [FW_ERR_MAP_FULL]       = ERROR_STR("map full"),

  [FW_ERR_RULE_OPEN]      = ERROR_STR("cannot open rule file"),
  [FW_ERR_RULE_SYNTAX]    = ERROR_STR("rule syntax error"),
  [FW_ERR_RULE_BAD_CIDR]  = ERROR_STR("bad cidr"),
  [FW_ERR_RULE_BAD_PORT]  = ERROR_STR("bad port"),
  [FW_ERR_RULE_BAD_PROTO] = ERROR_STR("bad protocol"),
  [FW_ERR_RULE_TOO_MANY]  = ERROR_STR("too many rules"),

  [FW_ERR_RB_NEW]         = ERROR_STR("cannot create ring buffer"),
  [FW_ERR_RB_POLL]        = ERROR_STR("ring buffer poll failed"),
};

str8 error_str(const error8 err) {
  if (err >= FW_ERR_MAX)
    return STR8_LIT("unknown error");

  if (error_strs[err].str == NULL)
    return STR8_LIT("unknown error"); /* gap in the numbering */

  return error_strs[err];
}
