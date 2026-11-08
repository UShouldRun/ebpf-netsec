#include "firewall_private.h"

// ================# PUBLIC #================

error8 firewall_open(Firewall** fw, const FirewallOpts* opts) {
  if (fw == NULL || opts == NULL)
    return FW_ERR_NULL;

  /* TODO: calloc Firewall, copy opts, cgroup_fd = -1
   *       skel = fw_bpf__open()          → FW_ERR_BPF_OPEN
   *       __firewall_resize(fw)           → FW_ERR_BPF_RESIZE
   *       fw_bpf__load(skel)              → FW_ERR_BPF_LOAD (print the verifier log)
   *       on any failure: firewall_close and *fw = NULL */
  return FW_ERR_UNSUPPORTED;
}

error8 firewall_attach(Firewall* fw) {
  if (fw == NULL)
    return FW_ERR_NULL;

  /* TODO: __firewall_attach_xdp, then __firewall_attach_cg */
  return FW_ERR_UNSUPPORTED;
}

error8 firewall_pin(Firewall* fw) {
  if (fw == NULL)
    return FW_ERR_NULL;
  if (fw->opts.pin_dir == NULL)
    return FW_OK;

  /* TODO: bpf_link__pin(fw->xdp, "<pin_dir>/xdp") (same for connect4, bind4)
   *       __firewall_pin_map for every map */
  return FW_ERR_UNSUPPORTED;
}

void firewall_close(Firewall* fw) {
  if (fw == NULL)
    return;

  /* TODO: bpf_link__destroy each link, close(cgroup_fd) if >= 0, fw_bpf__destroy(skel), free(fw) */
}

i32 firewall_map_fd(const Firewall* fw, const FwMap map) {
  if (fw == NULL || fw->skel == NULL)
    return -1;

  /* TODO: switch (map) → bpf_map__fd(fw->skel->maps.fw_cidr), ... */
  (void)map;
  return -1;
}

// ================# PRIVATE #================

error8 __firewall_resize(Firewall* fw) {
  /* TODO: for each non-zero opts.s_*: bpf_map__set_max_entries(skel->maps.<map>, s) */
  return FW_ERR_UNSUPPORTED;
}

error8 __firewall_attach_xdp(Firewall* fw) {
  /* TODO: ifindex = if_nametoindex(ifname) → FW_ERR_IFINDEX
   *       opts.xdp_generic: bpf_program__set_flags / XDP_FLAGS_SKB_MODE
   *       fw->xdp = bpf_program__attach_xdp(skel->progs.fw_xdp, ifindex) */
  return FW_ERR_UNSUPPORTED;
}

error8 __firewall_attach_cg(Firewall* fw) {
  /* TODO: cgroup_fd = open(cgroup_path, O_RDONLY | O_DIRECTORY) → FW_ERR_CGROUP_OPEN
   *       fw->connect4 = bpf_program__attach_cgroup(skel->progs.fw_connect4, cgroup_fd)
   *       fw->bind4    = bpf_program__attach_cgroup(skel->progs.fw_bind4, cgroup_fd) */
  return FW_ERR_UNSUPPORTED;
}

error8 __firewall_pin_map(const Firewall* fw, struct bpf_map* map) {
  /* TODO: snprintf "<pin_dir>/<bpf_map__name(map)>", bpf_map__pin */
  return FW_ERR_UNSUPPORTED;
}

error8 __firewall_errno(const i32 err, const error8 fallback) {
  switch (-err) {
    case 0:      return FW_OK;
    case EPERM:
    case EACCES: return FW_ERR_PERM;
    case ENOMEM: return FW_ERR_NOMEM;
    case ENOENT: return FW_ERR_NOT_FOUND;
    case EINVAL: return FW_ERR_INVAL;
    default:     return fallback;
  }
}
