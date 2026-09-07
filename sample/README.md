# Pact C++ Consumer Sample

This project links directly to the release files under `../build/install/pact-cpp`.

Configure, build, and run it from this directory:

```console
cmake -S . -B build
cmake --build build
./build/pact-consumer-sample
```

To use an installation in another location, pass its root directory to CMake:

```console
cmake -S . -B build -DPACT_CPP_ROOT=/path/to/pact-cpp
```

The program initializes Pact FFI, defines the `GET /projects/1001` interaction shown in the consumer README, and executes it with `Pact::run_test`. It deliberately makes no request to the mock server, so it prints the resulting mismatch and exits with status `1`.

Replace the empty callback with your HTTP client call and return whether that call succeeded. When the configured interaction is received, `run_test` writes the pact file to the configured `pacts` directory.