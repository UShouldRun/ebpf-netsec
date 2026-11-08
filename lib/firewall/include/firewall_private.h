#ifndef __FIREWALL_PRIVATE_H__
#define __FIREWALL_PRIVATE_H__

#include "firewall.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <net/if.h>

#include <bpf/libbpf.h>
#include <bpf/bpf.h>

#include "fw.skel.h" /* generated: bpftool gen skeleton build/fw.bpf.o */

/**
 * @file firewall_private.h
 * @brief Implementation details: the skeleton and the attach links
 *
 * - Links (bpf_link) instead of legacy attach: detaching is bpf_link__destroy, and a pinned
 * link keeps the program attached after the process exits.
 */

struct firewall {
  /* Suggestion:
   *   struct fw_bpf* skel;  // generated skeleton: programs and maps
   *   struct bpf_link
   *     *xdp,               // layer 1 attachment
   *     *connect4,          // layer 2 attachments
   *     *bind4;
   *   i32
   *     ifindex,            // interface XDP is attached to
   *     cgroup_fd;          // cgroup the hooks are attached to
   *   FirewallOpts opts;    // copy of the options it was opened with
   */
};

error8 __firewall_resize      (Firewall* fw);                   /* bpf_map__set_max_entries, before load */
error8 __firewall_attach_xdp  (Firewall* fw);                   /* if_nametoindex + bpf_program__attach_xdp */
error8 __firewall_attach_cg   (Firewall* fw);                   /* open(cgroup_path) + bpf_program__attach_cgroup */
error8 __firewall_pin_map     (const Firewall* fw, struct bpf_map* map);
error8 __firewall_errno       (const i32 err, const error8 fallback); /* libbpf -errno → error8 */

#endif // !__FIREWALL_PRIVATE_H__
