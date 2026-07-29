#!/usr/bin/env bash

mv lib/log/debug.log lib/log/debug-last.log

export NR_NOISE_SO="$(pwd)/native/noise/libnrnoise.so"
./build/bin/driver lib/secure/etc/local.config
