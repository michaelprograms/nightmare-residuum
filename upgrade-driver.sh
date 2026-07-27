#!/usr/bin/env bash

set -euo pipefail

# descend into the driver
cd driver

# replicate the driver's build version string (see driver/CMakeLists.txt PROJECT_VERSION)
driver_version() {
  local stamp local_commit="" dirty=""
  stamp=$(git log --committer='Yucong Sun' -1 --date=format:%Y%m%d --format=%cd-%h)
  if [[ $(git log --committer='Yucong Sun' -1 --format=%h) != $(git log -1 --format=%h) ]]; then
    local_commit="-$(git log -1 --format=%h)"
  fi
  git diff-index --quiet HEAD -- || dirty="-uncommited"
  echo "${stamp}${local_commit}${dirty}"
}

# current driver commit + version
before=$(git rev-parse HEAD)
currentVersion=$(driver_version)

# stash any driver changes as the current time
git stash save "$(date +%Y-%m-%d\ %H:%M:%S)"

# attempt to update the driver
git checkout master
git pull origin master

# updated driver commit + version
after=$(git rev-parse HEAD)
updateVersion=$(driver_version)

if [[ "$before" == "$after" ]]; then
  message="FluffOS driver already on the latest version of ${updateVersion}"
else
  message="FluffOS driver updated from ${currentVersion} to ${updateVersion}"
fi
echo "$message"

cd ..

# commit the updated submodule pointer if it moved
if ! git diff --quiet HEAD -- driver; then
  git commit -m "$message" -- driver
fi
