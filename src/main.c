#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static void on_signal(i32 sig) {
  (void)sig;
  ctl.running = false;
}

/* argv strings are NUL-terminated, so the str8 can still be handed to libc/libbpf */
static str8 arg_str8(char* arg) {
  return string8_create(strlen(arg), (u8*)arg);
}

static void usage(const str8 name) {
  fprintf(stderr,
    "usage: %.*s -i <ifname> -c <cgroup-path> -r <rules> [-p <pin-dir>] [-g] [-n <s_cidr>]\n"
    "  -g  generic (SKB) XDP mode\n"
    "  -n  CIDR map size (benchmarks: 1000, 10000, 100000)\n",
    STR8_FMT(name)
  );
}

i32 main(i32 argc, char** argv) {
  FirewallOpts opts = {0};
  str8 rules = STR8_NULL;

  i32 opt;
  while ((opt = getopt(argc, argv, "i:c:r:p:gn:h")) != -1) {
    switch (opt) {
      case 'i': {
        opts.ifname = arg_str8(optarg);
        break;
      }
      case 'c': {
        opts.cgroup_path = arg_str8(optarg);
        break;
      }
      case 'r': {
        rules = arg_str8(optarg);
        break;
      }
      case 'p': {
        opts.pin_dir = arg_str8(optarg);
        break;
      }
      case 'g': {
        opts.xdp_generic = true;
        break;
      }
      case 'n': {
        opts.s_cidr = (u32)strtoul(optarg, NULL, 10);
        break;
      }
      default: {
        usage(arg_str8(argv[0]));
        return EXIT_FAILURE;
      }
    }
  }

  if (opts.ifname.str == NULL || opts.cgroup_path.str == NULL || rules.str == NULL) {
    usage(arg_str8(argv[0]));
    return EXIT_FAILURE;
  }

  Firewall* fw = NULL;
  error8 err = FW_OK;

  err = firewall_open(&fw, &opts);
  if (err != FW_OK)
    goto fail;

  err = firewall_attach(fw);
  if (err != FW_OK)
    goto fail;

  err = firewall_pin(fw);
  if (err != FW_OK)
    goto fail;

  err = controller_init(&ctl, fw);
  if (err != FW_OK)
    goto fail;

  err = controller_load_rules(&ctl, rules);
  if (err != FW_OK)
    goto fail;

  signal(SIGINT,  on_signal);
  signal(SIGTERM, on_signal);

  while (ctl.running) {
    err = controller_poll(&ctl, 100);
    if (err != FW_OK && err != FW_ERR_AGAIN)
      break;
  }

  controller_dump_counters(&ctl, stdout);

fail:
  if (err != FW_OK)
    fprintf(stderr, "fw: %.*s\n", STR8_FMT(error_str(err)));

  controller_free(&ctl);
  firewall_close(fw);
  return err == FW_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
