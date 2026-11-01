// Slides deck for the Assignment #1 report, same house palette as template.typ
#set page(width: 30cm, height: 16.875cm, margin: 2cm, footer: context [Network Security 2026/27 · #counter(page).display("1")])
#set text(font: "Libertinus Serif", size: 16pt)
#set par(justify: false, leading: 0.7em)

#let primary   = rgb("#1a3a5c")
#let secondary = rgb("#2e6da4")
#let accent    = rgb("#e8f0f8")
#let muted      = rgb("#667085")

#let slide(title, body) = [
  #v(0.6em)
  #text(fill: primary, size: 26pt, weight: "bold")[#title]
  #v(0.2em)
  #line(length: 100%, stroke: 1.2pt + secondary)
  #v(0.8em)
  #body
]

// compact header for full-height figure slides
#let figslide(title, body) = [
  #text(fill: primary, size: 22pt, weight: "bold")[#title]
  #v(-0.4em)
  #line(length: 100%, stroke: 1.2pt + secondary)
  #v(-0.2em)
  #body
]

#let bullet(body) = grid(columns: (auto, 1fr), column-gutter: 0.6em, text(fill: secondary)[•], body)
#let gap = v(0.55em)

// ══ 1. Title ══
#v(2.5em)
#align(center)[
  #text(fill: primary, size: 40pt, weight: "bold")[Increasing Network Security with eBPF]
  #v(0.4em)
  #text(fill: muted, size: 20pt)[Assignment \#1: State of the art]
  #v(1.5em)
  #line(length: 40%, stroke: 1.5pt + secondary)
  #v(0.8em)
  #text(size: 14pt)[
    António Lanção (up202205416) \
    David Sá (up202303580) \
    Henrique Teixeira (up202306640)
  ]
  #v(0.8em)
  #text(fill: muted, size: 13pt)[Network Security 2026/27 · Faculty of Sciences, University of Porto]
]

#pagebreak()

// ══ 2. The problem ══
#slide[The problem: kernel extension is a trade-off][
  #bullet[LKMs: arbitrary code, full kernel privilege, exact kernel match]
  #gap
  #bullet[Fixed subsystems: only the policies their authors anticipated]
  #gap
  #bullet[eBPF: untrusted code, *proven safe before it runs*, attached kernel-wide]
  #gap
  #box(width: 100%, fill: accent, stroke: (left: 3pt + secondary), inset: 10pt, radius: 2pt)[
    eBPF is a *substrate*, not a product. Defense systems are built on top, and the substrate is itself attack surface.
  ]
]

#pagebreak()

// ══ 3. Execution model ══
#slide[Execution and safety model][
  #grid(columns: (1fr, 1fr), gutter: 1.5em,
    [
      #text(weight: "bold", fill: secondary)[Lifecycle]
      #gap
      #bullet[Small fixed instruction set, 11 registers.]
      #gap
      #bullet[Verifier proves safety statically, before JIT.]
      #gap
      #bullet[JIT to native code; attach as event handler.]
      #gap
      #bullet[Maps: the only shared, durable state.]
    ],
    [
      #text(weight: "bold", fill: secondary)[The verifier checks]
      #gap
      #bullet[Termination: DAG check, bounded loops.]
      #gap
      #bullet[Memory safety: range tracking vs. packet bounds.]
      #gap
      #bullet[Types: per-register type state.]
      #gap
      #bullet[*Not* intent, *not* authorization: it is the TCB.]
    ]
  )
  #v(0.8em)
  #box(width: 100%, fill: rgb("#fff3cd"), stroke: (left: 3pt + rgb("#d4a017")), inset: 10pt, radius: 2pt)[
    #text(weight: "bold")[Verifier safety ≠ authorization to load.] Loading is gated separately: \ 
    `CAP_BPF`/`CAP_PERFMON` (Linux 5.8), `unprivileged_bpf_disabled`, BPF tokens.
  ]
]

#pagebreak()

