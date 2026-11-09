#!/bin/bash
set -eo pipefail

# usage: ./build.sh --project=consumer --os=linux --test --release
#
# options:
#   --project=<project>   Specify the project directory (e.g., consumer)
#   --os=<os>             Specify the target OS (e.g., linux, mac)
#   --test                Enable running tests
#   --release             Install the built library into <project>/install
#
# pact_ffi is downloaded and checksum-verified by CMake at configure time;
# export PACT_FFI_ROOT=<dir> to use a local copy instead.

echo $@
for arg in "$@"; do
  case $arg in
    --os=*)
      ARG_OS="${arg#*=}"
      shift
      ;;
    --project=*)
      ARG_PROJECT="${arg#*=}"
      shift
      ;;
    --test)
      ARG_TEST="true"
      shift
      ;;
    --release)
      ARG_RELEASE="true"
      shift
      ;;
    *)
      ;;
  esac
done

if [ -z "$ARG_OS" ]; then
  echo "No OS specified"
  exit 1
fi

# detect if alpine
if [ -f /etc/alpine-release ]; then
  IS_ALPINE="true"
else
  IS_ALPINE="false"
fi

case "$ARG_OS" in
  mac|linux) ;;
  *)
    echo "Unsupported --os=$ARG_OS (expected mac or linux)"
    exit 1
    ;;
esac

if [ "$IS_ALPINE" == "true" ]; then
  apk add build-base cmake curl python3 py3-pip bzip2-dev zlib-dev xz-dev gzip linux-headers perl bash automake autoconf libtool m4
  pip3 install --upgrade pip --break-system-packages
  pip3 install "conan>=2.0" --break-system-packages
  which conan || export PATH="$PATH:~/.local/bin"
elif [ "$ARG_OS" == "linux" ]; then
    sudo apt-get install -y libbz2-dev zlib1g-dev liblzma-dev libicu-dev libboost-all-dev cmake python3-pip
    pip3 install --upgrade pip --break-system-packages
    pip3 install "conan>=2.0" --break-system-packages
    which conan || export PATH="$PATH:~/.local/bin"
fi

conan profile detect --force
if [ "$IS_ALPINE" == "true" ]; then
  # Register os.distro (conan-io/conan#16179) so every package in the graph gets a
  # musl-specific package_id, instead of silently reusing glibc binaries on Alpine.
  # This has to follow `conan profile detect --force`, which rewrites the profile.
  printf "os:\n  Linux:\n    distro: [null, alpine]\n" > ~/.conan2/settings_user.yml
  echo "os.distro=alpine" >> ~/.conan2/profiles/default
fi
conan profile show

case "$ARG_OS" in
  linux) extra_build="--build=m4/*" ;;
esac
if [ "$ARG_TEST" == "true" ]; then
  export PactBuildTests=ON
else
  export PactBuildTests=OFF
fi
if [ -d "$ARG_PROJECT" ]; then
  cd "$ARG_PROJECT"
else
  echo "Project directory $ARG_PROJECT not found"
  exit 1
fi
conan install conanfile.py \
            --build=missing $extra_build \
            -s build_type=Release \
            -s compiler.cppstd=17 \
            -c tools.build:jobs=2
preset=conan-release
cmake --preset $preset \
            -DPactUseConan=ON \
            -DPactBuildTests=$PactBuildTests
cmake --build --preset $preset

if [ "$ARG_TEST" == "true" ]; then
  ctest --preset $preset --output-on-failure --verbose
fi

if [ "$ARG_RELEASE" == "true" ]; then
  cmake --install build/Release --config Release --prefix install
fi
