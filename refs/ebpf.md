# eBPF

### Learner

Can we continue from the PPF chat? I think it somehow crashed. I want you to go reread everything that I've said so far. And I want to... So, I got the last question correct, but I want to stage you my thought process, which I didn't and I forgot to tell you. Basically, that's it.

### Tool

*list_sessions result delivered*

### Teacher

I have no record of the previous "PPF chat" session — it looks like it may have been wiped or was in a different session context. I don't have any memory of what we discussed or what your last question was.

To pick up where you left off, I need to know what you're working on.

### Learner

eBPF, thats what I meant

### Question

**Q:** I checked the persistent knowledge graph and it's empty — the eBPF session's records didn't survive the crash, so I can't reread anything you said there. I need you to rebuild that context for me before we continue. What was the last question you got right, and roughly where were we in the material?

The learner answered: You
So I need to learn about eBPF, which is a technology on the Linux kernel system, about network security. So this is a project for network security and I need to go over some papers about it to learn so I can draft the report. Can you help me with that?

AGENT’S THINKING


this is the content I have from it

Question
What is the specific scope of the eBPF knowledge required for your network security project, and what is your current understanding of the foundational concepts?

To ensure the material is relevant to your project, we need to define the scope of understanding required. eBPF is an extensive system that touches upon kernel tracing, packet filtering, and high-performance networking, and security is an application built upon those foundations.

Please clarify the following:

What specific area of network security using eBPF does your project focus on? (e.g., malware detection, firewall enforcement, network traffic visibility, compliance auditing, etc.)
What is your existing knowledge base regarding Linux kernel internals, networking concepts, or general security principles? This helps determine where we start.
“I have readme and documents that were basically results from search from claude code and I just need to go over them and then help me learn about it.”

AGENT’S THINKING


AGENT’S THINKING


AGENT’S THINKING


AGENT’S THINKING


AGENT’S THINKING

Retrying…
Retrying…
You
/home/henrique/Documents/College/MCC/S1/NetSecurity/project

AGENT’S THINKING


AGENT’S THINKING


AGENT’S THINKING

I've read your README and the report skeleton — the structure is already laid out well. Before we start learning, I need to measure where you stand so the teaching hits your actual edge. Let me probe a few strands, starting at the foundation.

Probe
correct
In one picture: how does an eBPF program relate to the Linux kernel?

A special syscall filter like seccomp, running entirely in user space

A loadable kernel module written in C that must be recompiled per kernel version

Bytecode loaded into the kernel, verified for safety, then JIT-compiled and attached to a hook point

A user-space process that watches kernel events through a socket

90%
YOUR THINKING
Why

AGENT’S THINKING

Correct — he picked the bytecode/verifier/JIT model, and his thinking shows he's already read the He et al. introduction and threat-model section. Next strand: the verifier, since the report hinges on it.

Probe
correct
What does the eBPF verifier actually check before a program is allowed to run?

It checks that the program's source code was written by a user with CAP_SYS_ADMIN

It runs the program on test inputs to make sure it does not crash

It only checks that the program does not exceed the instruction limit

By static analysis over all possible paths: control flow terminates, register states are tracked, every memory access is bounds-checked, and helper argument types match

70%
Why

AGENT’S THINKING

Correct, though less confident — fragile on verifier details, which is where Jia et al. will bite later. Next: hook points, the comparison table your report is built around

### Teacher

The transcript helps — I can see where we were: a network security project report on eBPF, papers (He et al. and Jia et al.), and you'd just answered the verifier question correctly. The transcript cuts off at the hook points strand, so I don't know whether that question was asked or answered. Let me rebuild the material context from your files first.

### Tool

*ls result delivered*

### Tool

*ls result delivered*

### Tool

*ls result delivered*

### Tool

*read result delivered*

### Teacher

Context rebuilt. Here's where we were per the transcript:

- **Goal:** Assignment #1 on Topic 9 (network security with eBPF) — reading plan in `refs/README.md`, papers from `papers/`, report skeleton in `report/`.
- **Session state:** You'd answered the eBPF program-model question (bytecode → verified → JIT → attached to hook) and the verifier question (static analysis over all paths: termination, bounds, register tracking, helper typing) — the second one correct but at 70% confidence.
- **Pending:** The hook-points strand was about to be probed, and your thought process for the last question wasn't staged.

Now stage it: tell me your reasoning for that last question — the verifier one, or whichever was actually last — what you were thinking as you picked the answer, and where you felt unsure. Then we resume at the hook-points strand.

### Learner

So, essentially, we're reading the question. So, the question was, what does ZBPF refirer actually check before a program is allowed to run? So, the first answer you gave me was, it checks that the program's source code was written by a user with caps as admin. This makes some sense, but what I ended up thinking was, okay, this is not a necessary condition for the program to be allowed to run. Then, option B, is it runs the program on test inputs to make sure it doesn't crash. This does not make sense because the verifier does not know what the program does exactly. So, he would have, he would need to somehow create tests for it and that would be, otherwise it wouldn't just be viable and the tests wouldn't mean, wouldn't say if the program was okay to run. So, that doesn't make sense. Test C, it only checks that the program does not accent the instruction limits. I don't know what the instruction limits here means. Does it mean CPU instructions? I don't understand, so, but it doesn't make sense. But answer D does make sense here. So, we have static analysis, control for all terminates, extract every memory access is bomb checked and help parameter types match. This makes sense from what I've read from the papers and this is from what eBPF is.

### Teacher

Good staging — that's exactly the transfer I need. Two things stand out in your reasoning:

