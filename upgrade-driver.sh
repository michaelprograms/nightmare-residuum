#!/usr/bin/env bash

set -euo pipefail

# descend into the driver
cd driver

# current driver's last commit date
currentVersion=$(git log -1 --format="%at" | xargs -I{} date -d @{} +%Y-%m-%d\ %H:%M:%S)

# stash any driver changes as the current time
git stash save "$(date +%Y-%m-%d\ %H:%M:%S)"

# attempt to update the driver
git pull

# updated driver's last commit date
updateVersion=$(git log -1 --format="%at" | xargs -I{} date -d @{} +%Y-%m-%d\ %H:%M:%S)

if [[ "$currentVersion" == "$updateVersion" ]]; then
  echo "Fluffos driver already on the latest version of ${currentVersion}"
else
  echo "Fluffos driver updated from ${currentVersion} to ${updateVersion}"
fi

cd ..