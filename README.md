[![CI on ubuntu](https://github.com/michaelprograms/nightmare-residuum/actions/workflows/ci.yml/badge.svg)](https://github.com/michaelprograms/nightmare-residuum/actions/workflows/ci.yml)

# Nightmare Residuum

An [LPMud](https://en.wikipedia.org/wiki/LPMud) inspired by the [Nightmare Mudlib](https://github.com/fluffos/nightmare3) but utilizing modern [fluffos](https://github.com/fluffos/fluffos) driver features.

![image](https://user-images.githubusercontent.com/1260602/235332602-dd4fb705-8b14-4bc9-9b85-fec29276bf84.png)

## Fluffos Driver

### Requirements

The fluffos driver builds and runs best on Ubuntu 20.04 with the following dependencies installed (reference this repository's [Github Action for build-and-test](.github/workflows/ci.yml)):

```
sudo apt install -y build-essential clang-6.0 autoconf automake bison expect libevent-dev libmysqlclient-dev libpcre3-dev libpq-dev libsqlite3-dev libssl-dev libtool libz-dev telnet libgtest-dev libjemalloc-dev
sudo apt remove libgcc-11-dev gcc-11
```

### Build the driver
```
./build.sh
```

### Run the mudlib
```
./run.sh
```

### Run the mudlib test suite
```
./run-tests.sh
```

### Update the driver docs
```
./docs.sh
```