**Your eliminations are sound but one rests on a shaky premise.** You rejected option B ("runs the program on test inputs") because "the verifier does not know what the program does exactly." Actually the verifier *does* know — that's precisely what static analysis buys it. It doesn't execute the program; it symbolically simulates every reachable path, tracking register states and memory bounds abstractly, so it knows properties of *all* executions without running any. That's the whole point of option D. Your conclusion (B is wrong) is right; your reason was slightly off. The correct reason is: testing only covers the inputs you thought of, and eBPF programs can't be allowed to crash the kernel on an input nobody thought of — so testing is categorically insufficient, not just impractical.

**"Instruction limits" is a real gap.** You flagged not knowing what option C meant — and it's worth pinning down, because the limit is a real verifier check and it ties into why D is the *complete* answer rather than "static analysis, full stop."

Let me check that gap, then we move on to hook points.

### Probe

**Q:** The eBPF verifier does enforce an instruction limit. Why does a limit on the number of executed/analyzed instructions exist as a safety check?

(Think about what an unbounded instruction count would let a program do inside the kernel.)

The learner did not know.

### Teacher

That was an honest decline after good reasoning — the reasoning itself surfaced two things worth fixing before we move to hooks.

**Your loop-forever objection.** You said a program that analyzes behavior might legitimately want to run forever. The model you're missing: an eBPF program is not a daemon. It's an *event handler* — it gets invoked, does a bounded piece of work, and returns. "Run forever" is achieved by being invoked forever (every packet, every syscall), not by looping inside one invocation. The kernel cannot tolerate even one handler that hangs mid-execution, because it's running in kernel context on a CPU the rest of the system needs. That's why termination is non-negotiable, and the instruction limit is how it's enforced — historically by outright banning back-edges (no loops at all until Linux 5.3), then by bounded loops.

**On B vs D:** both are wrong, and you eliminated C and A correctly by your own lights. The JIT angle is a red herring — code *size* limits exist as a separate check, but the instruction limit is about execution/analysis time, not output size.

So the refined picture: **verifier checks = (1) termination — via the instruction/loop bound, (2) memory safety — every access bounds-checked, (3) type safety — helper arguments match, register states tracked** — all proven statically over all paths, never by running the code.

Now the strand the crash interrupted: hook points.

### Probe

**Q:** The same eBPF bytecode can be compiled once, but which hook it attaches to changes fundamentally what the program can see and do. Which pair of hooks is correctly matched to what a program attached there sees?

Recall: XDP processes packets at the driver level, tc sits in the network stack, tracepoints/kprobes observe kernel events, and BPF LSM intercepts security decisions.

The learner did not know.

### Teacher

Honest gap declared — that's the useful signal. The hooks strand starts at zero, but you've told me a lot about your floor: embedded engineer, C, OS internals, compilers. So before we teach hooks, I need to know how much of the Linux *networking* substrate you already hold — the hook concepts are all built on it.

### Probe

**Q:** When a NIC receives a packet, the Linux network stack wraps the frame's data in a struct called an sk_buff (skb) as it moves up through the stack. What is the essential role of this structure?

You know OS internals and C — reason from that.

The learner answered correctly.

### Teacher

That's enough signal. You got it right at 50% confidence by reasoning from your TCP-from-scratch project — the knowledge exists but is fresh and unstructured. Combined with the earlier answers, I have the map I need: the verifier strand sits at a solid floor with a fragile ceiling, the hooks strand starts at zero, and your networking substrate is real but raw — you can reason your way through it once the terms are pinned.

### Tool

*probe_done result delivered*

### Teacher

Probe concluded. The map is set: your program-model and verifier foundations hold (with the termination/instruction-limit nuance to reinforce), and the hook points strand — XDP, tc, tracepoints, BPF LSM — is the genuine gap we build from zero, using your skb knowledge as the anchor. The teaching phase will start there.

### Teacher

Here's the arc I've reasoned out, built on what the probe established.

**Where you stand:** the program model (bytecode → verify → JIT → attach) is settled; the verifier is a floor with one crack (why termination is non-negotiable — you reasoned "maybe a monitoring program *should* run forever," which hides the event-handler model); hooks start at zero; your networking substrate is real but raw, so I'll pin terms as they appear rather than teach networking.

**The spine of the plan:** everything in eBPF-as-security rests on one unconditional truth — *an eBPF program is an event handler invoked by the kernel, and the kernel cannot tolerate a handler that misbehaves even once, because it runs in kernel context*. From that single fact, the verifier's checks, the hook contexts, XDP's design, the security systems, and even the verifier's own limitations (Jia et al.) all fall out as motivated consequences rather than facts to memorize. That's the generator I want you to walk away with.

Three structures carry the derivation:

1. **Safety through proof, not trust** — why static verification is the *only* option, derived from the kernel-context fact. This repairs the termination crack and the "verifier doesn't know what the program does" misconception in one move: the verifier doesn't test, it *proves*, because a test misses inputs nobody thought of and the kernel can't afford one crash.
2. **The hook determines the reality the program sees** — built from your skb knowledge upward: XDP (pre-skb raw frame), tc (skb exists), tracepoints/kprobes (kernel events), BPF LSM (security decisions). Each hook exists because someone needed to intervene at a different point in time — that's the motivation, not arbitrary taxonomy.
3. **Maps as the only durable memory** — programs are stateless; persistence and cross-program communication go through maps. That's why every security system (BPFContain, Tetragon) is an *architecture* of programs + maps, not a single program.

