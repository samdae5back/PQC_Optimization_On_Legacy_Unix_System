CC = cc
AR = ar
RANLIB = ranlib

CPPFLAGS =
CFLAGS = -O2
LDFLAGS =
LDLIBS =
KAT_LDLIBS =

BUILD = build
OBJDIR = $(BUILD)/obj
LIBDIR = $(BUILD)/lib
BINDIR = $(BUILD)/bin
RESULTDIR = $(BUILD)/results
KATDIR = $(BUILD)/kat
VECTORDIR = $(BUILD)/vectors
SUMMARY = $(BUILD)/summary.txt
KATSRC = tests/nistkat
VECTORSRC = tests/vectors
SHA256CHECK = $(BINDIR)/sha256_file

OBJ512 = $(OBJDIR)/512/kem.o $(OBJDIR)/512/indcpa.o $(OBJDIR)/512/polyvec.o $(OBJDIR)/512/poly.o $(OBJDIR)/512/ntt.o $(OBJDIR)/512/cbd.o $(OBJDIR)/512/reduce.o $(OBJDIR)/512/verify.o $(OBJDIR)/512/fips202.o $(OBJDIR)/512/symmetric-shake.o
OBJ768 = $(OBJDIR)/768/kem.o $(OBJDIR)/768/indcpa.o $(OBJDIR)/768/polyvec.o $(OBJDIR)/768/poly.o $(OBJDIR)/768/ntt.o $(OBJDIR)/768/cbd.o $(OBJDIR)/768/reduce.o $(OBJDIR)/768/verify.o $(OBJDIR)/768/fips202.o $(OBJDIR)/768/symmetric-shake.o
OBJ1024 = $(OBJDIR)/1024/kem.o $(OBJDIR)/1024/indcpa.o $(OBJDIR)/1024/polyvec.o $(OBJDIR)/1024/poly.o $(OBJDIR)/1024/ntt.o $(OBJDIR)/1024/cbd.o $(OBJDIR)/1024/reduce.o $(OBJDIR)/1024/verify.o $(OBJDIR)/1024/fips202.o $(OBJDIR)/1024/symmetric-shake.o

LIB512 = $(LIBDIR)/libb0_kyber512.a
LIB768 = $(LIBDIR)/libb0_kyber768.a
LIB1024 = $(LIBDIR)/libb0_kyber1024.a

PAIRWISE_RESULTS = $(RESULTDIR)/pairwise512.txt $(RESULTDIR)/pairwise768.txt $(RESULTDIR)/pairwise1024.txt
TIME_RESULTS = $(RESULTDIR)/time512.txt $(RESULTDIR)/time768.txt $(RESULTDIR)/time1024.txt
KAT_RESULTS = $(KATDIR)/512/.done $(KATDIR)/768/.done $(KATDIR)/1024/.done
VECTOR_CHECKS = $(VECTORDIR)/tvecs512.sha256.ok $(VECTORDIR)/tvecs768.sha256.ok $(VECTORDIR)/tvecs1024.sha256.ok

all: summary

libs: $(LIB512) $(LIB768) $(LIB1024)

pairwise: $(PAIRWISE_RESULTS)

time: $(TIME_RESULTS)

kat: $(KAT_RESULTS) vectors

vectors: $(VECTOR_CHECKS)

summary: $(SUMMARY)

check: pairwise kat

