#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <getopt.h>

#include "firewall.h"
#include "controller.h"

/**
 * @file main.c
 * @brief fw: two-layer eBPF firewall controller
 *
 * usage: fw -i <ifname> -c <cgroup-path> -r <rules> [-p <pin-dir>] [-g] [-n <s_cidr>]
 */

static Controller ctl;

static void on_signal(int sig) {
  (void)sig;
  ctl.running = false;
}

static void usage(const char* name) {
  fprintf(stderr,
    "usage: %s -i <ifname> -c <cgroup-path> -r <rules> [-p <pin-dir>] [-g] [-n <s_cidr>]\n"
    "  -g  generic (SKB) XDP mode\n"
    "  -n  CIDR map size (benchmarks: 1000, 10000, 100000)\n",
    name
  );
}

int main(int argc, char** argv) {
  FirewallOpts opts = {0};
  const char* rules = NULL;

  i32 opt;
  while ((opt = getopt(argc, argv, "i:c:r:p:gn:h")) != -1) {
    switch (opt) {
      case 'i': opts.ifname      = optarg;                   break;
      case 'c': opts.cgroup_path = optarg;                   break;
      case 'r': rules            = optarg;                   break;
      case 'p': opts.pin_dir     = optarg;                   break;
      case 'g': opts.xdp_generic = true;                     break;
      case 'n': opts.s_cidr      = (u32)strtoul(optarg, NULL, 10); break;
      default:  usage(argv[0]); return EXIT_FAILURE;
    }
  }

  if (opts.ifname == NULL || opts.cgroup_path == NULL || rules == NULL) {
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  Firewall* fw = NULL;
  error8 err = FW_OK;

  if ((err = firewall_open(&fw, &opts))         != FW_OK) goto fail;
  if ((err = firewall_attach(fw))               != FW_OK) goto fail;
  if ((err = firewall_pin(fw))                  != FW_OK) goto fail;
  if ((err = controller_init(&ctl, fw))         != FW_OK) goto fail;
  if ((err = controller_load_rules(&ctl, rules)) != FW_OK) goto fail;

  signal(SIGINT,  on_signal);
  signal(SIGTERM, on_signal);

  while (ctl.running)
    if ((err = controller_poll(&ctl, 100)) != FW_OK && err != FW_ERR_AGAIN)
      break;

  controller_dump_counters(&ctl, stdout);

fail:
  if (err != FW_OK)
    fprintf(stderr, "fw: %s\n", error_str(err));

  controller_free(&ctl);
  firewall_close(fw);
  return err == FW_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