The research systems and threat model (He et al., BPFContain, bpfbox) then become applications of these three structures — you'll read them as instances of patterns you already hold, which is what the report writing needs. The Jia et al. critique lands last: once you've seen the verifier as a proof system, its costs (complexity, path explosion, unprovable TCB) are visible as the natural price of the design.

### Tool

*plan_init result delivered*

### Teacher

The plan is live. Accept it or ask for changes — once you're on board, I'll begin at the event-handler truth.

### Learner

Go for it

### Teacher

## Step 1 — The one fact everything rests on

Let's lay the foundation stone, and it needs nothing else to be true:

> [!IMPORTANT]
> **An eBPF program is an event handler invoked by the kernel, and it runs in kernel context.**

Unpack that, because the consequences are the whole subject:

- **Event handler.** Nothing *runs* an eBPF program continuously. It sits there as compiled code, and the kernel *calls* it when something happens — a packet arrives at the NIC, a syscall executes, a function is entered. Same shape as a signal handler or an interrupt handler you know from embedded work: registered, dormant, invoked by an event, runs, returns. "Monitoring everything" is achieved by *being invoked on every event*, never by looping inside one invocation.

- **Kernel context.** When it runs, it runs on the same CPU, in the same privilege ring, as the kernel itself. There is no process boundary between your program and the kernel's own code. It is *inside the trust boundary of the operating system* — a place user-space code can normally never reach, because the kernel is the one thing that must trust nothing.

Hold that second part, because it's what forces everything else in the subject:

**The kernel cannot tolerate a misbehaving handler — not even once.** If your handler dereferences a bad pointer, it doesn't crash *your* program. There is no "your program." It corrupts or panics *the kernel* — the machine every process on the system depends on. A user-space library can segfault and you lose that process. An event handler running in kernel context that misbehaves once loses the machine. And it can be triggered by any event the hook fires on — including one an attacker crafts on purpose: a packet, a syscall, a filename.

From this single fact, the verifier's existence is already motivated — not as a feature eBPF designers bolted on, but as the *only way* the second sentence can be reconciled with the first. You're loading untrusted code into the trust boundary. Something must establish, *before it ever runs*, that it can't misbehave. That's the next step, and I want to know how you'd solve it before I show you how it was solved:

### Quiz

**Q:** You must guarantee that untrusted eBPF bytecode, loaded into the kernel, can never crash the kernel — proven before it runs, on every possible input, including attacker-crafted ones. What approach would you reach for to get that guarantee?

The learner answered incorrectly.

### Quiz

**Q:** You must guarantee that untrusted eBPF bytecode, loaded into the kernel, can never crash the kernel — proven before it runs, on every possible input, including attacker-crafted ones. What approach would you reach for to get that guarantee?

The learner answered incorrectly.

### Teacher

## Step 2 — Why static proof is the only option

Your prediction was the sandbox. Let's resolve it: the answer is **static analysis over all possible executions**, and your own reasoning in the thinking field had already spotted why — you said fuzzing "may never create a pathway that crashes the program," and static analysis is "good, although it is exponential." Both observations are correct. Now I'll connect them.

Three candidate approaches, and the failure of each one is the motivation for the next:

**1. Fuzzing.** You test the program against a corpus of inputs and reject anything that crashes. But the requirement is *every possible input, including attacker-crafted ones*. A crash triggered by one malformed packet header nobody happened to generate — that's a kernel panic waiting for the attacker to find it. Testing covers the inputs you thought of. The guarantee needs the inputs you didn't. **Fuzzing gives probability, the requirement is certainty.** Out.

**2. Sandboxing.** Run the code inside a sub-environment and catch faults at runtime. This is how you sandbox *user-space* code — and it works there because the kernel is the sandbox and the crashing process is disposable. But for eBPF the code runs *in kernel context* — there is no environment below the kernel to catch a fault in. A fault in kernel context *is* the kernel faulting. The sandbox has no lower floor to catch you. **The runtime approach needs a layer below the one you're protecting; this is the layer you're protecting.** Out.

**3. Signing / review.** A trusted authority reviews the source. That's policy, not a guarantee — and it means eBPF is only usable by code someone pre-approved, which kills the entire design goal: programs can be written by anyone, about anything, at any time. Out.

What's left is the only thing that can be true *before* execution and *for all inputs*: **reason about the program itself — its control flow and its memory accesses — without running it.** You've done a version of this in your compiler work: it's static analysis. The verifier doesn't ask "does this program crash on input X?" It asks "does *any* path through this program lead to an unsafe operation, *for any* values the variables might hold?" Answered exhaustively over all paths, before the program ever runs, once, at load time. Certainty, not probability. That's the whole design decision.

And here's the discovery that makes it *feasible* rather than impossible — this is where your "exponential" worry is answered. The verifier doesn't need to know *what* the program computes, only *what values its registers can hold*. So it walks the control-flow graph, and at each instruction it tracks an **abstract state**: for each register, not "the value is 7," but "the value is in range [0, 1024]" — a *set* of possible values, not the value. Two things make this trackable:

- **It's inductive over the graph.** If the state before an instruction guarantees its safety, the state after is computed mechanically — the analysis is a fixed-point walk, not a case-by-case proof. Every path in the graph is covered by covering every edge.
- **Values narrow.** After `r1 = 0` and `r1 += 1`, the verifier isn't reasoning about "all integers" — it knows r1 is exactly 1. Precision is what keeps the analysis from drowning in false alarms, and losing precision is exactly the cost you pay when the analysis gets hard (that thread comes back with Jia et al. at the end of the arc).

