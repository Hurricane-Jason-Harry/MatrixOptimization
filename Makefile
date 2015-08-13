SHELL := /bin/bash

EMULATOR_DIR := /home/main/Desktop/rocket-chip/emulator
EMULATOR_BIN := emulator-Top-HwachaCPPConfig # need to switch to emulator_dir before running bin
#EMULATOR_ARG1 := +dramsim +verbose +max-cycles=50000000 $$RISCV/riscv64-unknown-elf/bin/pk # first argument following tested binary
#EMULATOR_ARG2 := none 3>&1 1>&2 2>&3 | spike-dasm 1>/home/main/Desktop/ConvolutionOptimization/test.txt# second argument
BENCHMARKDIR := benchmark
EMULATOR_ARG1 := +max-cycles=20000000 $$RISCV/riscv64-unknown-elf/bin/pk # first argument following tested binary
EMULATOR_ARG2 := 

RUNDIR := run
TESTDIR := test
TESTFILE := $(TESTDIR)/test.out
ARCHS := x86 riscv
OPTIMIZATIONS := naive omp simd cb lu rb omp_simd omp_simd_cb omp_simd_cb_lu omp_simd_cb_lu_rb cb_profiling
FILES := $(addprefix src/, main.c optimizations.c utils.c config.h mm_malloc.h \
				optimizations.h unroll.h utils.h optimizations_x86.c optimizations_riscv.c mm_malloc.h)
ARCH_OPTIMIZATIONS := $(foreach ARCH, $(ARCHS), $(addprefix $(ARCH)/, $(OPTIMIZATIONS)))

define init
	$(eval ARCH := $(strip $(subst /, , $(subst $(RUNDIR)/, , $(dir $@)))))
	$(eval OPTIMIZATION := $(strip $(notdir $@)))
	$(call $(ARCH))
	$(eval OPTIMIZATION := $(shell echo $(OPTIMIZATION) | tr a-z A-Z))
endef

define x86
	$(eval CXX := gcc)
	$(eval CXX_FLAGS := -Wall -march=native -mavx2 -mfma -O3 -std=c99 -Wno-format -fopenmp -lm -lpthread)
	$(eval CXX_FLAGS += $(EXTRA_FLAGS))
	$(eval EXE_PREFIX:=)
endef

define riscv
	$(eval CXX := riscv64-unknown-elf-gcc)
	$(eval CXX_FLAGS := -Wall -O3 -march=RV64IMAFDXhwacha -Wno-format -mhwacha4 -std=c99 -lm)
	$(eval CXX_FLAGS += $(EXTRA_FLAGS))
	$(eval EXE_PREFIX := spike --l2=512:8:64 --isa=RV64IMAFDXhwacha pk)
endef

ifneq ($(wildcard Makefrag_profiling), )
include Makefrag_profiling
endif
	
$(TESTDIR) $(RUNDIR):
	mkdir -p $@
	
$(addprefix $(RUNDIR)/, $(ARCHS)): | $(RUNDIR)
	mkdir -p $@

$(TESTDIR)/make_ref_data: src/make_ref_data.c src/config.h | $(TESTDIR)
	gcc -Wall -std=c99 $^ -o $@ 

$(TESTDIR)/flush_cache: src/flush_cache.c | $(TESTDIR)
	gcc -Wall -std=c99 -O0 -fopenmp $^ -o $@ 
		
$(ARCH)_all: $(ARCH_OPTIMIZATIONS)
	
$(addprefix $(RUNDIR)/, $(ARCH_OPTIMIZATIONS)) : $(FILES) | $(addprefix $(RUNDIR)/, $(ARCHS))
	$(call init)
	$(CXX) $(CXX_FLAGS) $(FILES) -D MATMUL_$(OPTIMIZATION) -o $@


# Phony targets
make_ref_data flush_cache : %: $(TESTDIR)/%
	$<

$(TESTFILE): $(TESTDIR)/make_ref_data
	$(TESTDIR)/make_ref_data
	
$(ARCH_OPTIMIZATIONS) : % : $(RUNDIR)/% $(TESTFILE)
	$(call init)
	@echo Running optimization $(OPTIMIZATION)---
	$(EXE_PREFIX) $<

$(addsuffix .benchmark, $(ARCH_OPTIMIZATIONS)) : %.$(BENCHMARKDIR) : $(RUNDIR)/% $(TESTFILE)
	$(call init)
	mkdir -p $(BENCHMARKDIR)/$(ARCH)
	@echo Running benchmark
	@$(EXE_PREFIX) $<  > $(BENCHMARKDIR)/$@
	$(eval CURRENTDIR := $(shell pwd))
	$(eval condition := $(filter riscv, $(ARCH)))
	$(if $(condition), cd $(EMULATOR_DIR) && ./$(EMULATOR_BIN)  \
		$(EMULATOR_ARG1) $(CURRENTDIR)/$<  $(EMULATOR_ARG2))
	cd $(CURRENTDIR)
	
clean:
	rm -f -r $(TESTDIR) $(RUNDIR)
				 
.PHONY: flush_cache make_ref_data 
.PHONY: $(ARCH_OPTIMIZATIONS)
.PHONY: clean
.PHONY: $(addsuffix .benchmark, $(ARCH_OPTIMIZATIONS))  # do benchmark. Output to $(benchmark/$(ARCH_OPTIMIZATIONS).out