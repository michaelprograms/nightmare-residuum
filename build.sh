#!/usr/bin/env bash

set -euo pipefail

cp -f local_options ./driver/src/local_options
rm -rf build && mkdir -p build && cd build
cmake ../driver \
  -DPACKAGE_DB_SQLITE=2 \
  -DPACKAGE_UIDS=OFF \
  -DPACKAGE_MUDLIB_STATS=OFF \
  -DPACKAGE_EXTERNAL=ON \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo

make -j `nproc` install
cd ..
