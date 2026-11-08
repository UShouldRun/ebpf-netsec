#include "error.h"

const char* error_str(const error8 err) {
  switch (err) {
    case FW_OK:                return "ok";

    case FW_ERR_NOMEM:         return "out of memory";
    case FW_ERR_INVAL:         return "invalid argument";
    case FW_ERR_NULL:          return "null argument";
    case FW_ERR_PERM:          return "permission denied (need CAP_BPF + CAP_NET_ADMIN)";
    case FW_ERR_NOT_FOUND:     return "not found";
    case FW_ERR_UNSUPPORTED:   return "unsupported";
    case FW_ERR_IO:            return "i/o error";
    case FW_ERR_AGAIN:         return "try again";

    case FW_ERR_BPF_OPEN:      return "cannot open bpf object";
    case FW_ERR_BPF_RESIZE:    return "cannot resize bpf map";
    case FW_ERR_BPF_LOAD:      return "bpf load failed (verifier)";

    case FW_ERR_IFINDEX:       return "unknown interface";
    case FW_ERR_XDP_ATTACH:    return "cannot attach xdp program";
    case FW_ERR_CGROUP_OPEN:   return "cannot open cgroup";
    case FW_ERR_CGROUP_ATTACH: return "cannot attach cgroup program";
    case FW_ERR_PIN:           return "cannot pin to bpffs";

    case FW_ERR_MAP_UPDATE:    return "map update failed";
    case FW_ERR_MAP_LOOKUP:    return "map lookup failed";
    case FW_ERR_MAP_FULL:      return "map full";

    case FW_ERR_RULE_OPEN:     return "cannot open rule file";
    case FW_ERR_RULE_SYNTAX:   return "rule syntax error";
    case FW_ERR_RULE_BAD_CIDR: return "bad cidr";
    case FW_ERR_RULE_BAD_PORT: return "bad port";
    case FW_ERR_RULE_BAD_PROTO:return "bad protocol";
    case FW_ERR_RULE_TOO_MANY: return "too many rules";

    case FW_ERR_RB_NEW:        return "cannot create ring buffer";
    case FW_ERR_RB_POLL:       return "ring buffer poll failed";

    default:                   return "unknown error";
  }
}
