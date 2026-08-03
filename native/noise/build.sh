#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"

# parity self-test gate: the C port must reproduce the LPC reference values
# before we ship the shared object (set -e aborts the build on a failure).
gcc -O2 -ffp-contract=off -DNS_SELFTEST noise.c -o /tmp/nrnoise_selftest -lz -lm
/tmp/nrnoise_selftest

gcc -shared -fPIC -O2 -ffp-contract=off noise.c -o libnrnoise.so -lz -lm
echo "built $(pwd)/libnrnoise.so"
