#include "vmlinux.h"
#include <bpf/bpf_helpers.h>

#include "maps.bpf.h"

/* Owns the license for the linked fw.bpf.o: exactly one object may carry
 * the "license" section. GPL is required for the ring buffer helpers. */
char LICENSE[] SEC("license") = "GPL";
