#ifndef __FW_ERROR_H__
#define __FW_ERROR_H__

#include "utils.h"

/**
 * @file error.h
 * @brief Firewall error codes: 0 is success, every error is positive and fits in a u8
 */

enum fw_error {
  FW_OK = 0,

  /* Generic */
  FW_ERR_NOMEM = 1,
  FW_ERR_INVAL,
  FW_ERR_NULL,
  FW_ERR_PERM,          /* missing CAP_BPF / CAP_NET_ADMIN */
  FW_ERR_NOT_FOUND,
  FW_ERR_UNSUPPORTED,
  FW_ERR_IO,
  FW_ERR_AGAIN,

  /* BPF object lifecycle */
  FW_ERR_BPF_OPEN = 20,
  FW_ERR_BPF_RESIZE,
  FW_ERR_BPF_LOAD,      /* the verifier rejected a program */

  /* Attach / pin */
  FW_ERR_IFINDEX = 30,
  FW_ERR_XDP_ATTACH,
  FW_ERR_CGROUP_OPEN,
  FW_ERR_CGROUP_ATTACH,
  FW_ERR_PIN,

  /* Maps */
  FW_ERR_MAP_UPDATE = 40,
  FW_ERR_MAP_LOOKUP,
  FW_ERR_MAP_FULL,

  /* Rule file */
  FW_ERR_RULE_OPEN = 50,
  FW_ERR_RULE_SYNTAX,
  FW_ERR_RULE_BAD_CIDR,
  FW_ERR_RULE_BAD_PORT,
  FW_ERR_RULE_BAD_PROTO,
  FW_ERR_RULE_TOO_MANY,

  /* Ring buffer */
  FW_ERR_RB_NEW = 60,
  FW_ERR_RB_POLL,

  FW_ERR_MAX /* keep last; must stay <= 255 */
};

typedef enum fw_error FwError;

_Static_assert(FW_ERR_MAX <= 255, "FwError must fit in a u8");

typedef u8 error8;

#ifndef __bpf__

#include <string8/string8.h>

str8 error_str (const error8 err);

#endif

#endif // !__FW_ERROR_H__
