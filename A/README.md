# A - public-constant multiplication specialization

## Baseline and scope

The initial A/R commit copies the B0 tree from main commit
`856da4fc299846fa6b40b9435586f2616ae4169f` exactly, including symlinks.
This A-only change modifies **only `ntt.c` in the production sources**.

Forward NTT twiddle multiplication, inverse NTT twiddle multiplication,
and the inverse transform's final factor 1441 use a dedicated fixed-public-
factor Montgomery path. The generic basemul path, `poly_tomont`, reduction
schedule, and all other production code are unchanged. In particular, this
commit does not implement R, combine the candidates, or alter B0/reference/AR.

FIPS202, RNG, validation, cleanup, compiler flags, the existing benchmark
harness, and cache measurement policy remain the copied B0 versions.
The additional 256-byte read-only table is part of A's measured cost, not a
change to the warm/cold-cache measurement policy.

## Arithmetic identity and bounds

Let `M = 65536`, `q = 3329`, and `QINV = -3327`.
For a public factor `c`, precompute `ci = signed_low16(c * QINV)`.
Then

```
signed_low16((a * c) * QINV) = signed_low16(a * ci)
```

so the correction limb and final representative are **bit-for-bit identical**
to B0's Montgomery multiplication. The two products `a*c` and `a*ci` can be
computed independently. This does not remove a Montgomery reduction or
claim fewer integer multiplications; it changes the dependency chain and
uses a dedicated helper rather than the generic reduction call path.

For every signed 16-bit `a`, the used constants satisfy `|c| <= 1664` and
`|ci| <= 32768`. Thus `|a*ci| <= 2^30`, `|a*c| <= 54525952`, and
`|a*c - t*q| <= 163610624`, all within signed 32-bit arithmetic.
The signed narrowing and right-shift assumptions are the same as B0's;
no new wider integer type, compiler extension, assembly, secret-indexed
lookup, or data-dependent branch is introduced.

`zetas_qinv[i]` is the signed low 16 bits of `zetas[i] * QINV`.
The corresponding value for 1441 is -10079. The regression test recomputes
these constants and exhaustively checks all 65,536 signed 16-bit inputs
for every one of the 129 constants (8,454,144 products).

## Build and validation

Initialize the pinned reference submodule before building. Work inside
`A/`, not `B0/`; otherwise the copied build instructions in
[the baseline README](../B0/README.md) apply without flag changes.
The copied archive names still use `libb0_kyber*.a`, but reside in A's own
`build/lib/` directory. Never mix archives from different candidates.

```sh
# From the repository root:
CC=cc sh A/tests/check_optimization.sh
CC=gcc CFLAGS=-O2 sh A/tests/check_optimization.sh

# Full inherited KEM/pairwise/KAT/vector-hash checks on the target machine:
cd A
make clean
make check CC=cc
```

`tests/optimization.c` includes the actual B0 and A arithmetic sources
under separate test symbols. It checks exact transform equivalence,
constant products, boundary/random basemuls, K-term normalized dot products,
and complete negacyclic polynomial products against an independent
schoolbook oracle. The test code is not linked into the benchmark libraries.

Local validation on 2026-09-17: Linux x86-64, GCC 14.2.0 and Clang 17.0.0;
`-O0` and `-O2`, strict C89 with warnings as errors, all `KYBER_K=2,3,4`;
plus Clang AddressSanitizer/UndefinedBehaviorSanitizer at `-O1`: passed.
Only the arithmetic sources and regression runner were built in that local
check. **Full KEM KAT/pairwise/vector-hash checks and native legacy-Unix
builds have not yet been run for A.** No speedup is asserted before measurement.
