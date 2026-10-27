#import "template.typ": report, todo, note-box

#show: report.with(
  title: "Increasing Network Security with eBPF",
  subtitle: "Assignment #1 — State of the art",
  authors: (
    (name: "Henrique Teixeira", id: "up202306640"),
    (name: "TODO",              id: "up2025XXXXX"),
    (name: "TODO",              id: "up2025XXXXX"),
  ),
  course: "Network Security 2026/27",
  institution: "Faculty of Sciences · University of Porto",
  abstract: [
    #todo[150–250 words, written last: the problem (programmable kernel hooks
    for network and runtime security), why it is hard (verifier as TCB,
    privilege boundary, new attack surface), what this report compares
    (hooks, production vs. research systems, threat model), and the
    prototype planned for Assignment \#2.]
  ],
)

= Introduction

#todo[Context: eBPF runs verified programs at kernel hook points and shares
state with user space through maps @kernel-bpf-docs. For network security
this spans early packet filtering (XDP) to runtime enforcement (BPF LSM).
State the gap: eBPF is a substrate, not a product, and it is itself
security-sensitive @he2023crosscontainer.]

#todo[Contributions as 3–4 checkable bullets, e.g. a hook comparison for two
use cases, a production-vs-research comparison, a threat model for eBPF
itself, and a measurable roadmap for the prototype.]

#todo[One-paragraph outline of the report.]

= The eBPF execution and safety model

== Program lifecycle: bytecode, loading and JIT

#todo[Restricted instruction set, `bpf()` syscall, program types, attach
points, JIT compilation, pinning and lifetime @kernel-bpf-docs
@cilium-bpf-guide. A small figure of the load → verify → JIT → attach
pipeline helps here.]

== The verifier

#todo[What it checks: control-flow (DAG / bounded loops), register and
pointer types, bounds on memory access, helper argument types, program
size and complexity limits @kernel-bpf-verifier. What it does _not_
guarantee. Contrast with the abstract-interpretation approach of PREVAIL
@gershuni2019prevail and the critique in @jia2023untenable.]

== Maps, ring buffers and helpers

#todo[Map types relevant to security (hash, LPM trie, per-CPU arrays, ring
buffer), helpers and kfuncs as the interface to the kernel, and how user
space reads and updates policy.]

== Portability: BTF and CO-RE

#todo[Kernel-version coupling, BTF type information, CO-RE relocations with
libbpf @libbpf-bootstrap. Why it matters when the same policy must run on
heterogeneous kernels.]

== Verifier safety vs. authorization to load

#note-box(label: "Key distinction required by the brief")[
  The verifier answers _"is this program safe to run in the kernel?"_; the
  privilege model answers _"who may load and attach it?"_. A program the
  verifier accepts can still be malicious.
]

#todo[`CAP_SYS_ADMIN` vs. `CAP_BPF`/`CAP_PERFMON`/`CAP_NET_ADMIN`, the
`kernel.unprivileged_bpf_disabled` sysctl, BPF token for delegated loading,
and program signing. Verify each against the current kernel docs and cite
them.]

= Hook points for security

== Overview of hooks

#todo[One short paragraph per hook: XDP @hoiland2018xdp, tc, cgroup/socket,
tracepoints/kprobes, BPF LSM @kernel-bpf-lsm. Choose the hook from the
question, not "eBPF is faster".]

