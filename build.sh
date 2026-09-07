#!/bin/bash

# Pre-requisite: Download pact ffi dependencies
# cd [pact-cplusplus sibling dir]
# mkdir libpact_ffi-v0.5.0
# cd libpact_ffi-v0.5.0
# mkdir include
# cd include
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/pact.h
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/pact-cpp.h
# cd ..
# mkdir lib
# cd lib
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-linux-x86_64.a.gz
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-linux-x86_64.so.gz
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-linux-aarch64.a.gz
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-linux-aarch64.so.gz
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-linux-x86_64-musl.a.gz
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-linux-x86_64-musl.so.gz
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-linux-aarch64-musl.a.gz
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-linux-aarch64-musl.so.gz
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-macos-aarch64.a.gz
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-macos-aarch64.dylib.gz
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-macos-x86_64.a.gz
# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.5.0/libpact_ffi-macos-x86_64.dylib.gz
# gunzip *.gz

ARG_OS=${1:-}

export _PACT_FFI_VERSION="v0.5.0"
export _CMAKE_OSX_ARCHITECTURES=""

if [ "$ARG_OS" == "mac" ]; then
  export _CMAKE_OSX_ARCHITECTURES="arm64;x86_64"
  export _BOOST_ROOT=$(brew --prefix boost)/include
  export _PACT_FFI_ROOT=$(pwd)/consumer/libpact_ffi-$_PACT_FFI_VERSION
  echo "Building for macOS"
else
  export _BOOST_ROOT=/usr/include
  export _PACT_FFI_ROOT=$(pwd)/consumer/libpact_ffi-$_PACT_FFI_VERSION
  echo "Building for linux"
fi

mkdir -p build
cmake -S consumer -B build -DCMAKE_BUILD_TYPE=Release -DBoost_ROOT=$_BOOST_ROOT -DPACT_FFI_ROOT=$_PACT_FFI_ROOT -DPACT_FFI_VERSION=$_PACT_FFI_VERSION -DPactUseConan=OFF -DPactBuildTests=OFF -DCMAKE_OSX_ARCHITECTURES=$_CMAKE_OSX_ARCHITECTURES
cmake --build build --config Release
cmake --install build --prefix ./build/install/pact-cpp

# cd build/install/pact-cpp
# 7za -mm=lzma -mx9 -r a ../[linux or MAC64].zip *
# cd ../../../

# To copy from docker container to host (windows example)
# docker cp MyFusionDev:/home/buser/pact-cplusplus/build/install/linux.zip D:\temp