#!/bin/sh
# Run from any working directory. Use the same toolchain/flags as B0.
set -eu
script_dir=`dirname "$0"`
cd "$script_dir/.."
CC=${CC-cc}
CPPFLAGS=${CPPFLAGS-}
CFLAGS=${CFLAGS-}
LDFLAGS=${LDFLAGS-}
LDLIBS=${LDLIBS-}
mkdir -p build/arithmetic
for k in 2 3 4; do
    echo "Checking R arithmetic: KYBER_K=$k"
    # Toolchain variables intentionally use conventional shell word splitting.
    $CC $CPPFLAGS $CFLAGS -I. -DKYBER_K=$k  tests/optimization.c reduce.c \
        $LDFLAGS $LDLIBS -o "build/arithmetic/check_k$k"
    "./build/arithmetic/check_k$k"
done