> [!IMPORTANT]
> **The verifier proves properties of all executions without executing any of them.** Certainty before execution is the only way to satisfy "kernel context, can't fail once" for untrusted code.

Check that the step landed — then next we'll break the proof into its three concrete checks, which is what the kernel doc actually says:

### Quiz

**Q:** The verifier is tracking $r1$ as "possible values in $[0, 1000]$." The program then does: if $r1 > 500$, load $r1$ bytes from a buffer of size 100. What does the verifier conclude, and why?

The learner answered correctly.

### Teacher

The step landed — you're reasoning about abstract ranges, not parroting. Next step: the three concrete checks the verifier performs.

## Step 3 — The three checks the verifier performs

From the one design decision — *prove all executions safe, before running, without knowing inputs* — everything the verifier checks falls out. The verifier has exactly three jobs, and each one is forced by a way the kernel could crash:

**1. The program must terminate.** The verifier can't decide this in the general case — the halting problem is undecidable, exactly the exponential wall you mentioned. So it doesn't try. Instead it imposes a **budget**: every path through the program is at most N instructions (historically 4096, raised to 1M on modern kernels). Loops aren't forbidden — they're *unfolded* within the budget. So the check is mechanical: walk the control-flow graph, every path stays under the limit, done. Termination is guaranteed not because the analysis is clever, but because the *class of allowed programs is restricted* so the question becomes decidable.

**2. Every memory access must be bounds-checked.** This is the range-tracking you just reasoned through. The verifier walks each path carrying the abstract state, and before any load or store it proves the register holding the pointer is within bounds for that access. What makes it *practical* is that the verifier doesn't treat all memory as one blob — it knows the **types** of the things it's touching: the packet buffer (`ctx`), a map value, the stack. Each type carries its own bounds, and the state tracks what's provable about each register on *this* path. Crucially, this is what makes the *programmer's* job well-defined: a check like `if (r1 > 1000) return;` before a load is not a courtesy the kernel happens to respect — it's the mechanism by which the programmer *hands* the verifier the proof.

**3. Values must be what they're used for.** A pointer's offset must be treated consistently: a register holding a packet pointer can't be incremented by 500 and then used as a map key. This is abstract typing — each register carries a type alongside its range, and every instruction's operands must be type-compatible. It exists because type confusion is the classic kernel exploitation primitive; the verifier refuses to let a program *create* one.

> [!IMPORTANT]
> **The verifier's three checks — termination (budget), memory safety (range tracking), type safety (register typing) — are all one mechanism applied three ways: abstract state over the control-flow graph.**

Check:

### Quiz

**Q:** A program passes the verifier's memory-bounds check, but it passes a register holding a packet pointer directly to a helper function that expects a map value. Which of the verifier's three checks does this fail?

The learner answered correctly.

### Quiz

**Q:** Once a program has passed the verifier, the kernel hands the bytecode to the JIT compiler before attaching it to a hook point. What is the JIT step doing, and why is it safe to do it at that point?

The learner answered correctly.

### Teacher

## Step 4 — From proven bytecode to running code

You predicted (a) and that's correct. Now resolve it, because the "why safe at that point" is the load-bearing half:

The verifier's proof is a proof **about the bytecode**. The JIT's job is one translation: the eBPF instruction set is a small fixed set of operations — register arithmetic, loads, stores, calls — so JIT compiles each instruction to a small block of native x86/ARM instructions with a one-to-one correspondence. The bytecode already carries all the guarantees (bounds, types, termination), and the JIT only *preserves* them — it never invents new behavior. That's why no re-verification of the machine code is needed: the machine code is a faithful, mechanical restatement of something already proven.

This is exactly the discipline you know from compiler work: correctness is proven on the IR (intermediate representation), and lowering to native code is a semantics-preserving transformation. The verifier is the safety analysis pass; the JIT is the backend. The eBPF instruction set was designed small *for this reason* — few operations means each maps to native instructions predictably, and the translation can be trusted to preserve properties.

**Performance follows for free.** The verifier's proof is paid once at load time. At runtime there is no interpreter, no per-instruction checking, no sandbox — the program executes as native machine instructions at effectively kernel speed. That's the deal the whole design makes:

> [!IMPORTANT]
> **Verification is expensive once, at load time; execution is fast, forever.** All the safety is proven upfront so the runtime pays zero safety overhead.

This is what separates eBPF from every previous attempt at the same problem — bpf (the classic packet filter) was interpreted, kernel modules are unsafe, and nothing else lets untrusted code run at kernel speed with proof of safety.

And then the last step of the lifecycle: **attach**. The compiled native code is registered at a hook point, and the kernel invokes it whenever the hooked event fires. Which brings us to the strand the crash interrupted — what a hook point *is*, and why the same program means different things at different hooks. That's where the skb you already hold becomes the anchor.

Next step — the hook determines what the program sees:

### Quiz

**Q:** What does "attaching a program to a hook point" determine about how the program behaves?

The learner answered correctly.

### Teacher

The step landed. Now the hook strand — and I'll have a figure built showing exactly where each hook sits in the packet's path through the kernel while we work through them.

### Tool

*visual result delivered*

### Teacher

## Step 5 — XDP: the earliest hook

The packet's journey through the kernel is a story about costs, and XDP sits at the cheapest possible point. Here's the path, which you partly reconstructed yourself from your TCP-from-scratch project:

1. The NIC receives the frame and DMAs it into memory — raw bytes.
2. The driver notices a new frame and hands it to the stack. Here, the kernel **allocates an skb** — the buffer-and-metadata container you identified earlier — and begins the slow work: parsing headers, tracking flow state, reserving the socket, copying where needed.
3. From there the packet climbs the stack, each layer adding or stripping metadata, until it reaches the socket.

