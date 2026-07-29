#!/usr/bin/env bash

set -euo pipefail

cp -f local_options ./driver/src/local_options

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

# Only the `driver`: `install` implies `all`, which is 6 more executables that
# each pay a full whole-program LTO link of libdriver (lpcc/lpcshell/symbol/
# o2json/json2o/generate_keywords, all dev/offline tools we don't run).
time make -j $(nproc) driver
mkdir -p bin && cp -f src/driver bin/driver
cd ..

# native noise library for FFI acceleration (package_ffi); optional at runtime
bash native/noise/build.sh
