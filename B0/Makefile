CC ?= cc
AR ?= ar
RANLIB ?= ranlib

CPPFLAGS ?=
CFLAGS ?= -O2
LDFLAGS ?=
LDLIBS ?=
KAT_LDLIBS ?= -lcrypto

BUILD := build
OBJDIR := $(BUILD)/obj
LIBDIR := $(BUILD)/lib
BINDIR := $(BUILD)/bin
RESULTDIR := $(BUILD)/results
KATDIR := $(BUILD)/kat

CORE_SRC := kem.c indcpa.c polyvec.c poly.c ntt.c cbd.c reduce.c verify.c fips202.c symmetric-shake.c
OBJ512 := $(CORE_SRC:%.c=$(OBJDIR)/512/%.o)
OBJ768 := $(CORE_SRC:%.c=$(OBJDIR)/768/%.o)
OBJ1024 := $(CORE_SRC:%.c=$(OBJDIR)/1024/%.o)

LIB512 := $(LIBDIR)/libb0_kyber512.a
LIB768 := $(LIBDIR)/libb0_kyber768.a
LIB1024 := $(LIBDIR)/libb0_kyber1024.a

PAIRWISE_BINS := $(BINDIR)/pairwise512 $(BINDIR)/pairwise768 $(BINDIR)/pairwise1024
TIME_BINS := $(BINDIR)/time512 $(BINDIR)/time768 $(BINDIR)/time1024
KAT_BINS := $(BINDIR)/kat512 $(BINDIR)/kat768 $(BINDIR)/kat1024

PAIRWISE_RESULTS := $(RESULTDIR)/pairwise512.txt $(RESULTDIR)/pairwise768.txt $(RESULTDIR)/pairwise1024.txt
TIME_RESULTS := $(RESULTDIR)/time512.txt $(RESULTDIR)/time768.txt $(RESULTDIR)/time1024.txt
KAT_RESULTS := $(KATDIR)/512/.done $(KATDIR)/768/.done $(KATDIR)/1024/.done

.PHONY: all libs pairwise time kat check clean

all: libs pairwise time kat

libs: $(LIB512) $(LIB768) $(LIB1024)
pairwise: $(PAIRWISE_RESULTS)
time: $(TIME_RESULTS)
kat: $(KAT_RESULTS)
check: pairwise kat

$(OBJDIR)/512/%.o: %.c
	@mkdir -p $(OBJDIR)/512
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=2 -c $< -o $@

$(OBJDIR)/768/%.o: %.c
	@mkdir -p $(OBJDIR)/768
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=3 -c $< -o $@

$(OBJDIR)/1024/%.o: %.c
	@mkdir -p $(OBJDIR)/1024
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=4 -c $< -o $@

$(LIB512): $(OBJ512)
	@mkdir -p $(LIBDIR)
	$(AR) rcs $@ $(OBJ512)
	-$(RANLIB) $@

$(LIB768): $(OBJ768)
	@mkdir -p $(LIBDIR)
	$(AR) rcs $@ $(OBJ768)
	-$(RANLIB) $@

$(LIB1024): $(OBJ1024)
	@mkdir -p $(LIBDIR)
	$(AR) rcs $@ $(OBJ1024)
	-$(RANLIB) $@

$(BINDIR)/pairwise512: tests/pairwise.c randombytes.c $(LIB512)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=2 tests/pairwise.c randombytes.c $(LIB512) $(LDFLAGS) $(LDLIBS) -o $@

$(BINDIR)/pairwise768: tests/pairwise.c randombytes.c $(LIB768)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=3 tests/pairwise.c randombytes.c $(LIB768) $(LDFLAGS) $(LDLIBS) -o $@

$(BINDIR)/pairwise1024: tests/pairwise.c randombytes.c $(LIB1024)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=4 tests/pairwise.c randombytes.c $(LIB1024) $(LDFLAGS) $(LDLIBS) -o $@

$(BINDIR)/time512: tests/time_test.c randombytes.c $(LIB512)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=2 tests/time_test.c randombytes.c $(LIB512) $(LDFLAGS) $(LDLIBS) -o $@

$(BINDIR)/time768: tests/time_test.c randombytes.c $(LIB768)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=3 tests/time_test.c randombytes.c $(LIB768) $(LDFLAGS) $(LDLIBS) -o $@

$(BINDIR)/time1024: tests/time_test.c randombytes.c $(LIB1024)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=4 tests/time_test.c randombytes.c $(LIB1024) $(LDFLAGS) $(LDLIBS) -o $@

$(BINDIR)/kat512: nistkat/PQCgenKAT_kem.c nistkat/rng.c $(LIB512)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=2 nistkat/PQCgenKAT_kem.c nistkat/rng.c $(LIB512) $(LDFLAGS) $(KAT_LDLIBS) -o $@

$(BINDIR)/kat768: nistkat/PQCgenKAT_kem.c nistkat/rng.c $(LIB768)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=3 nistkat/PQCgenKAT_kem.c nistkat/rng.c $(LIB768) $(LDFLAGS) $(KAT_LDLIBS) -o $@

$(BINDIR)/kat1024: nistkat/PQCgenKAT_kem.c nistkat/rng.c $(LIB1024)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=4 nistkat/PQCgenKAT_kem.c nistkat/rng.c $(LIB1024) $(LDFLAGS) $(KAT_LDLIBS) -o $@

$(RESULTDIR)/pairwise512.txt: $(BINDIR)/pairwise512
	@mkdir -p $(RESULTDIR)
	./$(BINDIR)/pairwise512 > $@

$(RESULTDIR)/pairwise768.txt: $(BINDIR)/pairwise768
	@mkdir -p $(RESULTDIR)
	./$(BINDIR)/pairwise768 > $@

$(RESULTDIR)/pairwise1024.txt: $(BINDIR)/pairwise1024
	@mkdir -p $(RESULTDIR)
	./$(BINDIR)/pairwise1024 > $@

$(RESULTDIR)/time512.txt: $(BINDIR)/time512
	@mkdir -p $(RESULTDIR)
	./$(BINDIR)/time512 > $@

$(RESULTDIR)/time768.txt: $(BINDIR)/time768
	@mkdir -p $(RESULTDIR)
	./$(BINDIR)/time768 > $@

$(RESULTDIR)/time1024.txt: $(BINDIR)/time1024
	@mkdir -p $(RESULTDIR)
	./$(BINDIR)/time1024 > $@

$(KATDIR)/512/.done: $(BINDIR)/kat512
	@mkdir -p $(KATDIR)/512
	@(cd $(KATDIR)/512 && ../../bin/kat512)
	@touch $@

$(KATDIR)/768/.done: $(BINDIR)/kat768
	@mkdir -p $(KATDIR)/768
	@(cd $(KATDIR)/768 && ../../bin/kat768)
	@touch $@

$(KATDIR)/1024/.done: $(BINDIR)/kat1024
	@mkdir -p $(KATDIR)/1024
	@(cd $(KATDIR)/1024 && ../../bin/kat1024)
	@touch $@

clean:
	rm -rf $(BUILD)
