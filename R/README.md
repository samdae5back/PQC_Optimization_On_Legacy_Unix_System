# R - bounded reduction/accumulation batching

## Baseline and scope

The initial A/R commit copies the B0 tree from main commit
`856da4fc299846fa6b40b9435586f2616ae4169f` exactly, including symlinks.
This R-only change modifies **only `basemul` in `ntt.c` in production**.
No A precomputation or specialized constant-multiplication helper is used.
B0, reference/kyber, AR, NTT/inverse-NTT scheduling, FIPS202, RNG, validation,
cleanup, compiler flags, benchmark inputs, and the existing harness remain
unchanged.

This first R implementation batches the two same-scale products within each
quadratic basemul output. It does not yet move reductions across NTT stages
or across separate vector elements.

## Reduction identity

Writing `REDC(x) = x * 65536^-1 (mod 3329)`, compute

```
t    = REDC(a1*b1)
acc0 = a0*b0 + t*zeta
acc1 = a0*b1 + a1*b0
r0   = REDC(acc0)
r1   = REDC(acc1)
```

This reduces the number of Montgomery reductions per basemul from **5 to 3**.
The first reduction on `a1*b1` is retained because it aligns the Montgomery
scale before multiplication by zeta. The accumulators are signed 32-bit;
no 64-bit accumulator, new reciprocal table, or secret-dependent branch is
introduced.

Outputs are congruent to B0 modulo q, but their raw representatives can
differ by a multiple of q. The unchanged
`polyvec_basemul_acc_montgomery` ends with `poly_reduce`, which produces the
same centered coefficients. Raw, unnormalized basemul buffers must therefore
be compared modulo q, not with `memcmp`.

## Range contract and proof

The supported internal input domain is `|a[i]|, |b[i]| <= 4095` and
`|zeta| <= 1664`. Existing KEM callers use centered post-NTT coefficients,
matrix samples below q, or 12-bit unpacked coefficients. The latter bound
also covers non-canonical 12-bit encodings; no validation policy is changed.
This is not a promise for arbitrary full-range int16_t basemul inputs.

Using `|REDC(x)| <= ceil((|x| + 32768*q) / 65536)` gives:

| Quantity | Conservative absolute bound |
| --- | ---: |
| Each input product | 16,769,025 |
| First reduced product t | 1,921 |
| acc0 | 19,965,569 |
| acc1 | 33,538,050 |
| Either output coefficient | 2,177 |
| Sum of up to four output coefficients | 8,708 |

Both accumulators are strictly within `q*2^15 = 109,084,672`, the existing
Montgomery reducer's input contract. Its internal subtraction is bounded
by 142,622,722 in absolute value, well within int32_t. The existing int16_t
vector accumulation therefore remains safe for `KYBER_K=2,3,4` before the
unchanged final Barrett reduction. B0's signed-narrowing/right-shift target
assumptions are retained.

## Build and validation

Initialize the pinned reference submodule before building. Work inside
`R/`, not `B0/`; otherwise the copied build instructions in
[the baseline README](../B0/README.md) apply without flag changes.
The copied archive names still use `libb0_kyber*.a`, but reside in R's own
`build/lib/` directory. Never mix archives from different candidates.

```sh
# From the repository root:
CC=cc sh R/tests/check_optimization.sh
CC=gcc CFLAGS=-O2 sh R/tests/check_optimization.sh

# Full inherited KEM/pairwise/KAT/vector-hash checks on the target machine:
cd R
make clean
make check CC=cc
```

`tests/optimization.c` includes the actual B0 and R arithmetic sources
under separate test symbols. It checks 859,491 boundary tuples, 100,000
deterministic random basemuls, explicit accumulator bounds, normalized
B0 equivalence, an independent modular oracle, 10,000 K-term dot products,
transform equivalence, and 32 complete polynomial products against an
independent negacyclic schoolbook oracle. The regression code is not linked
into the benchmark libraries.

Local validation on 2026-09-17: Linux x86-64, GCC 14.2.0 and Clang 17.0.0;
`-O0` and `-O2`, strict C89 with warnings as errors, all `KYBER_K=2,3,4`;
plus Clang AddressSanitizer/UndefinedBehaviorSanitizer at `-O1`: passed.
Only the arithmetic sources and regression runner were built in that local
check. **Full KEM KAT/pairwise/vector-hash checks and native legacy-Unix
builds have not yet been run for R.** No speedup is asserted before measurement.
