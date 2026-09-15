# B0 — Common Baseline

This directory contains the common experimental baseline derived from the pinned reference implementation.

Only portability and compatibility changes required to build and run consistently across the target legacy Unix environments belong here. Arithmetic optimizations studied in the experiment must not be introduced into B0.

## Build model

`make` builds one static B0 library for each Kyber parameter set and then builds/runs pairwise, timing, NIST KAT, and deterministic upstream-vector checks against those libraries. After all required outputs have completed successfully, it writes `build/summary.txt`.

The generated layout is:

```text
build/
├── summary.txt
├── lib/
│   ├── libb0_kyber512.a
│   ├── libb0_kyber768.a
│   └── libb0_kyber1024.a
├── bin/
├── results/
├── kat/
├── vectors/
└── obj/
```

The parameter-specific libraries contain the KEM core and FIPS202 implementation but intentionally do not contain an entropy provider. Pairwise and timing tests link the normal OS-backed `randombytes.c`, while the KAT executables link the deterministic NIST KAT RNG implementation. This keeps all test classes on the same cryptographic library while allowing the randomness source to be selected by the test harness.

`build/summary.txt` records experiment provenance and results in one place, including hostname, `uname` system information, OS/kernel/machine/processor, detected word size, compiler command/path/version, build and linker flags, archiver tools, OpenSSL version, generated libraries and sizes, pairwise results, timing measurements, NIST KAT completion, and deterministic vector SHA-256 verification.

## Legacy C source policy

B0 source intended to be built on legacy compilers follows a declaration-first style: local variables are declared at the beginning of each function or block before executable statements. C99-style loop declarations such as `for (int i = 0; ...)` are avoided, and loop variables are declared once and reused instead of being redeclared in nested or later loops.

The B0-specific KAT sources under `tests/nistkat/` are adapted from the pinned upstream KAT harness only for compiler portability and use the same deterministic KAT procedure.

The deterministic vector check follows the pinned upstream Kyber `test/test_vectors.c` procedure and verifies its output against the SHA-256 values recorded by the same pinned upstream revision.

Useful targets:

```sh
make          # complete build/validation + build/summary.txt
make libs     # libraries only
make pairwise # pairwise tests and result files
make time     # timing tests and result files
make kat      # NIST KAT + upstream deterministic vector verification
make vectors  # deterministic vector verification only
make summary  # ensure all required outputs exist and write summary
make check    # pairwise + KAT
make clean
```

KAT generation requires a compatible OpenSSL `libcrypto` by default. Override `KAT_LDLIBS` if the target system uses a different linker option or crypto provider.