// ══ 4. Hooks ══
#slide[Hook points: the security decision][
  #table(
    columns: (auto, 1fr, 1fr, 1fr),
    align: center + horizon,
    stroke: 0.5pt + rgb("#d1d5db"),
    fill: (_, row) => if row == 0 { secondary } else if calc.odd(row) { white } else { rgb("#f3f4f6") },
    inset: (x: 8pt, y: 6pt),
    table.header[#text(fill: white, weight: "bold", size: 12pt)[Hook]][#text(fill: white, weight: "bold", size: 12pt)[Input]][#text(fill: white, weight: "bold", size: 12pt)[Verdict]][#text(fill: white, weight: "bold", size: 12pt)[Cost]],
    [XDP], [Raw frame, driver RX], [Drop / pass / redirect], [Lowest],
    [tc], [`sk_buff`, ingress/egress], [Pass / drop / redirect], [skb already paid],
    [cgroup/socket], [Workload, socket], [Deny / restrict], [No skb cost],
    [kprobe/tracepoint], [Function args], [Observe only], [Low],
    [BPF LSM], [Security check], [Allow / deny], [Function entry],
  )
  #v(0.9em)
  #grid(columns: (1fr, 1fr), gutter: 1.5em,
    [#box(width: 100%, fill: accent, stroke: (left: 3pt + secondary), inset: 9pt, radius: 2pt)[
      #text(weight: "bold", fill: primary)[Use case A (DDoS):] XDP; identification needs header bytes only, and the target of defense is the machine itself.
    ]],
    [#box(width: 100%, fill: accent, stroke: (left: 3pt + secondary), inset: 9pt, radius: 2pt)[
      #text(weight: "bold", fill: primary)[Use case B (container control):] socket/cgroup; identity-aware policy, BPF LSM as the generalization BPFContain takes.
    ]],
  )
]

#pagebreak()

// ══ 4b. Hook map ══
#figslide[Where each hook sits][
  #grid(columns: (1.6fr, 1fr), gutter: 1.2em, align: horizon,
    image("figures/hooks.svg", height: 10.6cm),
    [
      #bullet[Receive path runs bottom to top]
      #gap
      #bullet[XDP is the only network hook before `sk_buff` allocation]
      #gap
      #bullet[nftables (our baseline) sits after it]
      #gap
      #bullet[kprobes / tracepoints see every layer but only observe]
    ],
  )
]

#pagebreak()

// ══ 5. Systems ══
#slide[Security systems: same substrate, different authors][
  #table(
    columns: (auto, 1fr, 1fr, 1fr),
    align: center + horizon,
    stroke: 0.5pt + rgb("#d1d5db"),
    fill: (_, row) => if row == 0 { secondary } else if calc.odd(row) { white } else { rgb("#f3f4f6") },
    inset: (x: 8pt, y: 6pt),
    table.header[#text(fill: white, weight: "bold", size: 12pt)[System]][#text(fill: white, weight: "bold", size: 12pt)[Kind]][#text(fill: white, weight: "bold", size: 12pt)[Hooks]][#text(fill: white, weight: "bold", size: 12pt)[Overhead]],
    [Cilium], [Production], [XDP / tc / socket], [None published],
    [Tetragon], [Production], [kprobe / tracepoint / uprobe], [None published],
    [bpfbox], [Research], [LSM + probes], [≈8.4% web throughput],
    [BPFContain], [Research], [LSM + probes], [< 16%, ≈ AppArmor],
  )
  #v(0.4em)
  #bullet[Production (Cilium/Tetragon): label/pod identity, orchestration-native policy]
  #bullet[Research (bpfbox/BPFContain): confinement from the threat model up]
  #box(width: 100%, fill: accent, stroke: (left: 3pt + secondary), inset: 10pt, radius: 2pt)[
    #text(weight: "bold", fill: primary)[Finding:] what separates them is *policy authorship* and the control plane's exposure, not capability. Overhead is only comparable in the research row.
  ]
]

#pagebreak()

