# PQC Optimization on Legacy Unix Systems

This repository contains the experimental artifact for a study of ML-KEM optimization on legacy Unix systems.

## Repository layout

- `reference/kyber/` — unmodified upstream reference implementation from `pq-crystals/kyber`, included as a pinned Git submodule. This directory is the external reference source and is not an experimental candidate.
- `B0/` — common baseline. Only changes required for portability and compatibility are permitted (for example, type adjustments, build-system changes, timers, and platform-support helpers). No arithmetic optimization is introduced here.
- `A/` — B0 plus specialization for multiplication by public constants, including precomputation or dedicated constant-multiplication paths.
- `R/` — B0 plus changes to reduction timing and accumulation batching, restricted to cases whose intermediate ranges have been verified.
- `AR/` — B0 with both A and R applied. This candidate is used to measure whether the two optimization classes combine additively, redundantly, or antagonistically.

The four experimental candidates form a 2 x 2 design: A absent/present and R absent/present. Unless explicitly documented otherwise, FIPS202, RNG policy, input validation, secret cleanup, cache policy, benchmark inputs, compiler settings, and surrounding implementation are held fixed across B0, A, R, and AR.

## Reference implementation

The upstream Kyber repository is pinned to commit:

`3edd5af5991927164edd4aacebfcbee00b8064e7`

Clone this repository together with its reference submodule using:

```sh
git clone --recurse-submodules https://github.com/samdae5back/PQC_Optimization_On_Legacy_Unix_System.git
```

For an existing clone:

```sh
git submodule update --init --recursive
```

The reference implementation retains its upstream licensing terms. Code developed specifically for this repository is covered by the root `LICENSE` unless otherwise noted.

## Research-code notice

This repository is intended for research, benchmarking, and reproducibility. It is not intended for production cryptographic deployment.
