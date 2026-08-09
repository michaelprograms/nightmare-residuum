#!/usr/bin/env bash

# rebuild the FFI noise lib if the shared object is missing or old
if [ ! -f native/noise/libnrnoise.so ] \
    || [ native/noise/noise.c -nt native/noise/libnrnoise.so ] \
    || [ native/noise/noise.h -nt native/noise/libnrnoise.so ] \
    || [ native/noise/build.sh -nt native/noise/libnrnoise.so ]; then
    bash native/noise/build.sh
fi
export NR_NOISE_SO="$(pwd)/native/noise/libnrnoise.so"
./build/bin/driver lib/secure/etc/local.config -ftest