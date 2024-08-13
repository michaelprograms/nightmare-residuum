#!/usr/bin/env bash

mkdir -p backup

tar -cvf backup/"$(date +%Y-%m-%d\_%H-%M-%S)".tar lib/save lib/realm

# To restore data:
# tar -xvf backup/YYYY-MM-DD_HH-MM-SS.tar