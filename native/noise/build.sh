#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
gcc -shared -fPIC -O2 -ffp-contract=off noise.c -o libnrnoise.so -lm
echo "built $(pwd)/libnrnoise.so"
