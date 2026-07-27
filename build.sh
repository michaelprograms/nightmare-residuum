#!/usr/bin/env bash

set -euo pipefail

cp -f local_options ./driver/src/local_options

# compiler-output cache: keyed on source+flags, stored in ~/.ccache (NOT build/),
# so it survives the rm -rf below. Optional -- build still works without it.
ccache_flags=()
if command -v ccache >/dev/null 2>&1; then
  ccache_flags=(-DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache)
else
  echo "note: ccache not installed -- full rebuild (run: apt install ccache)"
fi

rm -rf build && mkdir -p build && cd build
time cmake ../driver \
  "${ccache_flags[@]}" \
  -DPACKAGE_DB_SQLITE=2 \
  -DPACKAGE_UIDS=OFF \
  -DPACKAGE_MUDLIB_STATS=OFF \
  -DPACKAGE_EXTERNAL=ON \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo

time make -j $(nproc) install
cd ..
