#!/usr/bin/env bash

[ -f native/noise/libnrnoise.so ] || bash native/noise/build.sh
export NR_NOISE_SO="$(pwd)/native/noise/libnrnoise.so"
./build/bin/driver lib/secure/etc/local.config -ftest