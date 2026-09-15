# B0 — Common Baseline

This directory contains the common experimental baseline derived from the pinned reference implementation.

Only portability and compatibility changes required to build and run consistently across the target legacy Unix environments belong here. Arithmetic optimizations studied in the experiment must not be introduced into B0.

## Build model

`make` builds one static B0 library for each Kyber parameter set and then builds/runs pairwise, timing, and KAT programs against those libraries.

The generated layout is:

```text
build/
├── lib/
│   ├── libb0_kyber512.a
│   ├── libb0_kyber768.a
│   └── libb0_kyber1024.a
├── bin/
│   ├── pairwise512
│   ├── pairwise768
│   ├── pairwise1024
│   ├── time512
│   ├── time768
│   ├── time1024
│   ├── kat512
│   ├── kat768
│   ├── kat1024
│   ├── vectors512
│   ├── vectors768
│   └── vectors1024
├── results/
│   ├── pairwise512.txt
│   ├── pairwise768.txt
│   ├── pairwise1024.txt
│   ├── time512.txt
│   ├── time768.txt
│   └── time1024.txt
├── kat/
│   ├── 512/
│   ├── 768/
│   └── 1024/
├── vectors/
│   ├── tvecs512
│   ├── tvecs768
│   ├── tvecs1024
│   └── *.sha256.ok
└── obj/
```

The parameter-specific libraries contain the KEM core and FIPS202 implementation but intentionally do not contain an entropy provider. Pairwise and timing tests link the normal OS-backed `randombytes.c`, while the NIST KAT executables link the deterministic NIST KAT RNG implementation. The upstream deterministic vector test provides its own SHAKE128-based `randombytes` implementation. This keeps all test classes on the same cryptographic library while allowing the randomness source to be selected by the test harness.

## Upstream vector verification

The pinned upstream Kyber repository does not store generated `PQCkemKAT_*.rsp` files as golden files. Instead, its `test/test_vectors.c` program emits deterministic vector streams and the repository root `SHA256SUMS` records the expected SHA-256 digest for each parameter set.

B0 reproduces that procedure under `tests/vectors/`. `make kat` generates `tvecs512`, `tvecs768`, and `tvecs1024` using the B0 libraries and verifies their SHA-256 digests against the values copied from the pinned upstream commit. A mismatch causes the build to fail.

The expected digests are:

```text
4d34994299a8f8dcb36c550951a00f6e16918d6d5b6f280ee2aa12a7bf8375a0  tvecs512
b59ac4d2b429b1f8c3b8a5542fb638179da2fd8b1212891d2f976e70e219fed1  tvecs768
3f577090c7cb7a345ce0417a2a2353153a9f1b8d79f8d927cb6a7b4ec17fd2a1  tvecs1024
```

## Legacy C source policy

B0 source intended to be built on legacy compilers follows a declaration-first style: local variables are declared at the beginning of each function or block before executable statements. C99-style loop declarations such as `for (int i = 0; ...)` are avoided, and loop variables are declared once and reused instead of being redeclared in nested or later loops.

The B0-specific KAT sources under `tests/nistkat/` are adapted from the pinned upstream KAT harness only for compiler portability and use the same deterministic KAT procedure.

Useful targets:

```sh
make          # libraries + pairwise + timing + KAT + upstream vector verification
make libs     # libraries only
make pairwise # pairwise tests and result files
make time     # timing tests and result files
make kat      # NIST KAT generation + upstream vector verification
make vectors  # upstream deterministic vector verification only
make check    # pairwise + KAT/vector verification
make clean
```

KAT generation requires a compatible OpenSSL `libcrypto` by default. The vector hash check uses `openssl dgst -sha256` rather than GNU `sha256sum` to avoid adding a coreutils dependency on legacy Unix systems. Override `KAT_LDLIBS`, `OPENSSL`, or `AWK` if the target system uses different commands or linker options.
