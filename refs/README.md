# References — Topic 9: Increasing Network Security with eBPF

`assignment_topics.pdf` is the brief: Topic 9 is on pp. 32–34, the common
project model on p. 5 and the submission rules on p. 52.

BibTeX for everything below is in `../report/refs.bib`.

## From the brief

| File / link | Cite key | Use it for |
|---|---|---|
| [Linux BPF docs](https://docs.kernel.org/bpf/) | `kernel-bpf-docs` | Program types, maps, BTF |
| [eBPF verifier](https://docs.kernel.org/bpf/verifier.html) | `kernel-bpf-verifier` | Safety model |
| [BPF LSM](https://docs.kernel.org/bpf/prog_lsm.html) | `kernel-bpf-lsm` | Runtime enforcement hook |
| [Cilium BPF & XDP guide](https://docs.cilium.io/en/stable/reference-guides/bpf/) | `cilium-bpf-guide` | Hooks, architecture, production case |
| [Tetragon](https://tetragon.io/docs/) | `tetragon` | Production case study |
| [libbpf-bootstrap](https://github.com/libbpf/libbpf-bootstrap) | `libbpf-bootstrap` | CO-RE, prototype starting point |
| `papers/findlay2020-bpfbox-ccsw.pdf` | `findlay2020bpfbox` | Research case study |
| `papers/findlay2021-bpfcontain-arxiv.pdf` | `findlay2021bpfcontain` | Research case study (containers) |
| `papers/he2023-cross-container-attacks-usenix.pdf` | `he2023crosscontainer` | Threat model |

## Extra foundation

| File / link | Cite key | Use it for |
|---|---|---|
| `papers/hoiland-jorgensen2018-xdp-conext.pdf` | `hoiland2018xdp` | The XDP paper: design and performance numbers |
| `papers/vieira2019-ebpf-xdp-sbc-pt.pdf` | `vieira2020survey` | Portuguese SBRC 2019 short-course chapter by the same authors as the ACM CSUR survey (cite the CSUR version, which is paywalled: [doi](https://doi.org/10.1145/3371038)) |
| `papers/jia2023-verification-untenable-hotos.pdf` | `jia2023untenable` | Critique of the verifier as TCB |
| PREVAIL, PLDI '19 — not downloaded, [doi](https://doi.org/10.1145/3314221.3314590) (try via U.Porto VPN) | `gershuni2019prevail` | Formal alternative to the Linux verifier |
| Liz Rice, *Learning eBPF*, O'Reilly 2023 (book, not downloaded) | — | Easiest introduction; has a security chapter |

## Reading plan for Assignment #1

Everything above adds up to about 200 pages: the papers are 119, the Cilium
guide about 55, the kernel docs about 25 and Tetragon about 3. Web-doc page
counts are estimates at about 500 words per page. Assignment #1 needs about
**50 pages** of that.

### What to read (about 50 pages)

| Source | Sections | ≈ pp | Report section |
|---|---|---|---|
| He et al. 2023 | §1, §2.3, §3 (threat model), §4.1, skim §4.2–4.4, §5.3–5.4, §6 | 8 | Threat model (the core reading) |
| XDP paper | §3 (design: eBPF VM, maps, verifier), §4.1–4.2 headline results, §5.2 DoS mitigation | 6 | Execution model, XDP, use case A |
| BPFContain | §1, §3.1–3.2, skim §4, §6, §8.2 | 6 | Research case study |
| bpfbox | §1, §3, §5.1–5.2, §8 (results only), §9.1 | 5 | Research case study |
| Jia et al. 2023 | §1–2 | 4 | Verifier limits, open challenges |
| Kernel verifier doc | Opening part, up to the detailed register-tracking material | 3 | Verifier |
| Kernel BPF LSM doc | All | 2 | BPF LSM hook |
| Cilium: [Architecture](https://docs.cilium.io/en/stable/reference-guides/bpf/architecture/) | Maps, Helpers, Object pinning, JIT, Hardening (skip instruction-set details) | 6 | Execution model, privileges |
| Cilium: [Program types](https://docs.cilium.io/en/stable/reference-guides/bpf/progtypes/) | Conceptual parts of XDP and tc (skip code walkthroughs) | 5 | Hook comparison |
| Tetragon | [Overview](https://tetragon.io/docs/overview/), [Tracing policy](https://tetragon.io/docs/concepts/tracing-policy/), [Enforcement](https://tetragon.io/docs/concepts/enforcement/) | 3 | Production case study |
| [libbpf overview](https://docs.kernel.org/bpf/libbpf/libbpf_overview.html) | CO-RE section only | 1 | Portability |

bpfbox and BPFContain have the same authors. The brief only needs one
research mechanism, so read BPFContain in depth (containers, network policy)
and skim bpfbox. Its §4 is a good short introduction to eBPF.

### Skip for Assignment #1

- Cilium *Toolchain* and *Debug & Test* pages: about 31 pages, useful only for Assignment #2.
- libbpf-bootstrap: code, for Assignment #2.
- `btf.html`: the format specification. One paragraph on what BTF/CO-RE do is enough.
- Vieira chapter: Portuguese backup. §3.3.3 (verifier) and §3.4 (hooks) are good if a concept doesn't click.
- PREVAIL: not downloaded. If you can't get it through the VPN, drop the citation or cite only what Jia et al. say about it. Don't cite it from the abstract alone.

### Split across three people (about 17 pages each)

1. **eBPF model and verifier:** XDP §3, kernel verifier doc, Cilium Architecture, Jia et al., libbpf CO-RE.
2. **Hooks and production system:** Cilium Program types, BPF LSM doc, XDP §4–5, Tetragon.
3. **Research systems and threat model:** He et al., BPFContain, bpfbox.

Each person writes the report sections their reading covers. The whole group
writes the roadmap together, since it depends on which prototype you choose.
