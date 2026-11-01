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
      #bullet[Termination: bounded loops, DAG-shaped state.]
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
    `CAP_BPF`, `CAP_PERFMON`, `unprivileged_bpf_disabled`, BPF tokens.
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
    [tc], [`sk_buff`, stack], [Pass / drop / redirect], [Low],
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
    [Tetragon], [Production], [kprobe / tracepoint], [None published],
    [bpfbox], [Research], [LSM + kprobes], [≈8.4% web throughput],
    [BPFContain], [Research], [LSM], [Under 16%, on par w/ AppArmor],
  )
  #v(0.4em)
  #bullet[Production (Cilium/Tetragon): label identity, orchestration-native policy]
  #bullet[Research (bpfbox/BPFContain): confinement from the threat model up]
  #box(width: 100%, fill: accent, stroke: (left: 3pt + secondary), inset: 10pt, radius: 2pt)[
    #text(weight: "bold", fill: primary)[Finding:] what separates them is *policy authorship* and the control plane's exposure ,  not capability. Overhead is only comparable in the research row.
  ]
]

#pagebreak()

// ══ 6. Threat model ══
#slide[eBPF itself is attack surface][
  #text(weight: "bold", fill: secondary)[Adversaries, from the literature]
  #v(0.4em)
  #bullet[Over-privileged container: 2.5% of Docker Hub images, no exploit needed]
  #gap
  #bullet[Verifier/JIT bugs: 22 CVEs in two years]
  #gap
  #bullet[Policy bypass: TOCTOU, fail-open on event loss]
  #gap
  #bullet[Shared kernel is NOT a shared boundary: tracing ignores namespaces]
  #v(0.8em)
  #box(width: 100%, fill: rgb("#fdecea"), stroke: (left: 3pt + rgb("#c62828")), inset: 10pt, radius: 2pt)[
    #text(weight: "bold")[The uncomfortable finding] ; the the verifier is a hand-written, growing TCB; the literature argues it cannot scale as kfuncs multiply, and cross-container attacks work regardless of what it proved about the programs.
  ]
]

#pagebreak()

// ══ 7. Prototype ══
#slide[Assignment \#2: profiler + rule-based detector][
  #grid(columns: (1fr, 1fr), gutter: 1.5em,
    [
      #text(weight: "bold", fill: secondary)[Design]
      #gap
      #bullet[`perf_event` stack sampling: "what is it doing"]
      #gap
      #bullet[Tracepoints on exec + syscalls: "what did it become"]
      #gap
      #bullet[User-space detector: rates, n-grams, z-scores]
      #gap
      #bullet[Detection only, no enforcement]
    ],
    [
      #text(weight: "bold", fill: secondary)[Evaluation targets]
      #gap
      #bullet[100% of threat-model replays detected]
      #gap
      #bullet[Under 5% CPU at 10 Hz]
      #gap
      #bullet[FPR reported, not hidden]
      #gap
      #bullet[Loads with `CAP_BPF` + `CAP_PERFMON` only]
    ]
  )
  #v(0.8em)
  #box(width: 100%, fill: accent, stroke: (left: 3pt + secondary), inset: 10pt, radius: 2pt)[
    #text(weight: "bold", fill: primary)[Security property demonstrated:] detection fidelity *at deployable overhead*, with least privilege (the gap no surveyed system publishes).
    ]
]

#pagebreak()

// ══ 8. Roadmap ══
#slide[Roadmap: deadline 6 December 2026][
  #table(
    columns: (auto, 1fr, 1fr, auto),
    align: center + horizon,
    stroke: 0.5pt + rgb("#d1d5db"),
    fill: (_, row) => if row == 0 { secondary } else if calc.odd(row) { white } else { rgb("#f3f4f6") },
    inset: (x: 8pt, y: 6pt),
    table.header[#text(fill: white, weight: "bold", size: 12pt)[\#]][#text(fill: white, weight: "bold", size: 12pt)[Milestone]][#text(fill: white, weight: "bold", size: 12pt)[Validation]][#text(fill: white, weight: "bold", size: 12pt)[Week]],
    [M1], [Program loads and attaches], [`bpftool prog` shows the hook], [1],
    [M2], [Controller / telemetry], [Events at idle and under load], [1],
    [M3], [Detector + test cases], [Fires on replays, silent on soak], [2],
    [M4], [Benchmark + baseline], [Targets met at 10 Hz], [3--4],
    [M5], [Privilege audit], [`CAP_BPF` + `CAP_PERFMON` only], [4],
    [M6], [Reproducibility artefacts], [Fresh clone reproduces a run], [5],
  )
  #v(0.9em)
  #text(fill: muted)[35 days from today; 5 working weeks; M6 leaves the final week free for the report and packaging (`TP1_group7_report_2.pdf`).]
]

#pagebreak()

// ══ 9. Conclusion ══
#slide[Conclusion][
  #bullet[The *hook is the security decision*: same bytecode, three defense regimes]
  #gap
  #bullet[Production vs. research: policy authorship, not capability]
  #gap
  #bullet[The verifier is a growing TCB; the kernel is a shared attack surface]
  #gap
  #bullet[A\#2: detection with least privilege, every number published]
  #v(1em)
  #align(center)[
    #line(length: 30%, stroke: 1pt + secondary)
    #v(0.5em)
    #text(fill: primary, size: 18pt)[Questions?]
  ]
]
