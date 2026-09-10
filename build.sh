#!/bin/bash
set -eio pipefail

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
    --package)
      ARG_PACKAGE="true"
      shift
      ;;
    *)
      ;;
  esac
done

export PACT_FFI_VERSION="v0.5.6"
export PACT_FFI_ROOT=$(pwd)/libpact_ffi-$PACT_FFI_VERSION

case "$(uname -m)" in
  x86_64) ARCH="x86_64" ;;
  aarch64|arm64) ARCH="aarch64" ;;
  *) ARCH="unknown" ;;
esac
case "$ARG_OS" in
  mac) FFI_ARCH="macos-$ARCH" ;;
  linux) FFI_ARCH="linux-$ARCH" ;;
esac


if [ -d "$PACT_FFI_ROOT" ]; then
  echo "Found PACT_FFI_ROOT at $PACT_FFI_ROOT"
else
  echo "PACT_FFI_ROOT not found, downloading..."
  root="libpact_ffi-${PACT_FFI_VERSION}"                                                             
  base="https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-${PACT_FFI_VERSION}"
  mkdir -p "$root/include" "$root/lib"
  curl -sSfL -o "$root/include/pact.h" "$base/pact.h"
  curl -sSfL -o "$root/include/pact-cpp.h" "$base/pact-cpp.h"
  case "$FFI_ARCH" in
    macos-*)   files="libpact_ffi-macos-aarch64.a libpact_ffi-macos-aarch64.dylib libpact_ffi-macos-x86_64.a libpact_ffi-macos-x86_64.dylib" ;;
    linux-*)    files="libpact_ffi-$FFI_ARCH.a libpact_ffi-$FFI_ARCH.so" ;;
  esac  
  for f in $files; do
  curl -sSfL -o "$root/lib/$f.gz" "$base/$f.gz"
  gunzip -f "$root/lib/$f.gz"
  done
fi

conan profile detect --force
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
            -DPactBuildTests=$PactBuildTests \
            -DPACT_FFI_VERSION=$PACT_FFI_VERSION \
            -DPACT_FFI_ROOT="$PACT_FFI_ROOT"
cmake --build --preset $preset

if [ "$ARG_TEST" == "true" ]; then
  ctest --preset conan-release --output-on-failure --verbose
fi

if [ "$ARG_RELEASE" == "true" ]; then
  cmake --install build/Release --config Release --prefix install
fi

if [ "$ARG_PACKAGE" == "true" ]; then
  mkdir -p artifacts
  lib=$(find install/pact-cpp-consumer -type f -name "*pact-cpp-consumer.*" | head -1)
  name=$(basename "$lib")

  gzip -c "$lib" > "artifacts/${name%.*}-${FFI_ARCH}.${name##*.}.gz"
  cp include/*.h artifacts
  (cd artifacts && for f in *.gz; do openssl dgst -sha256 -r "$f" > "$f.sha256"; done)
  echo "Artifacts have been created in the $ARG_PROJECT/artifacts directory"
fi