$(OBJDIR)/512/kem.o $(OBJDIR)/768/kem.o $(OBJDIR)/1024/kem.o: kem.c
	@case "$@" in */512/*) k=2;; */768/*) k=3;; *) k=4;; esac; \
	mkdir -p `dirname "$@"`; \
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=$$k -c kem.c -o "$@"

$(OBJDIR)/512/indcpa.o $(OBJDIR)/768/indcpa.o $(OBJDIR)/1024/indcpa.o: indcpa.c
	@case "$@" in */512/*) k=2;; */768/*) k=3;; *) k=4;; esac; \
	mkdir -p `dirname "$@"`; \
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=$$k -c indcpa.c -o "$@"

$(OBJDIR)/512/polyvec.o $(OBJDIR)/768/polyvec.o $(OBJDIR)/1024/polyvec.o: polyvec.c
	@case "$@" in */512/*) k=2;; */768/*) k=3;; *) k=4;; esac; \
	mkdir -p `dirname "$@"`; \
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=$$k -c polyvec.c -o "$@"

$(OBJDIR)/512/poly.o $(OBJDIR)/768/poly.o $(OBJDIR)/1024/poly.o: poly.c
	@case "$@" in */512/*) k=2;; */768/*) k=3;; *) k=4;; esac; \
	mkdir -p `dirname "$@"`; \
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=$$k -c poly.c -o "$@"

$(OBJDIR)/512/ntt.o $(OBJDIR)/768/ntt.o $(OBJDIR)/1024/ntt.o: ntt.c
	@case "$@" in */512/*) k=2;; */768/*) k=3;; *) k=4;; esac; \
	mkdir -p `dirname "$@"`; \
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=$$k -c ntt.c -o "$@"

$(OBJDIR)/512/cbd.o $(OBJDIR)/768/cbd.o $(OBJDIR)/1024/cbd.o: cbd.c
	@case "$@" in */512/*) k=2;; */768/*) k=3;; *) k=4;; esac; \
	mkdir -p `dirname "$@"`; \
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=$$k -c cbd.c -o "$@"

$(OBJDIR)/512/reduce.o $(OBJDIR)/768/reduce.o $(OBJDIR)/1024/reduce.o: reduce.c
	@case "$@" in */512/*) k=2;; */768/*) k=3;; *) k=4;; esac; \
	mkdir -p `dirname "$@"`; \
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=$$k -c reduce.c -o "$@"

$(OBJDIR)/512/verify.o $(OBJDIR)/768/verify.o $(OBJDIR)/1024/verify.o: verify.c
	@case "$@" in */512/*) k=2;; */768/*) k=3;; *) k=4;; esac; \
	mkdir -p `dirname "$@"`; \
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=$$k -c verify.c -o "$@"

$(OBJDIR)/512/fips202.o $(OBJDIR)/768/fips202.o $(OBJDIR)/1024/fips202.o: fips202.c
	@case "$@" in */512/*) k=2;; */768/*) k=3;; *) k=4;; esac; \
	mkdir -p `dirname "$@"`; \
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=$$k -c fips202.c -o "$@"

$(OBJDIR)/512/symmetric-shake.o $(OBJDIR)/768/symmetric-shake.o $(OBJDIR)/1024/symmetric-shake.o: symmetric-shake.c
	@case "$@" in */512/*) k=2;; */768/*) k=3;; *) k=4;; esac; \
	mkdir -p `dirname "$@"`; \
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=$$k -c symmetric-shake.c -o "$@"

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

$(BINDIR)/kat512: $(KATSRC)/PQCgenKAT_kem.c $(KATSRC)/rng.c $(KATSRC)/rng.h $(KATSRC)/aes256.c $(KATSRC)/aes256.h $(LIB512)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -I$(KATSRC) -DKYBER_K=2 $(KATSRC)/PQCgenKAT_kem.c $(KATSRC)/rng.c $(KATSRC)/aes256.c $(LIB512) $(LDFLAGS) $(KAT_LDLIBS) -o $@

$(BINDIR)/kat768: $(KATSRC)/PQCgenKAT_kem.c $(KATSRC)/rng.c $(KATSRC)/rng.h $(KATSRC)/aes256.c $(KATSRC)/aes256.h $(LIB768)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -I$(KATSRC) -DKYBER_K=3 $(KATSRC)/PQCgenKAT_kem.c $(KATSRC)/rng.c $(KATSRC)/aes256.c $(LIB768) $(LDFLAGS) $(KAT_LDLIBS) -o $@

$(BINDIR)/kat1024: $(KATSRC)/PQCgenKAT_kem.c $(KATSRC)/rng.c $(KATSRC)/rng.h $(KATSRC)/aes256.c $(KATSRC)/aes256.h $(LIB1024)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -I$(KATSRC) -DKYBER_K=4 $(KATSRC)/PQCgenKAT_kem.c $(KATSRC)/rng.c $(KATSRC)/aes256.c $(LIB1024) $(LDFLAGS) $(KAT_LDLIBS) -o $@

$(BINDIR)/vectors512: $(VECTORSRC)/test_vectors.c $(LIB512)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=2 $(VECTORSRC)/test_vectors.c $(LIB512) $(LDFLAGS) $(LDLIBS) -o $@

$(BINDIR)/vectors768: $(VECTORSRC)/test_vectors.c $(LIB768)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=3 $(VECTORSRC)/test_vectors.c $(LIB768) $(LDFLAGS) $(LDLIBS) -o $@

$(BINDIR)/vectors1024: $(VECTORSRC)/test_vectors.c $(LIB1024)
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -DKYBER_K=4 $(VECTORSRC)/test_vectors.c $(LIB1024) $(LDFLAGS) $(LDLIBS) -o $@

$(SHA256CHECK): $(VECTORSRC)/sha256_file.c
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(VECTORSRC)/sha256_file.c $(LDFLAGS) -o $@

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

$(VECTORDIR)/tvecs512: $(BINDIR)/vectors512
	@mkdir -p $(VECTORDIR)
	./$(BINDIR)/vectors512 > $@

$(VECTORDIR)/tvecs768: $(BINDIR)/vectors768
	@mkdir -p $(VECTORDIR)
	./$(BINDIR)/vectors768 > $@

$(VECTORDIR)/tvecs1024: $(BINDIR)/vectors1024
	@mkdir -p $(VECTORDIR)
	./$(BINDIR)/vectors1024 > $@

$(VECTORDIR)/tvecs512.sha256.ok: $(VECTORDIR)/tvecs512 $(VECTORSRC)/SHA256SUMS $(SHA256CHECK)
	@actual=`$(SHA256CHECK) $(VECTORDIR)/tvecs512`; expected=`sed -n 's/[ ][ ]*tvecs512$$//p' $(VECTORSRC)/SHA256SUMS`; \
	if test "$$actual" != "$$expected"; then echo "FAIL tvecs512 SHA-256: $$actual != $$expected" >&2; exit 1; fi; \
	echo "PASS tvecs512 SHA-256 $$actual"; echo "$$actual" > $@

$(VECTORDIR)/tvecs768.sha256.ok: $(VECTORDIR)/tvecs768 $(VECTORSRC)/SHA256SUMS $(SHA256CHECK)
	@actual=`$(SHA256CHECK) $(VECTORDIR)/tvecs768`; expected=`sed -n 's/[ ][ ]*tvecs768$$//p' $(VECTORSRC)/SHA256SUMS`; \
	if test "$$actual" != "$$expected"; then echo "FAIL tvecs768 SHA-256: $$actual != $$expected" >&2; exit 1; fi; \
	echo "PASS tvecs768 SHA-256 $$actual"; echo "$$actual" > $@

$(VECTORDIR)/tvecs1024.sha256.ok: $(VECTORDIR)/tvecs1024 $(VECTORSRC)/SHA256SUMS $(SHA256CHECK)
	@actual=`$(SHA256CHECK) $(VECTORDIR)/tvecs1024`; expected=`sed -n 's/[ ][ ]*tvecs1024$$//p' $(VECTORSRC)/SHA256SUMS`; \
	if test "$$actual" != "$$expected"; then echo "FAIL tvecs1024 SHA-256: $$actual != $$expected" >&2; exit 1; fi; \
	echo "PASS tvecs1024 SHA-256 $$actual"; echo "$$actual" > $@

$(SUMMARY): $(LIB512) $(LIB768) $(LIB1024) $(PAIRWISE_RESULTS) $(TIME_RESULTS) $(KAT_RESULTS) $(VECTOR_CHECKS)
	@mkdir -p $(BUILD)
	@{ \
		echo "B0 Build and Validation Summary"; \
		echo "==============================="; \
		echo; \
		echo "[System]"; \
		echo "generated_at=`date 2>/dev/null || echo unknown`"; \
		echo "hostname=`hostname 2>/dev/null || uname -n 2>/dev/null || echo unknown`"; \
		echo "uname=`uname -a 2>/dev/null || echo unknown`"; \
		echo "os=`uname -s 2>/dev/null || echo unknown`"; \
		echo "kernel_release=`uname -r 2>/dev/null || echo unknown`"; \
		echo "machine=`uname -m 2>/dev/null || echo unknown`"; \
		echo "processor=`uname -p 2>/dev/null || echo unknown`"; \
		echo "word_bits=`getconf LONG_BIT 2>/dev/null || echo unknown`"; \
		echo; \
		echo "[Toolchain]"; \
		echo "CC=$(CC)"; \
		ccpath=`whence $(CC) 2>/dev/null`; if test -z "$$ccpath"; then ccpath=`which $(CC) 2>/dev/null`; fi; if test -z "$$ccpath"; then ccpath=unknown; fi; echo "compiler_path=$$ccpath"; \
		compiler_version=`$(CC) --version 2>/dev/null | sed -n '1p'`; if test -z "$$compiler_version"; then compiler_version=`$(CC) -V 2>&1 | sed -n '1p'`; fi; if test -z "$$compiler_version"; then compiler_version=`$(CC) -qversion 2>&1 | sed -n '1p'`; fi; echo "compiler_version=$$compiler_version"; \
		echo "CPPFLAGS=$(CPPFLAGS)"; \
		echo "CFLAGS=$(CFLAGS)"; \
		echo "LDFLAGS=$(LDFLAGS)"; \
		echo "LDLIBS=$(LDLIBS)"; \
		echo "KAT_LDLIBS=$(KAT_LDLIBS)"; \
		echo "AR=$(AR)"; \
		echo "RANLIB=$(RANLIB)"; \
		echo "kat_aes=internal portable AES-256"; \
		echo "sha256_checker=internal portable C"; \
		echo; \
		echo "[Libraries]"; \
		for f in $(LIB512) $(LIB768) $(LIB1024); do if test -f $$f; then echo "PASS $$f size=`wc -c < $$f | tr -d ' '` bytes"; else echo "FAIL $$f missing"; fi; done; \
		echo; \
		echo "[Pairwise]"; \
		cat $(PAIRWISE_RESULTS); \
		echo; \
		echo "[Timing]"; \
		for f in $(TIME_RESULTS); do echo "--- $$f ---"; cat $$f; done; \
		echo; \
		echo "[NIST KAT]"; \
		for p in 512 768 1024; do if test -f $(KATDIR)/$$p/.done; then echo "PASS kyber$$p KAT generated"; else echo "FAIL kyber$$p KAT missing"; fi; done; \
		echo; \
		echo "[Upstream deterministic vectors]"; \
		for p in 512 768 1024; do if test -f $(VECTORDIR)/tvecs$$p.sha256.ok; then echo "PASS tvecs$$p SHA-256 `cat $(VECTORDIR)/tvecs$$p.sha256.ok`"; else echo "FAIL tvecs$$p SHA-256 missing"; fi; done; \
	} > $(SUMMARY)
	@echo "Wrote $(SUMMARY)"

clean:
	rm -rf $(BUILD)
