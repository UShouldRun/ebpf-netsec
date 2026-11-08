#ifndef __FIREWALL_H__
#define __FIREWALL_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "error.h"
#include "maps.h"

/**
 * @file firewall.h
 * @brief BPF object lifecycle: one linked fw.bpf.o, one skeleton, both layers
 *
 * * Lifecycle:
 * - firewall_open:   open skeleton → resize maps → load (the verifier runs here).
 * - firewall_attach: XDP on opts->ifname, connect4/bind4 on opts->cgroup_path.
 * - firewall_pin:    pin links and maps under opts->pin_dir, so the firewall outlives
 * the controller and a restarted controller can reuse the maps (no reload).
 * - firewall_close:  detach and free (pinned objects stay in bpffs).
 *
 * * Privileges: CAP_BPF + CAP_NET_ADMIN only (never CAP_SYS_ADMIN), plus write access to bpffs.
 */

typedef struct firewall      Firewall;
typedef struct firewall_opts FirewallOpts;

struct firewall_opts {
  const char
    *ifname,      // XDP interface, e.g. "eth0"
    *cgroup_path, // cgroup v2 parent, e.g. "/sys/fs/cgroup/fw.slice"
    *pin_dir;     // bpffs directory, NULL = don't pin

  u32
    s_cidr,       // max entries, 0 = FW_MAX_CIDR
    s_ports,      // max entries, 0 = FW_MAX_PORTS
    s_rate,       // max entries, 0 = FW_MAX_RATE
    s_cgroup;     // max entries, 0 = FW_MAX_CGROUP

  bool xdp_generic; // SKB mode for veth / VMs (slower, say so in the benchmarks)
};

typedef enum fw_map {
  FW_MAP_CIDR = 0,
  FW_MAP_PORTS,
  FW_MAP_RATE_CFG,
  FW_MAP_RATE,
  FW_MAP_CG_CONNECT,
  FW_MAP_CG_BIND,
  FW_MAP_COUNTERS,
  FW_MAP_EVENTS,
  FW_MAP_COUNT /* keep last */
} FwMap;

error8 firewall_open   (Firewall** fw, const FirewallOpts* opts);
error8 firewall_attach (Firewall* fw);
error8 firewall_pin    (Firewall* fw);
void   firewall_close  (Firewall* fw);

i32    firewall_map_fd (const Firewall* fw, const FwMap map);

#endif // !__FIREWALL_H__