// ══ 6. Threat model ══
#slide[eBPF itself is attack surface][
  #text(weight: "bold", fill: secondary)[Adversaries, from the literature]
  #v(0.4em)
  #bullet[Over-privileged container: ≈2.5% of active Docker Hub repos, no exploit needed]
  #bullet[Verifier/JIT bugs: ≥22 verifier bugs in 2021–22; JIT bug hijacks control flow]
  #bullet[Unsafe helpers: `bpf_probe_write_user`, map tamper via `bpf_map_get_fd_by_id`]
  #bullet[Policy bypass: process identity confusion, TOCTOU, fail-open on event loss]
  #bullet[Shared kernel is not a boundary: tracing ignores namespaces]
  #v(0.5em)
  #box(width: 100%, fill: rgb("#fdecea"), stroke: (left: 3pt + rgb("#c62828")), inset: 10pt, radius: 2pt)[
    #text(weight: "bold")[The uncomfortable finding:] the verifier is a hand-written, growing TCB; the literature argues it cannot scale as helpers multiply, and cross-container attacks work regardless of what it proved about the programs.
  ]
]

#pagebreak()

// ══ 7. Prototype ══
#slide[Assignment \#2: two-layer eBPF firewall][
  #grid(columns: (1fr, 1fr), gutter: 1.5em,
    [
      #text(weight: "bold", fill: secondary)[Design]
      #bullet[XDP: CIDR blocklist (LPM trie), port denylist, per-source rate limit]
      #bullet[cgroup `connect`/`bind`: per-container egress, no host-port binds]
      #bullet[Controller: rules → maps at runtime; counters + drop log]
    ],
    [
      #text(weight: "bold", fill: secondary)[Evaluation targets]
      #bullet[100% expected verdicts on positive/negative tests]
      #bullet[Drop rate (Mpps) and CPU vs. nftables, same rules]
      #bullet[Zero legitimate drops under flood]
      #bullet[`CAP_BPF` + `CAP_NET_ADMIN` only; map-tamper test]
    ]
  )
  #v(0.5em)
  #box(width: 100%, fill: accent, stroke: (left: 3pt + secondary), inset: 10pt, radius: 2pt)[
    #text(weight: "bold", fill: primary)[Security property demonstrated:] correct enforcement of a defined policy, cheaper than nftables, with the privileges it adds measured.
    ]
]

#pagebreak()

// ══ 7b. Prototype flow ══
#figslide[Prototype flow][
  #align(center)[#image("figures/architecture.svg", height: 10.6cm)]
]

#pagebreak()

// ══ 8. Conclusion ══
#slide[Conclusion][
  #bullet[The *hook is the security decision*: same bytecode, three defense regimes]
  #gap
  #bullet[Production vs. research: policy authorship, not capability]
  #gap
  #bullet[The verifier is a growing TCB; the kernel is a shared attack surface]
  #gap
  #bullet[A\#2: XDP + cgroup firewall, least privilege, every number published]
]

#pagebreak()

// ══ 9. Roadmap ══
#slide[Roadmap: deadline 6 December 2026][
  #table(
    columns: (auto, 1fr, 1fr, auto),
    align: center + horizon,
    stroke: 0.5pt + rgb("#d1d5db"),
    fill: (_, row) => if row == 0 { secondary } else if calc.odd(row) { white } else { rgb("#f3f4f6") },
    inset: (x: 8pt, y: 6pt),
    table.header[#text(fill: white, weight: "bold", size: 12pt)[\#]][#text(fill: white, weight: "bold", size: 12pt)[Milestone]][#text(fill: white, weight: "bold", size: 12pt)[Validation]][#text(fill: white, weight: "bold", size: 12pt)[Week]],
    [M1], [XDP CIDR drop + counters], [Attached; counters match sent], [1],
    [M2], [Controller, cgroup hooks, drop log], [Runtime rule changes apply], [1--2],
    [M3], [Policy test suite], [All +/− cases pass], [2],
    [M4], [Benchmark + nftables baseline], [Scripted, repeated runs], [3--4],
    [M5], [Privilege audit + map tamper], [`CAP_BPF` + `CAP_NET_ADMIN` only], [4],
    [M6], [Reproducibility artefacts], [Fresh VM reproduces a run], [4],
  )
  #v(0.6em)
  #text(fill: muted)[35 days from today; 5 working weeks; M6 leaves the final week free for the report and packaging (`TP1_group7_report_2.pdf`).]
  #v(0.4em)
  #align(center)[#text(fill: primary, size: 18pt)[Questions?]]
]
