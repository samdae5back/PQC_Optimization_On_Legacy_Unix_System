# Endianness audit

The pinned reference implementation already performs ML-KEM serialization and Keccak byte loading/storing explicitly, rather than through host-order integer casts. B0 keeps that behavior and centralizes the remaining 24-bit and 32-bit little-endian loads used by the CBD sampler in `endian.h`.

This change is intended to make host-byte-order independence explicit for legacy big-endian targets while preserving the algorithmic behavior of the reference implementation.
