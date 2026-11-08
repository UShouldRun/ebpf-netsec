#include "firewall_private.h"

// ================# PUBLIC #================

error8 firewall_open(Firewall** fw, const FirewallOpts* opts) {
  if (fw == NULL || opts == NULL)
    return FW_ERR_NULL;

  /* TODO: create the firewall and load both layers into the kernel */
  return FW_ERR_UNSUPPORTED;
}

error8 firewall_attach(Firewall* fw) {
  if (fw == NULL)
    return FW_ERR_NULL;

  /* TODO: attach both layers */
  return FW_ERR_UNSUPPORTED;
}

error8 firewall_pin(Firewall* fw) {
  if (fw == NULL)
    return FW_ERR_NULL;
  /* TODO: make the firewall outlive the controller process */
  return FW_ERR_UNSUPPORTED;
}

void firewall_close(Firewall* fw) {
  if (fw == NULL)
    return;

  /* TODO: detach and release everything firewall_open/attach acquired */
}

i32 firewall_map_fd(const Firewall* fw, const FwMap map) {
  if (fw == NULL)
    return -1;

  /* TODO: expose the requested map to the controller */
  (void)map;
  return -1;
}

// ================# PRIVATE #================

error8 __firewall_resize(Firewall* fw) {
  /* TODO: apply the requested map sizes before load */
  return FW_ERR_UNSUPPORTED;
}

error8 __firewall_attach_xdp(Firewall* fw) {
  /* TODO: attach layer 1 to the interface */
  return FW_ERR_UNSUPPORTED;
}

error8 __firewall_attach_cg(Firewall* fw) {
  /* TODO: attach layer 2 to the cgroup */
  return FW_ERR_UNSUPPORTED;
}

error8 __firewall_pin_map(const Firewall* fw, struct bpf_map* map) {
  /* TODO: pin one map under the pin directory */
  return FW_ERR_UNSUPPORTED;
}

error8 __firewall_errno(const i32 err, const error8 fallback) {
  switch (-err) {
    case 0: {
      return FW_OK;
    }
    case EPERM:
    case EACCES: {
      return FW_ERR_PERM;
    }
    case ENOMEM: {
      return FW_ERR_NOMEM;
    }
    case ENOENT: {
      return FW_ERR_NOT_FOUND;
    }
    case EINVAL: {
      return FW_ERR_INVAL;
    }
    default: {
      return fallback;
    }
  }
}
