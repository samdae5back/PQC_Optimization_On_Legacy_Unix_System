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
│   └── kat1024
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
└── obj/
```

The parameter-specific libraries contain the KEM core and FIPS202 implementation but intentionally do not contain an entropy provider. Pairwise and timing tests link the normal OS-backed `randombytes.c`, while the KAT executables link the deterministic NIST KAT RNG implementation. This keeps all three test classes on the same cryptographic library while allowing the randomness source to be selected by the test harness.

Useful targets:

```sh
make          # libraries + pairwise + timing + KAT
make libs     # libraries only
make pairwise # pairwise tests and result files
make time     # timing tests and result files
make kat      # KAT executables and generated req/rsp files
make check    # pairwise + KAT
make clean
```

KAT generation requires a compatible OpenSSL `libcrypto` by default. Override `KAT_LDLIBS` if the target system uses a different linker option or crypto provider.