An XDP program attaches at step 2 — **in the driver, before the skb exists**. Its context is the raw frame: a pointer to the data, its length, the device. It cannot see flow state, sockets, or higher-layer context — it sees bytes and little else.

Why would anyone want to see *less*? Because of the economics of the point where it sits:

- At that point the kernel has spent almost **nothing** on the packet yet — no skb allocation, no parsing, no socket lookup.
- So a decision made there is the **cheapest decision the kernel can make** about a packet.

That's exactly what you want for the first-line defense in a security system. A DDoS flood's packets are garbage — but under attack, millions of them arrive per second, and if each one costs an skb allocation plus stack processing before you can decide to throw it away, the attack succeeds by *cost*, not by cleverness. An XDP program can look at the raw frame, decide "drop" in a few instructions, and the kernel never pays the skb cost at all. Line-rate filtering.

The return value is the program's whole verdict — the context is minimal, so the vocabulary of actions is minimal:

- `XDP_PASS` — let it continue up the stack (an skb gets built, normal processing).
- `XDP_DROP` — discard it here, cheapest possible outcome.
- `XDP_TX` — send it back out the same NIC (you've modified the frame in place; useful for crafting responses at line rate).
- `XDP_REDIRECT` — send it to another interface or CPU.

> [!IMPORTANT]
> **XDP trades context for position.** It sees less than any other hook because it runs earlier than any other hook — and that's precisely why its verdict is the cheapest one the kernel can act on.

The figure I'm having built will show this path with each hook's position marked — I'll point at it when it arrives. Check that the XDP step landed:

### Quiz

**Q:** What can an XDP program see about a packet, given that it runs in the driver before the skb exists?

The learner answered correctly.

### Teacher

Stopping point noted — we'll resume right here when you're back. The figure arrived, so I'll leave it with you as the picture to hold for next time.

## The map of hooks

The figure shows the packet's journey left to right — **NIC → driver → XDP → skb allocation → tc → IP → TCP/UDP → socket** — with the two packet-path hooks marked where they sit:

- **XDP, in red**, hangs before the skb allocation node. That position *is* its identity: it sees the raw frame, and its drop is the cheapest decision the kernel can make.
- **tc, in blue**, sits after the skb exists — same packet, later point, more context, more cost.
- The **purple row below the axis** is the other kind of hook: kprobes/tracepoints and BPF LSM. They're not on the packet path at all — they attach to kernel events and security decisions, which is the next strand after tc.

Next session picks up at **tc** — the middle hook — then the event hooks, maps, and the security architectures. All of it hangs off the two things you now hold: the event-handler truth and the "context traded for position" logic that this figure makes visible.

Good stopping point — the next node starts clean.

### Learner

lets continue. I want you too tell me what I need to know further to make this first assignment on eBPF

### Teacher

Picking up where we left off — tc, the middle hook. Everything from here maps directly onto your Assignment #1 reading plan, so I'll flag which report section each strand feeds as we go.

## Step 6 — tc: the same packet, one decision later

Look at the figure again: XDP in red sits *before* the skb allocation node, tc in blue sits *after* it. That single arrow between them — the skb getting built — is the entire difference between the two hooks.

By the time a packet reaches tc, the kernel has paid the costs XDP avoids: the skb exists, headers are parsed, routing state is consulted. tc's context reflects that. An XDP program sees a raw frame; a tc program sees a **structured skb** — parsed header pointers, netdev, qdisc info, marks, and access to helper functions that understand protocols.

That extra context buys decisions XDP can't make:

- **Classification per flow, not per packet.** tc sees the packet in the context of the stack's own bookkeeping — it can distinguish traffic classes, apply rate limiting, shape or police.
- **Both directions.** tc hooks exist on both ingress and egress; XDP is receive-side only. Egress filtering — deciding what *your machine sends* — is a tc job.
- ** richer actions** through the traffic-control machinery the hook is embedded in.

The price is the one XDP was designed to avoid: a tc drop happens *after* the kernel already paid skb allocation and early processing. So the economics mirror the figure: XDP is the cheap first line against volume attacks; tc is the better-informed middle line for policy that needs stack context.

> [!IMPORTANT]
> **Each hook's power is determined by its position:** earlier = cheaper but blinder, later = more expensive but better informed. Every security system you'll read (Tetragon, BPFContain, He et al.) is an answer to "which position does this defense need?"

Check that the comparison landed — this is the table your report's hook-comparison section is built on:

### Quiz

**Q:** You're designing a defense for a server under a volumetric DDoS attack: millions of junk packets per second, nearly all of which can be identified as garbage from their raw headers alone. Which hook do you attach the filter to, and why?

The learner answered incorrectly.

### Teacher

That's a miss, and it's an important one — your reasoning contains a hidden assumption worth naming: you treated "raw packet" as if it lacked the headers, and "tc sees parsed headers" as if parsing *adds information*.

**The headers are in the frame.** An Ethernet frame's first bytes *are* the Ethernet header; the next 20 bytes *are* the IP header; then TCP. The raw frame the XDP program sees is the complete packet — nothing has been removed. Parsing doesn't create information; it's just the kernel having *already done* the pointer arithmetic to locate each header and storing the offsets in the skb's metadata. An XDP program can do that same arithmetic itself — read the bytes, find the IP header at a fixed offset, check ports and flags. The XDP paper's DoS filter does exactly this: it parses IP and TCP headers from the raw frame and drops floods based on them.

So the real difference between the hooks is **who does the parsing and what else exists**:

| | XDP (pre-skb) | tc (post-skb) |
|---|---|---|
| Packet bytes | Full frame present | Full packet present |
| Header parsing | Program does it itself | Already done, offsets available |
| Stack context (flows, sockets, marks) | None — nothing exists yet | Available |
| Cost of a drop | Nearly zero | skb allocation already paid |

The choice in the DDoS question comes down to economics: identification needs only header bytes, which both hooks can access — so the deciding factor is which drop is cheaper. Under a volumetric attack, the attack's weapon is cost per packet, so you drop at the cheapest point that can do the identification: XDP.

The distinction to carry forward: **"can't see it" vs. "must compute it yourself."** XDP can see everything in the frame; it just can't see kernel state that doesn't exist yet — flow tables, sockets, routing decisions. tc trades the kernel doing some work for you in exchange for that state.

Re-check with the assumption flipped:

### Quiz

**Q:** A different defense: your server runs many services, and you must rate-limit traffic per established TCP connection — you need to know which flow a packet belongs to, using the stack's connection state. Raw-header parsing alone can't tell you whether a flow is established or new. Which hook fits, and why?

The learner answered correctly.

### Teacher

Correct, though at 50% — the rule is fresh. Say it once more to lock it: **the hook is chosen by where the information you need comes into existence.** Bytes-only decisions go as early as possible; state-dependent decisions wait until the state exists.

## Step 7 — Off the packet path: tracepoints and kprobes

Now the purple row in the figure — the hooks that aren't about packets at all.

The packet-path hooks answered "where in the packet's journey do I intervene?" But security work often needs to observe the *kernel's own behavior*: which processes are running, what files they open, which syscalls they make. No position in the network stack will ever show you that. So eBPF attaches somewhere else entirely — not to a point in a data path, but to **kernel execution itself**:

- **Tracepoints** are fixed, stable observation points the kernel developers placed deliberately at meaningful events — `sys_enter_open`, `sched_process_exec`, `net_dev_xmit`. The kernel guarantees their arguments stay stable across versions. Predictable, but only where someone decided to put one.
- **kprobes** are the opposite: attach to *almost any kernel instruction* — a function entry, a return, a specific address. The program receives the actual arguments of the probed function. Total reach, but unstable: the function you probed can disappear or change signature in the next kernel release.

The trade is the same shape as every other eBPF trade: **stability vs. reach**. Tracepoints are guaranteed-stable but limited to pre-chosen events; kprobes see everything but break on kernel updates.

What the program sees is neither a frame nor an skb — it's the **arguments of a kernel function** at the moment it runs. A kprobe on `do_sys_open` sees the filename pointer and flags; that's how you build "which process opened which file" without modifying the kernel.

And this is your first direct bridge into the He et al. paper (the threat-model core of your report): its cross-container attacks are *observed and understood* through exactly these hooks — a tracing program attached at kernel event points is what makes the attack's behavior visible. The observation layer of the whole paper is this strand.

Check:

### Quiz

**Q:** You need to build a monitor that logs every time any process executes a program (exec). Which attachment strategy fits, and what does the program see?

The learner answered correctly.

### Teacher

Correct, and your eliminations are now reasoned rather than pattern-matched — "a random scheduler function wouldn't correspond to exec" is exactly the right test. One strand left in the hooks row: the security-native hook.

## Step 8 — BPF LSM: hooks on security decisions

Every other hook we've seen answers "where in the *system's work* do I observe or filter?" The Linux Security Modules framework answers a different question: **"where does the kernel decide whether to *allow* something?"**

The kernel has a layer of permission checks it calls before sensitive actions — opening a file, binding a socket, mounting a filesystem, loading a kernel module. These checks aren't in one place; they're spread throughout the kernel at every point where an operation crosses a policy boundary. BPF LSM lets an eBPF program attach to those checks. Unlike every hook before it, this program's return value isn't about packets or observation — **it's a verdict: allow or deny.** It's the first hook whose output *changes* what the kernel does, not just what it sees.

That difference in kind is why it's the hook the enforcement systems in your reading list build on:

- **BPFContain and bpfbox** (your research case studies) use BPF LSM as their enforcement mechanism: a container's policy is a set of LSM programs that deny any action the policy doesn't allow.
- **Tetragon** (your production case study) combines the two kinds: tracepoints/kprobes for *observation*, LSM for *enforcement*.

Now the three non-packet strands form a complete pattern you should see as one design space:

> [!IMPORTANT]
> **Every hook is one of two kinds: observation or enforcement.** XDP/tc are packet-path; tracepoints/kprobes observe kernel events; BPF LSM *decides*. A security system's architecture is mostly the choice of which hooks at which positions — early drops for volume, stack hooks for flow policy, event hooks for visibility, LSM for enforcement.

Check — this one tests the kind-distinction, not the taxonomy:

### Quiz

**Q:** BPFContain's goal is to confine a container: any file it opens, socket it binds, or process it forks that the policy doesn't explicitly allow must be *blocked* — not logged, blocked. Which hook type makes blocking possible, and what property of that hook type is essential here?

The learner answered correctly.

### Teacher

Correct — and your framing was the right instinct: "XDP and tc look at packets, tracepoints observe events, blocking needs LSM." The hooks strand is done; you now hold the full position map.

Next: maps — the last piece of machinery before the security systems themselves.

## Step 9 — Maps: the only memory a program has

Here's a fact that forces maps into existence, from the event-handler truth we started with:

An eBPF program is *invoked, runs, returns*. Nothing persists between two invocations — no local state survives, no globals, no heap. Yet nearly every security task needs memory across events. A DDoS filter needs to remember "this source IP has sent 10,000 packets in the last second." A container policy needs to remember which cgroups are allowed to bind sockets. Tetragon needs to accumulate the history of a process's behavior.

If programs are stateless but tasks need state, the state must live **outside the program** — in kernel memory, in a structure the kernel manages and both the program and user space can reach. That structure is a **map**: a typed key→value store the kernel allocates, that programs read and write via helper calls, and that user space can also read and write.

The map type encodes the access pattern, and the type matters because each is a different data structure with different costs:

- **Hash map** — key→value lookup: "count packets per source IP." Random access, natural fit for per-flow state.
- **Array** — index→value: fast, fixed size, good for per-CPU counters.
- **LPM trie** — longest-prefix match: "is this IP inside the 10.0.0.0/8 range?" The data structure behind CIDR-based policy.
- **Per-CPU variants** — each CPU gets its own copy, so increments don't contend. This is why eBPF filters scale at line rate: the hot-path program touches only its own CPU's slot, and no lock is ever taken.

And one more consequence worth seeing: maps are how the *two sides* of the system talk. The program is in kernel space; your report, your monitor, your control plane is in user space. The map is the shared channel — the program writes observations into it, the user-space reader consumes them. A security system is not just programs; it's programs + maps + a user-space component reading the maps.

> [!IMPORTANT]
> **Programs are stateless; state lives in maps.** Persistence, cross-program sharing, and the kernel↔user-space channel are all the same object.

Check:

### Quiz

**Q:** Your DDoS filter (an XDP program) must drop any source IP that has sent more than 10,000 packets in the last minute. The program itself is stateless — where does the per-IP packet count live, and which map type fits the lookup "given this source IP, get its count"?

The learner answered correctly.

### Teacher

Correct — machinery is complete. Every piece of eBPF now exists in your model: programs, verifier, JIT, hooks, maps. Now the payoff step, where the pieces become systems.

## Step 10 — Security systems are architectures, not programs

Look at what we've built and notice a pattern that recurs in everything you'll read for this assignment:

- The DDoS filter was never "a program" — it was an **XDP program + a hash map + a user-space process** that reads the map and ages out entries.
- Tetragon (production case study) is **kprobes/tracepoints for observation + BPF LSM for enforcement + maps holding the policies + a user-space control plane** loading policies and reading events.
- BPFContain (research case study) is **BPF LSM programs per container + maps holding the policy rules + a user-space manager** that loads the policy when a container starts.

No security system in your reading list is a single program. Every one is an *architecture* of eBPF programs at chosen hook positions, maps holding state and policy, and a user-space component doing the slow, non-hot-path work. This isn't a coincidence — it follows from the machinery itself:

1. The event-handler truth forces the *hot path* to be fast and small — so anything slow (parsing a config, logging, policy decisions that change rarely) is pushed to user space.
2. Statelessness forces state into maps — so the map layout *is* the system's data model.
3. The hook taxonomy forces a choice per concern — observe with probes, filter with XDP/tc, enforce with LSM. The architecture diagram of any system is mostly an answer to "which hook, for which concern?"

> [!IMPORTANT]
> **The eBPF design space = {hook position} × {program} × {map layout} × {user-space component}.** Every system you read is a point in this space, and its design doc is arguing why its point is right.

This is the generative structure I wanted you to walk away with: when you open BPFContain or the He et al. paper, you're no longer learning new machinery — you're seeing instances of a space you already hold. Your report's sections write themselves as comparisons along these dimensions.

Check that the synthesis holds, then we move into the specific systems:

### Quiz

**Q:** BPFContain loads a policy when a container starts, eBPF LSM programs enforce it on every sensitive operation, and maps hold the rules. A container's policy changes rarely, but enforcement happens on every operation. Why does the architecture put the policy in maps rather than hardcoding it into the LSM programs?

The learner answered correctly.

### Teacher

Correct at 90%, and your question in the thinking field is exactly the right one to ask before reading BPFContain — so let's answer it as the next step.

## Step 11 — Who is the user-space component? (And why the answer is the point of the whole design)

Your question: is the user-space component the *targeted program* being protected, or a separate component that sits alongside — reading config, logging, changing policy?

**It's separate — and the separation is the security property.** The architecture has three kinds of actors:

```
┌─────────────────────────────────────────────┐
│ kernel space                                │
│   eBPF programs (LSM/kprobe/XDP) + maps     │  ← enforcement & observation
├─────────────────────────────────────────────┤
│ user space                                  │
│   control-plane agent (separate daemon)     │  ← loads policy, reads events
│   ─────────────────────────────────────     │
│   the monitored workload (may be malicious) │  ← the thing being confined
└─────────────────────────────────────────────┘
```

The monitored workload is *untrusted by assumption* — in BPFContain's threat model, the container may already be compromised. The control-plane agent is trusted infrastructure owned by the system operator, like a container runtime.

Now the property that makes the design secure rather than decorative: **the enforcement lives below the adversary's reach.** If the confinement logic ran *inside* the monitored program's user-space process — a library it loads, a wrapper it spawns — then the attacker who compromises the container compromises the confinement: disable the library, and you're free. But BPFContain's enforcement runs in the kernel. The compromised container can execute arbitrary user-space code, and it still cannot bypass the LSM verdict — the deny happens in kernel space, at the permission check, on a path the attacker's code can never reach.

And the two user-space layers must not be confused, which is precisely the attack surface the papers study: the *agent* is in user space too — if an attacker gets root, they can unload the agent, alter its policies, or (per He et al.'s threat model) exploit the fact that confinement state lives in maps the agent must manage. The kernel programs are the hard wall; the agent is the softer layer above them.

> [!IMPORTANT]
> **Kernel enforcement means the verified, trusted logic sits below the compromised workload's privilege — the target cannot disable its own cage.** The user-space agent is trusted infrastructure, categorically separate from the monitored workload.

This is the conceptual core of your BPFContain case study section — check it, then we read the system itself as an instance:

### Quiz

**Q:** An attacker gains full code execution inside a BPFContain-confined container — arbitrary user-space code, as the container's user. Which of the following can the attacker NOT do, and why?

The learner answered correctly.

### Teacher

Correct — and your reasoning about the control-plane agent being attackable if the attacker gets root is exactly the bridge into the final two strands.

## Step 12 — He et al.: the threat model (your report's core reading)

Everything so far assumed the design works. He et al. 2023 (cross-container attacks, USENIX) is the paper that asks: **where does the confinement actually break?** — and their answer comes from inside the architecture you now hold.

Their setting: containers on a shared kernel, one of them malicious. BPFContain-style confinement is *supposed* to stop escape. The paper's contribution is a systematic hunt for the seams — and every seam they find is a consequence of the machinery you already know, not new machinery:

- **The confinement state lives in maps, and the policy is data.** You just established that "generic program + data-driven policy" is the pattern. He et al. attack the data path: if confinement decisions depend on maps that can be influenced — through the kernel interfaces that update them — the generic program faithfully enforces a policy the attacker helped write.
- **The hooks observe kernel events, and identity is inferred from them.** The LSM program must answer "is this operation allowed *for this container*?" — which requires mapping a kernel event (a file open) to a container identity (a cgroup, a namespace). That mapping is itself kernel state, built from observation. He et al. show attacks that confuse that mapping — making the enforcement program mis-attribute an action, so the check passes for the wrong identity.
- **The user-space agent is soft.** As your last answer reasoned — root in the host means access to the agent, its policies, and the map-loading path.

The reading plan has you at §1, §2.3, §3 (threat model), §4.1, skim §4.2–4.4, §5.3–5.4, §6. Read the threat model section *through the architecture lens*: for each attack, ask which box in the architecture it targets — the program, the map, the identity mapping, or the agent. That question is the difference between summarizing the paper and understanding it, and it's what your report's threat-model section needs.

The last strand is the critique of the machinery itself:

### Quiz

**Q:** You've now seen the verifier as the foundation of the whole design: static proof over all paths, paid once at load time, making untrusted code safe. Jia et al. (HotOS 2023) is titled "Verification is Untenable" — arguing the verifier itself is a problem. Based on the machinery you hold, what do you think their core argument is about *why* the verifier can't scale?

The learner answered correctly.

### Teacher

Correct prediction, derived from the machinery rather than recited — that's the arc closing properly. Resolve it and finish.

## Step 13 — Jia et al.: the verifier is the price

Your prediction named all three parts of their argument. Now connect each one to the machinery you hold:

**1. The verifier is trusted code that nothing verifies.** You established: the kernel can't tolerate a misbehaving program once. But the *verifier itself* is a kernel component — a large, complex C program doing subtle abstract interpretation. If the verifier has a bug — a path it mis-analyzes, a range it computes wrong — then an unsafe program can pass verification. The safety proof is only as good as the prover. Jia et al.'s point: **the verifier is part of the Trusted Computing Base** — the code you must trust even though eBPF's whole pitch was "don't trust code." Every kernel exploit in the verifier is a hole in the sandbox, and the verifier has had real CVEs.

**2. Path explosion and precision loss.** The abstract walk you reasoned through in step 2 covers every path. Programs grow — more branches, more loops — and paths multiply exponentially. And at every merge point, where two paths rejoin, the verifier must over-approximate: the merged state is the *union* of both paths' states, which is less precise than either. Less precision → more false rejections ("this safe program gets rejected") or more complexity to recover precision. Your "exponential" worry from step 2 was exactly right — it's not a footnote, it's the central scaling problem.

**3. No proof that the analysis is sound.** The Linux verifier is written by hand, informally. Formal alternatives exist — PREVAIL (in your reading plan as the citation to attempt via VPN) — which model the verifier mathematically so its soundness can be checked. The existence of alternatives that formalize what Linux does by hand is itself the argument: the current approach works in practice but isn't *proven*, which is ironic for a system whose entire value proposition is proof.

> [!IMPORTANT]
> **The design's central bargain — certainty before execution — is bought with a complex, hand-written, itself-unverified prover.** That prover is the TCB, and its cost grows with every feature eBPF gains.

This gives your report its critical-analysis section: eBPF is not a clean win, it's a trade — safety-by-proof bought with verifier complexity, and Jia et al. is the argument about who pays.

---

**The arc is complete.** You started at "bytecode → verify → JIT → attach" as a phrase, and you now hold the whole structure: the event-handler truth forcing the verifier; the verifier's proof forcing JIT; hooks chosen by where information comes into existence; statelessness forcing maps; programs+maps+agents forming architectures; and both the threat model (He et al.) and the critique (Jia et al.) as consequences of that architecture. Every paper in your reading plan is now an instance of a structure you hold — read them to fill in specifics, not to learn the machinery.

Suggested next steps for the assignment: I can quiz you through the specifics of any paper, work the report sections with you against the skeleton in `report/`, or stress-test the whole model with harder integrated questions. Where do you want to go?