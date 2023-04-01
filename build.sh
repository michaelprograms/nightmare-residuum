#!/usr/bin/env bash

set -euo pipefail

rm -rf build
mkdir -p build

cp -f local_options ./driver/src/

cd build
cmake ../driver \
  -DPACKAGE_DB_SQLITE=2 \
  -DPACKAGE_UIDS=OFF \
  -DPACKAGE_MUDLIB_STATS=OFF \
  -DPACKAGE_EXTERNAL=ON

# -DCMAKE_BUILD_TYPE=Release \

make -j `nproc` install
cd ..