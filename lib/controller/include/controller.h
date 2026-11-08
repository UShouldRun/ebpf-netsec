#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <alloc/alloc.h>
#include <string8/string8.h>

#include "utils.h"
#include "error.h"
#include "firewall.h"

/**
 * @file controller.h
 * @brief User-space policy: rule file → maps, counters export, drop log
 *
 * * Policy lives in maps: applying rules never reloads or re-attaches a program.
 *
 * * Rule file (one rule per line, '#' starts a comment):
 * - cidr    <a.b.c.d/len>                     layer 1, drop source prefix
 * - port    <tcp|udp> <port>                  layer 1, drop destination port
 * - rate    <pps> <burst>                     layer 1, per-source token bucket
 * - connect <cgroup-path> <tcp|udp> <port>    layer 2, allow-list connect(2)
 * - bind    <tcp|udp> <port>                  layer 2, deny bind(2) on host service ports
 *
 * * Each rule gets a rule id (its index, < FW_MAX_RULES) that keys fw_counters.
 *
 * * Memory: one Arena holds the whole rule load (file buffer, split lines, tokens) and is
 * reset after it, so a reload (e.g. on SIGHUP) never grows the controller.
 * - arena_alloc refuses a single allocation > s_arena: size it from the file (see load_rules).
 */

struct ring_buffer; /* libbpf, opaque */

typedef struct controller Controller;

struct controller {
  Firewall* fw;
  struct ring_buffer* events;
  Arena* arena;          // rule-load scratch, reset after every load

  u32 s_rules;

  volatile bool running; // cleared by the signal handler
};

error8 controller_init          (Controller* ctl, Firewall* fw);
error8 controller_load_rules    (Controller* ctl, const char* path);
error8 controller_dump_counters (Controller* ctl, FILE* file);
error8 controller_poll          (Controller* ctl, const i32 timeout_ms);
void   controller_free          (Controller* ctl);

#endif // !__CONTROLLER_H__
