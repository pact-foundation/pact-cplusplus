# Pact C++ Provider Verifier

Verifies that a provider honours the contracts published by its consumers, by
wrapping the [Pact FFI](https://github.com/pact-foundation/pact-reference) verifier.

Every option exposed by the FFI verifier is available, and the library can host
the HTTP endpoint the verifier uses to set up provider states, so you can write
state handlers as plain C++ lambdas.

## Installing

Requires CMake 3.16+, a C++17 compiler and the `pact_ffi` binaries for your
platform, downloaded from the
[pact-reference releases](https://github.com/pact-foundation/pact-reference/releases).

```console
cmake -S verifier -B build \
  -DPACT_FFI_ROOT=/path/to/libpact_ffi-v0.5.6 \
  -DPACT_FFI_VERSION=v0.5.6
cmake --build build
cmake --install build --prefix ./install
```

Dependencies can also be supplied by Conan 2:

```console
export PACT_FFI_ROOT=/path/to/libpact_ffi-v0.5.6
conan install conanfile.py --build=missing -s build_type=Release -s compiler.cppstd=17
cmake --preset conan-release -DPactUseConan=ON -DPactBuildTests=ON \
  -DPACT_FFI_VERSION=v0.5.6 -DPACT_FFI_ROOT="$PACT_FFI_ROOT"
cmake --build --preset conan-release
ctest --preset conan-release --output-on-failure
```

## Verifying a provider

Start your provider, point the verifier at it, tell it where the pacts are and
run it.

```cpp
#include <verifier.h>

using namespace pact_verifier;

int main() {
  init();

  Verifier verifier;
  verifier
    .set_provider_info("my-provider", "http", "localhost", 8080, "/")
    .add_file_source("./pacts/my-consumer-my-provider.json");

  auto result = verifier.execute();
  result.display();
  return result.is_ok() ? 0 : 1;
}
```

## Provider states

The verifier does not call your code directly to set up provider states, it makes
an HTTP request per state, so something has to be listening. Register a handler
and the library starts and stops a state change endpoint around the verification
for you.

```cpp
verifier
  .add_state_handler("the user exists", [](const ProviderStateRequest& request) {
    if (request.action == StateAction::Setup) {
      seed_user(request.params.get_int("id").value_or(0), request.params.get_or("name", ""));
    } else {
      clear_users();
    }
  })
  .add_state_handler("the user does not exist", [](const ProviderStateRequest& request) {
    delete_user(request.params.get_int("id").value_or(0));
  });
```

Parameters come from the consumer side `given("the user exists", { {"id", "1"} })`.
`ProviderStateParams` exposes them as strings with typed accessors
(`get_int`, `get_double`, `get_bool`); any non-string JSON value is returned as
its raw JSON text.

To feed values back into `fromProviderState` generators, return a JSON object:

```cpp
verifier.add_state_handler_with_values("a user is created", [](const ProviderStateRequest&) {
  return R"({"id": 100})";
});
```

Other options:

- `set_default_state_handler(handler)` — handles any state without a specific handler
- `set_state_change_port(port)` — pin the endpoint port (defaults to a free port)
- `set_state_change_teardown(false)` — skip the teardown call after each interaction
- `set_state_change_body(false)` — send state data as query parameters instead of a JSON body

If your application already hosts a state change endpoint, skip the handlers and
use `set_provider_state_url("http://localhost:8080/__pact/provider-state")`.

You can also drive `ProviderStateServer` yourself if you want to control its
lifetime, for example to run it inside your application under test.

## Fetching pacts from a broker

```cpp
BrokerSelectorOptions options;
options.url = "https://my-org.pactflow.io";
options.token = std::getenv("PACT_BROKER_TOKEN");
options.enable_pending = true;
options.include_wip_pacts_since = "2024-01-01";
options.provider_branch = "main";
options.consumer_version_selectors = {
  R"({"mainBranch": true})",
  R"({"deployedOrReleased": true})"
};

verifier.add_broker_source_with_selectors(options);
```

Publish the results back with:

```cpp
PublishOptions publish;
publish.provider_version = git_sha();
publish.provider_branch = "main";
publish.build_url = std::getenv("BUILD_URL");
verifier.set_publish_options(publish);
```

## Pact sources

| Method | Description |
| --- | --- |
| `add_file_source(file)` | Verify a single pact file |
| `add_directory_source(dir)` | Verify every matching pact in a directory |
| `add_url_source(url, ...)` | Fetch a pact from a URL |
| `add_broker_source(url, ...)` | Fetch the latest pacts for the provider from a broker |
| `add_broker_source_with_selectors(options)` | Fetch pacts using consumer version selectors, pending and WIP pacts |

## Other verifier options

| Method | Description |
| --- | --- |
| `set_provider_info(name, scheme, host, port, path)` | Where the running provider is |
| `add_provider_transport(protocol, port, path, scheme)` | Extra transport, e.g. `"message"` for message pacts |
| `add_custom_header(name, value)` | Header added to every request to the provider |
| `set_follow_redirects(bool)` | Follow redirects returned by the provider |
| `set_verification_options(disable_ssl_verification, request_timeout)` | TLS and timeout behaviour |
| `set_filter_info(description, state, no_state)` | Only verify matching interactions |
| `set_consumer_filters(consumers)` | Only verify pacts from these consumers |
| `set_coloured_output(bool)` | ANSI colour in the output |
| `set_strip_ansi_from_output(bool)` | Strip ANSI codes from `VerificationResult::output()` |
| `set_no_pacts_is_error(bool)` | Whether finding no pacts fails the run |
| `get_handle()` | The raw `VerifierHandle*`, for anything not wrapped here |

## Logging

```cpp
log_to_stdout(LogLevel::Debug);
log_to_buffer(LogLevel::Debug);  // required for VerificationResult::logs()
log_to_file("verification.log", LogLevel::Trace);
```

## Results

`execute()` returns a `VerificationResult`:

- `is_ok()` / `result_code()` — whether verification passed
- `output()` — the human readable report, as printed by the Pact CLI verifier
- `json()` — machine readable results
- `logs()` — verifier logs, if `log_to_buffer` was called first
- `display()` — prints `output()` to stdout

## Examples

See [test/src/verifier_test.cpp](test/src/verifier_test.cpp) for a complete
example that stands up a stub provider, seeds it from provider state handlers and
verifies a pact file against it.
