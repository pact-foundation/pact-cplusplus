# pact-cplusplus

[![Pact-C++ Build](https://github.com/pact-foundation/pact-cplusplus/workflows/Consumer%20Project/badge.svg)](https://github.com/pact-foundation/pact-cplusplus/actions/workflows/consumer.yaml)


C++ implementation of the consumer driven contract library [pact](https://github.com/pact-foundation/pact-specification).

From the [Ruby Pact website](https://github.com/pact-foundation/pact-ruby):

> Define a pact between service consumers and providers, enabling "consumer driven contract" testing.
>
>Pact provides an RSpec DSL for service consumers to define the HTTP requests they will make to a service provider and the HTTP responses they expect back. 
>These expectations are used in the consumers specs to provide a mock service provider. The interactions are recorded, and played back in the service provider 
>specs to ensure the service provider actually does provide the response the consumer expects.
>
>This allows testing of both sides of an integration point using fast unit tests.
>
>This gem is inspired by the concept of "Consumer driven contracts". See http://martinfowler.com/articles/consumerDrivenContracts.html for more information.


Read [Getting started with Pact](https://docs.pact.io/5-minute-getting-started-guide) for more information on how to get going.

## Installation

pact-cplusplus is two libraries, released separately:

| Library | Tag | CMake target | Conan reference |
| --- | --- | --- | --- |
| [consumer](consumer) | `pact-cpp-consumer-vX.Y.Z` | `pact-cpp-consumer` | `pact_cpp_consumer/X.Y.Z` |
| [verifier](verifier) | `pact-cpp-verifier-vX.Y.Z` | `pact-cpp-verifier` | `pact_cpp_verifier/X.Y.Z` |

Both wrap [pact_ffi](https://github.com/pact-foundation/pact-reference/tree/master/rust/pact_ffi),
the Rust core shared by every Pact implementation. Releases are **source only**:
each [GitHub release](https://github.com/pact-foundation/pact-cplusplus/releases)
is a tag plus notes naming the pact_ffi version it pins. You build the library
yourself, and CMake downloads the matching pact_ffi binary for your platform
at configure time, so no Rust toolchain is needed.

You need CMake 3.16+ and a C++17 compiler. The consumer library also needs the
Boost headers (no compiled Boost libraries), found with `find_package(Boost)`
or given with `-DBoost_ROOT=<dir>`. nlohmann_json is fetched automatically.

### CMake FetchContent (recommended)

```cmake
include(FetchContent)

FetchContent_Declare(pact_cpp_consumer
  GIT_REPOSITORY https://github.com/pact-foundation/pact-cplusplus.git
  GIT_TAG        pact-cpp-consumer-v0.2.0
  SOURCE_SUBDIR  consumer)
FetchContent_Declare(pact_cpp_verifier
  GIT_REPOSITORY https://github.com/pact-foundation/pact-cplusplus.git
  GIT_TAG        pact-cpp-verifier-v0.1.0
  SOURCE_SUBDIR  verifier)
FetchContent_MakeAvailable(pact_cpp_consumer pact_cpp_verifier)

target_link_libraries(my_pact_tests PRIVATE pact-cpp-consumer pact-cpp-verifier)
```

When you configure, the pinned pact_ffi release for your OS, architecture and
libc (glibc or musl) is downloaded into `<build>/_deps/libpact_ffi-<version>`.
Each file is checked against the `.sha256` published with it, and your target
is linked against the pact_ffi shared library. On Linux and macOS, the build
RPATH lets test executables run straight from the build tree. If you install
or ship them, or on Windows, put the pact_ffi `.so`/`.dylib`/`.dll` from that
directory next to the executable or on the loader path.

[examples/fetchcontent](examples/fetchcontent) is a complete project, and CI
builds and runs it.

| Cache variable | Default | Purpose |
| --- | --- | --- |
| `PACT_FFI_VERSION` | the version pinned by the release | pact_ffi release to download |
| `PACT_FFI_ROOT` | empty (download) | Use an existing pact_ffi tree instead of downloading |
| `PactDownloadFFI` | `ON` | Set `OFF` to fail rather than download when `PACT_FFI_ROOT` is unset |

### Plain CMake with your own pact_ffi

For offline or air-gapped builds, download the pact_ffi release assets
yourself, then point `PACT_FFI_ROOT` at a directory laid out as
`include/pact.h` and `lib/libpact_ffi-<os>-<arch>.{so,dylib}` (on Windows,
`lib/pact_ffi-windows-<arch>.{dll,dll.lib}`). On macOS, a top-level build is
universal (arm64 + x86_64) by default and needs the `.a` and `.dylib` for both
architectures, unless you pass `-DPactBuildUniversal=OFF`:

```console
git clone --branch pact-cpp-consumer-v0.2.0 https://github.com/pact-foundation/pact-cplusplus.git
cmake -S pact-cplusplus/consumer -B build -DCMAKE_BUILD_TYPE=Release \
  -DPACT_FFI_ROOT=/path/to/libpact_ffi-v0.5.6
cmake --build build
cmake --install build --prefix ./install
```

### Conan, from a clone

The repository includes Conan 2 recipes. Build the package into your local
cache from a release tag:

```console
git clone --branch pact-cpp-consumer-v0.2.0 https://github.com/pact-foundation/pact-cplusplus.git
conan create pact-cplusplus/consumer --build=missing -s compiler.cppstd=17
```

Then depend on it as usual:

```python
requires = "pact_cpp_consumer/0.2.0"
```

```cmake
find_package(pact_cpp_consumer CONFIG REQUIRED)
target_link_libraries(my_pact_tests PRIVATE pact_cpp_consumer::pact_cpp_consumer)
```

The package bundles the pact_ffi headers and shared library. Set the
`PACT_FFI_ROOT` environment variable to package a local pact_ffi instead of
downloading one. To share the build within your organisation, `conan upload`
it to your own remote (for example Artifactory or a Conan server). The
Pact Foundation does not host a Conan remote.

### Why isn't it on ConanCenter?

ConanCenter doesn't build packages that need a Rust toolchain, and its
maintainers have
[confirmed they are not changing that](https://github.com/conan-io/conan-center-index/pull/31112#issuecomment-6657412476).
pact_ffi is written in Rust, so these packages can't be published there.

## Contact

* Twitter: [@pact_up](https://twitter.com/pact_up)
* Slack: [Join the chat at http://slack.pact.io/](http://slack.pact.io/)
* Stack Overflow: https://stackoverflow.com/questions/tagged/pact

## Documentation

* [Main Pact documentation](https://docs.pact.io)
* Consumer DSL docs [consumer/](consumer/)
* Consumer DSL HTML docs [consumer/docs/html/](https://docs.pact.io/cpp_docs/consumer/annotated.html)
* Provider verifier docs [verifier/](verifier/)

## Consumer Tests

The support for consumer tests is documented in [consumer](consumer).

For an example for the consumer test DSL, have a look at [consumer_test.cpp](https://github.com/pact-foundation/pact-cplusplus/blob/master/consumer/test/src/consumer_test.cpp).

## Provider Verification

The support for verifying providers is documented in [verifier](verifier).

For an example of verifying a provider, including hosting provider states, have a look at [verifier_test.cpp](https://github.com/pact-foundation/pact-cplusplus/blob/master/verifier/test/src/verifier_test.cpp).

