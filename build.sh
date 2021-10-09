#!/bin/bash
set -euo pipefail

rm -rf build
mkdir -p build

cp -f local_options ./driver/src/

cd build
cmake ../driver \
  -DCMAKE_BUILD_TYPE=Release \
  -DPACKAGE_DB_SQLITE=2 \
  -DPACKAGE_UIDS=OFF \
  -DPACKAGE_MUDLIB_STATS=OFF

make install
cd ..