#figure(
  caption: [Security-relevant eBPF hooks. #todo[fill in and cite]],
  table(
    columns: (auto, 1fr, 1fr, 1fr, auto),
    align: (left, left, left, left, left),
    table.header[Hook][Context available][Enforcement actions][Overhead][Position],
    [XDP],          [#todo[raw frame]],         [#todo[drop/pass/redirect]], [#todo[]], [driver RX],
    [tc],           [#todo[sk_buff]],           [#todo[]],                  [#todo[]], [ingress/egress],
    [cgroup/socket],[#todo[workload, socket]],  [#todo[]],                  [#todo[]], [socket layer],
    [kprobe/tracepoint], [#todo[]],             [#todo[observe only]],      [#todo[]], [anywhere],
    [BPF LSM],      [#todo[]],                  [#todo[allow/deny]],        [#todo[]], [LSM hooks],
  ),
) <hook-table>

== Use case A: #todo[e.g. volumetric / DDoS filtering]

#todo[Which hooks can solve it, what each sees, what each can enforce, and
the expected overhead. Compare with nftables as the conventional baseline.]

== Use case B: #todo[e.g. process-aware connection control or telemetry]

#todo[Same structure as use case A; the point is that a different question
leads to a different hook.]

= eBPF security systems

== Production: Cilium and Tetragon

#todo[Cilium: data-plane networking, load balancing and identity-based
policy @cilium-bpf-guide. Tetragon: process and kernel observability with
in-kernel enforcement @tetragon. Compare what state each observes, how
identity is represented, and whether enforcement happens before or after
the event reaches the application.]

== Research: bpfbox and BPFContain

#todo[bpfbox: per-process confinement policy enforced through LSM hooks and
kprobes @findlay2020bpfbox. BPFContain: container-aware policy integrated
with container managers @findlay2021bpfcontain. The design trade-off
between high-level semantic policy and low-level hooks; reported overhead.]

== Comparison

#figure(
  caption: [Production vs. research eBPF security systems. #todo[fill in]],
  table(
    columns: (auto, 1fr, 1fr, 1fr, 1fr),
    table.header[System][Policy level][Hooks used][Identity][Reported overhead],
    [Cilium],     [#todo[]], [#todo[]], [#todo[]], [#todo[]],
    [Tetragon],   [#todo[]], [#todo[]], [#todo[]], [#todo[]],
    [bpfbox],     [#todo[]], [#todo[]], [#todo[]], [#todo[]],
    [BPFContain], [#todo[]], [#todo[]], [#todo[]], [#todo[]],
  ),
) <systems-table>

= Threat model for eBPF itself

== Assets, trust boundaries and assumptions

#todo[Kernel integrity, cross-container isolation, policy integrity, secrets
in kernel memory. Boundaries: user space ↔ loader ↔ verifier ↔ kernel;
container ↔ host.]

== Adversaries and attacks

#todo[One paragraph each, with preconditions and impact:]
- *Malicious or compromised loader.* #todo[]
- *Over-privileged container.* #todo[tracing helpers reading other
  containers' data without a kernel exploit @he2023crosscontainer]
- *Unsafe helper or program type.* #todo[]
- *Verifier and JIT bugs as kernel attack surface.* #todo[cite concrete
  CVEs and @jia2023untenable]
- *Policy bypass.* #todo[TOCTOU, semantic gaps between hook and intent,
  event loss under load]

== Mitigations

#todo[Capability restriction, disabling unprivileged BPF, seccomp/AppArmor
profiles for containers, BPF token, signing/auditing of programs, and
which of these the prototype will apply.]

= Limitations and open challenges

#todo[The verifier as part of the TCB; observability vs. enforcement (false
positives, availability impact); portability across kernels; event loss;
operational complexity. Compare approaches rather than list them.]

= Proposed prototype and evaluation criteria

== Scope

#todo[Pick one, distinct from the other group on this topic: XDP filtering
with counters, connection telemetry correlated with processes, or a
BPF-LSM policy protecting one resource. State the security property it
demonstrates.]

== Metrics

#figure(
  caption: [Evaluation criteria for Assignment \#2.],
  table(
    columns: (auto, 1fr, 1fr),
    table.header[Metric][How it is measured][Target / baseline],
    [Correctness],         [#todo[positive and negative test cases]], [#todo[]],
    [Throughput / event rate], [#todo[]],                          [#todo[vs. nftables]],
    [Latency],             [#todo[]],                              [#todo[]],
    [CPU overhead],        [#todo[]],                              [#todo[]],
    [Memory / map usage],  [#todo[]],                              [#todo[]],
    [Event loss / false positives], [#todo[]],                     [#todo[]],
    [Privileges required], [#todo[capabilities before and after]], [#todo[]],
  ),
) <metrics-table>

== Environment

#todo[Isolated VM(s), kernel version, clang/libbpf versions, traffic
generator, and whether the prototype runs natively or in a fallback mode.]

= Implementation roadmap for Assignment \#2

#figure(
  caption: [Milestones, dependencies and validation. #todo[set dates once
  the Assignment \#2 deadline is on Moodle]],
  table(
    columns: (auto, 1fr, auto, 1fr, auto),
    table.header[\#][Milestone and deliverable][Depends on][Validation][Week],
    [M1], [Minimal program loads and attaches], [—],  [#todo[]], [#todo[]],
    [M2], [User-space controller / telemetry],  [M1], [#todo[]], [#todo[]],
    [M3], [Policy and test cases],              [M2], [#todo[]], [#todo[]],
    [M4], [Benchmark harness and baseline],     [M3], [#todo[]], [#todo[]],
    [M5], [Threat-model validation (privilege audit)], [M3], [#todo[]], [#todo[]],
    [M6], [Reproducibility artefacts and README], [M4, M5], [#todo[]], [#todo[]],
  ),
) <roadmap-table>

= Ethical and legal considerations

#todo[All experiments in isolated, authorized infrastructure; no public
Internet targets; offensive tests (e.g. cross-container reads) only against
our own containers.]

= Conclusion

#todo[What the comparison showed, the main open challenges, and how the
prototype will address the research questions.]

#bibliography("refs.bib", style: "ieee")
