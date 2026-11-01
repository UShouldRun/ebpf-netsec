# Two-Layer eBPF Firewall

eBPF-based firewall for Assignment #2 of **Network Security 2026/27**,
Faculty of Sciences · University of Porto: an **XDP packet layer** and a
**cgroup/socket workload layer**, driven by one user-space controller.

- António Lanção — up202205416
- David Sá — up202303580
- Henrique Teixeira — up202306640

## eBPF in one paragraph

Extended Berkeley Packet Filter (eBPF) lets small, verified programs run
*inside* the kernel. A program is compiled from restricted C to bytecode,
proven safe by an in-kernel **verifier** (termination, memory safety, type
safety over all paths), JIT-compiled to native code, then attached to a
**hook point** — a place in the kernel where events pass. **Maps** are the
kernel-provided key→value stores that give programs memory across events
and serve as the channel to user space. Safety is paid once at load;
execution runs at native speed with zero per-event overhead.

## The prototype

The firewall turns one security question into two layers: XDP answers
*"what may this packet do"*, the cgroup/socket hooks answer *"what may
this workload do"*. Enforcement lives in the kernel; policy lives in
maps, updated at runtime by the controller — no program reloads.

![Prototype architecture: a user-space controller feeding maps shared with
an XDP ingress layer and a cgroup/socket
layer](report/figures/architecture.svg)

### Layer 1 — XDP ingress (packets)

Runs in the driver receive path, **before** the socket buffer exists —
the cheapest place in the kernel to drop a packet. Three rule classes,
each backed by its own map:

| Rule class       | Map type      | Notes                              |
|------------------|---------------|-------------------------------------|
| CIDR blocklist   | LPM trie      | longest-prefix match on source IP   |
| Port/protocol    | hash map      | denylist keyed by port + protocol   |
| Rate limit       | per-CPU hash  | per-source counters, no locks in hot path |

Every verdict increments per-rule counters; dropped packets are sampled
into a **ring buffer** for logging.

### Layer 2 — cgroup/socket (workloads)

Attached to each container's cgroup. Sees `connect(2)`/`bind(2)` with the
workload's identity attached — no packet is ever parsed. Only allow-listed
containers may connect to a given network or port; no container may bind
the host's service ports.

### Controller (user space, libbpf/CO-RE)

Reads a declarative rule file, populates and updates the maps without
reloading programs, pins programs and maps in bpffs, and exports counters
and drop logs. CO-RE (via BTF relocations resolved at load time) lets one
binary run across kernel versions.

### Evaluation (vs. nftables)

Enforcement correctness (positive/negative cases per rule class, both
layers), maximum drop rate per CPU with XDP vs. nftables applying the
same rules, CPU overhead, latency, map sizes at 1k/10k/100k rules — and a
**privilege audit**: the firewall must load and attach with only
`CAP_BPF` + `CAP_NET_ADMIN`, never `CAP_SYS_ADMIN`.

### Roadmap (weeks from 2 Nov to 6 Dec 2026)

| #  | Milestone                                                     | Week |
|----|---------------------------------------------------------------|------|
| M1 | Minimal XDP program: static CIDR drop with per-CPU counters   | 1    |
| M2 | Controller: rule file → maps; cgroup hooks; ring-buffer log    | 1--2 |
| M3 | Policy test suite (all metrics-table cases, both layers)      | 2    |
| M4 | Benchmark harness and nftables baseline                       | 3--4 |
| M5 | Threat-model validation: privilege audit + map-tamper test    | 4    |
| M6 | Reproducibility artefacts: fresh VM from the README runs it    | 4    |

## More on eBPF

A longer technical introduction — verifier internals, hook-by-hook
comparison, production (Cilium, Tetragon) vs. research (bpfbox,
BPFContain) systems, and a threat model for eBPF itself — is in the
Assignment #1 report (`report/report.pdf`) and notes in
[`refs/ebpf.md`](refs/ebpf.md). Sources and reading plan:
[`refs/README.md`](refs/README.md).
