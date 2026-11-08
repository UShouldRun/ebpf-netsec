CC          = gcc
CLANG       = clang
BPFTOOL     = bpftool
ARCH        = $(shell uname -m | sed 's/x86_64/x86/; s/aarch64/arm64/')

BUILDDIR    = ./build
LIBDIR      = ./lib
SRCDIR      = ./src
TARGET_NAME = fw
TARGET      = $(BUILDDIR)/$(TARGET_NAME)

MODULES     = maps xdp cgroup firewall controller
VENDORDIR   = $(LIBDIR)/vendor
INCLUDES    = -I$(BUILDDIR) $(foreach m,$(MODULES),-I$(LIBDIR)/$(m)/include) -I$(VENDORDIR)/include

# Dead-code elimination: one section per function/object, the linker drops the unreferenced ones
# (string8 is one translation unit; without this every string8_* would end up in the binary)
GCFLAGS     = -ffunction-sections -fdata-sections
GCLDFLAGS   = -Wl,--gc-sections

# -Wno-overlength-strings: the generated skeleton embeds the whole BPF object as one string
# -MMD -MP: emit build/*.d header dependencies, so editing a .h rebuilds what includes it
DEPFLAGS    = -MMD -MP

CFLAGS      = -Wall -Werror -Wpedantic -Wno-overlength-strings -O2 -g $(GCFLAGS) $(DEPFLAGS) $(INCLUDES)
BPF_CFLAGS  = -Wall -O2 -g -target bpf -D__TARGET_ARCH_$(ARCH) $(DEPFLAGS) $(INCLUDES)
LDFLAGS     = $(GCLDFLAGS)
LDLIBS      = -lbpf -lelf -lz

# Kernel side (*.bpf.c) and user side (*.c) from every module, plus src/
BPF_SRCS    = $(wildcard $(LIBDIR)/*/src/*.bpf.c)
BPF_OBJS    = $(patsubst %.bpf.c,$(BUILDDIR)/%.bpf.o,$(notdir $(BPF_SRCS)))
BPF_HDRS    = $(wildcard $(LIBDIR)/*/include/*.h)
SRCS        = $(filter-out %.bpf.c,$(wildcard $(LIBDIR)/*/src/*.c)) $(wildcard $(SRCDIR)/*.c)
OBJS        = $(patsubst %.c,$(BUILDDIR)/%.o,$(notdir $(SRCS)))

# Vendored libs (github.com/UShouldRun): string8 + alloc, vector and pair are header-only
VENDOR_SRCS = $(wildcard $(VENDORDIR)/src/*/*.c)
VENDOR_OBJS = $(patsubst %.c,$(BUILDDIR)/%.o,$(notdir $(VENDOR_SRCS)))

VPATH       = $(sort $(dir $(SRCS) $(BPF_SRCS) $(VENDOR_SRCS)))

VMLINUX     = $(BUILDDIR)/vmlinux.h
BPF_LINKED  = $(BUILDDIR)/$(TARGET_NAME).bpf.o
SKEL        = $(BUILDDIR)/$(TARGET_NAME).skel.h

report     ?= 2

.PHONY: all bpf clean zip

all: $(TARGET)

bpf: $(SKEL)

# 1. CO-RE types for the running kernel
$(VMLINUX): | $(BUILDDIR)
	$(BPFTOOL) btf dump file /sys/kernel/btf/vmlinux format c > $@

# 2. Kernel side: one object per .bpf.c
$(BUILDDIR)/%.bpf.o: %.bpf.c $(VMLINUX) $(BPF_HDRS) | $(BUILDDIR)
	$(CLANG) $(BPF_CFLAGS) -c $< -o $@

# 3. Link them: __weak maps merge, so both layers share one set of maps
$(BPF_LINKED): $(BPF_OBJS)
	$(BPFTOOL) gen object $@ $^

# 4. Typed C API for the controller: struct fw_bpf, fw_bpf__open/load/destroy
$(SKEL): $(BPF_LINKED)
	$(BPFTOOL) gen skeleton $< > $@

# 5. User side
$(BUILDDIR)/firewall.o: $(SKEL)

# each vendored .c includes its private header and "<lib>.h" from its own include dir
$(BUILDDIR)/alloc.o:   CFLAGS += -I$(VENDORDIR)/include/alloc
$(BUILDDIR)/string8.o: CFLAGS += -I$(VENDORDIR)/include/string8

$(BUILDDIR)/%.o: %.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) $(VENDOR_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BUILDDIR):
	mkdir -p $@

clean:
	rm -rf $(BUILDDIR)

-include $(wildcard $(BUILDDIR)/*.d)

zip:
	zip -r TP1_group7_presentation_$(report).zip . -x '$(BUILDDIR)/*' '.git/*'
