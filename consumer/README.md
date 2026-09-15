# C++ Consumer DSL

This is a static library (pact-cpp-consumer) that you can link into your test binaries and it provides a DSL for setting up and running a consumer test. It wraps the Pact-Rust [pact_ffi](https://github.com/pact-foundation/pact-reference/tree/master/rust/pact_ffi) library, which will need to be linked as well.

## Using the DSL

Documentation is [here in html](https://docs.pact.io/cpp_docs/consumer/annotated.html). For an example, see [consumer_test.cpp](https://github.com/pact-foundation/pact-cplusplus/blob/master/consumer/test/src/consumer_test.cpp).

### Setup the expectations 

Firstly, you need to instantiate an instance of `pact_consumer::Pact` in your test. This class takes the name of the consumer
and provider as parameters, and can then be used to define the expectations.

For instance:

```cpp
  auto provider = Pact("TodoAppCpp", "TodoServiceCpp");
  provider
    .given("i have a list of projects")
    .uponReceiving("a request for projects")
    .withRequest("GET", "/projects")
    .withQuery(query)
    .withHeaders(headers)
    .willRespondWith(200)
    .withResponseHeaders(res_headers)
    .withResponseJsonBody(Object({
      { "projects", EachLike(Object({
        { "id", Integer(3) },
        { "name", Like("Project 1") },
        { "due", DateTime("yyyy-MM-dd'T'HH:mm:ss.SSSX") },
        { "tasks", AtLeastOneLike(4, Object({
          { "id", Integer() },
          { "name", Matching("Task \\d+", "Task 1") },
          { "done", Like(true) }
        }))}
      }))}
    }));
```

#### Matchers

For defining JSON bodies, there are a number of matcher functions that can be used with the `withJsonBody` and `withResponseJsonBody` methods.

##### `Object(const std::unordered_map<std::string, IMatcher::Ptr> fields)`

Represents a JSON object (map of string fields to matchers).

##### `Integer(long value)`

Matchers an integer value (must be a number and have no decimal places).

##### `Integer(int value)`

Matchers an integer value (must be a number and have no decimal places).

##### `Integer()`

Matchers an integer value (must be a number and have no decimal places). A random example value will be generated.

##### `Like(long value)`

Matchers any value based on the value type.

##### `Like(int value)`

Matchers any value based on the value type.

##### `Like(double value)`

Matchers any value based on the value type.

##### `Like(std::string value)`

Matchers any value based on the value type.

##### `Like(const char *value)`

Matchers any value based on the value type.

##### `Like(bool value)`

Matchers any value based on the value type.

##### `DateTime(std::string format, std::string example)`

String value that must match the provided datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html).

##### `DateTime(std::string format)`

String value that must match the provided datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html). Example values will be generated using the current system date and time.

##### `Date(std::string format, std::string example)`

String value that must match the provided date format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html).

##### `Date(std::string format)`

String value that must match the provided date format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html). Example values will be generated using the current system date and time.


##### `Time(std::string format, std::string example)`

String value that must match the provided time format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html).

##### `Time(std::string format);

String value that must match the provided time format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html). Example values will be generated using the current system date and time.


##### `Matching(std::string regex, std::string example)`

String value that must match the regular expression.

##### `Matching(std::string regex)`

String value that must match the regular expression. Random values will be generated for the examples.

##### `EachLike(const IMatcher::Ptr obj)`

Array of values where each item in the array must match the provided template. Will generate an example array with one value.

##### `EachLike(int examples, const IMatcher::Ptr obj)`

Array of values where each item in the array must match the provided template. `examples` defines how many examples should
be generated in the example array.

##### `AtLeastOneLike(const IMatcher::Ptr obj)`

Array of values that must have at least one value and each item in the array must match the provided template. Will generate an example array with one value.

##### `AtLeastOneLike(int examples, const IMatcher::Ptr obj)`

Array of values that must have at least one value and each item in the array must match the provided template.`examples` defines how many examples should be generated in the example array.

##### `HexValue(const std::string example)`

Match a hexadecimal string value.

##### `HexValue()`

Match a hexadecimal string value. Random example will be generated.

##### `IPAddress(const std::string example)`

Match an IP Address.

##### `IPAddress()`

Match an IP Address. Will use 127.0.0.1 for examples.

##### `Numeric(int example)`

Match any numeric value. This will match any numeric type (integer or floating point).

##### `Numeric(long example)`

Match any numeric value. This will match any numeric type (integer or floating point).

##### `Numeric(float example)`

Match any numeric value. This will match any numeric type (integer or floating point).

##### `Numeric(double example)`

Match any numeric value. This will match any numeric type (integer or floating point).

##### `Numeric()`

Match any numeric value. Random decimal values will be generated for examples.

##### `Decimal(float example)`

Match a decimal value (number with atleast one digit after the decimal point).

##### `Decimal(double example)`

Match a decimal value (number with atleast one digit after the decimal point).

##### `Decimal()`

Match a decimal value (number with atleast one digit after the decimal point). Random values will be generated.

##### `Uuid(const std::string example)`

Match a universally unique identifier (UUID).

##### `Uuid()`

Match a universally unique identifier (UUID). Random values will be used for examples.

##### `AtMostLike(int max, const IMatcher::Ptr obj)`

Array with maximum size and each element like the template object. Will generate an example array with one value.

##### `AtMostLike(int max, int examples, const IMatcher::Ptr obj)`

Array with maximum size and each element like the template object. `examples` defines how many examples should
be generated in the example array.

##### `MinArrayLike(int min, const IMatcher::Ptr obj)`

Array with minimum size and each element like the template object. Will generate an example array with one value.

##### `MinArrayLike(int min, int examples, const IMatcher::Ptr obj)`

Array with minimum size and each element like the template object. `examples` defines how many examples should
be generated in the example array.

##### `ConstrainedArrayLike(int min, int max, const IMatcher::Ptr obj)`

Array with minimum and maximum size and each element like the template object. Will generate an example array with one value.

##### `ConstrainedArrayLike(int min, int max, int examples, const IMatcher::Ptr obj)`

Array with minimum and maximum size and each element like the template object. `examples` defines how many examples should
be generated in the example array.

##### `EqualTo(int value)`

Match by equality. This is mainly used to reset the cascading type matchers.

##### `EqualTo(long value)`

Match by equality. This is mainly used to reset the cascading type matchers.

##### `EqualTo(float value)`

Match by equality. This is mainly used to reset the cascading type matchers.

##### `EqualTo(double value)`

Match by equality. This is mainly used to reset the cascading type matchers.

##### `EqualTo(std::string value)`

Match by equality. This is mainly used to reset the cascading type matchers.

##### `EqualTo(const char *value)`

Match by equality. This is mainly used to reset the cascading type matchers.

##### `EqualTo(bool value)`

Match by equality. This is mainly used to reset the cascading type matchers.

##### `IncludesStr(std::string value)`

Matches if the string value contains the given value.

##### `NullValue()`

Matches a JSON null value.

##### `Url(std::string basePath, std::vector<IMatcher::Ptr> pathFragments)`

Matches a URL composed of a base path and a list of path fragments. The path fragments can be matched using the either `EqualTo` or `Matching` matchers.

for example:

```cpp
Object({ "url", Url("http://localhost/tasks", { EqualTo("1001"), Matching("\\d+", "200") }) })
```

This will generate 

```json
{
  "url": "http://localhost/tasks/1001/200"
}
```

and configure a regex matcher to match `.*\/(1001\/\d+)$`

##### `Url(std::vector<IMatcher::Ptr> pathFragments)`

Matches a URL composed of a base path and a list of path fragments. The path fragments can be matched using the either `EqualTo` or `Matching` matchers. The base path from the running mock server will be used for example
URL values.

##### `ArrayContaining(std::vector<IMatcher::Ptr> variants)`

Matches the items in an array against a number of variants. Matching is successful if each variant 
 occurs once in the array. Variants may be objects containing matching rules.

for example:

```cpp
ArrayContaining({ Object({ { "id", Integer(3) } }), Matching("\\d+", "100") })
```

This will generate two variants, one to match an object with a integer `id` attribute and one to match a string against the regular expression.

### Execute the test

The actual test is then run with the `run_test` method. This takes a clsoure to execute and will startup a mock server before and the validate the result afterwards.

For example:

```cpp
auto result = provider.run_test([] (auto mock_server) {
    // setup any client code you have. The mock_server parameter has the base address of the mock server started for the test
    TodoClient todo;
    todo.serverUrl = mock_server->get_url();

    std::vector<Project> projects = todo.getProjects();

    return true; // return a boolean value here to indicate if all is OK
});
```

The `run_test` method will return a `PactTestResult`. Your test should check that this value is ok.

## Pact specification V4: messages and plugins

By default, pacts are written using whichever specification version `pact_ffi` defaults to. To use V4-only
features such as async/sync messages or plugins, opt in with `withSpecification`:

```cpp
auto provider = Pact("TodoAppCpp", "TodoServiceCpp");
provider.withSpecification(PactSpecification_V4);
```

### Asynchronous messages

Use `newMessage` to describe a message a consumer expects to receive (e.g. from a message queue):

```cpp
provider
  .newMessage("a project created event")
  .given("i have a list of projects")
  .withMetadata("contentType", "application/json")
  .withJsonBody(Object({
    { "id", Integer(1001) },
    { "name", Like("Home Chores") }
  }));
```

### Synchronous messages

Use `newSyncMessage` for request/response style messaging. The request is configured with `withBody`/`withJsonBody`
and the response with `withResponseBody`/`withResponseJsonBody`, same as HTTP interactions. Set the response body
before any `withResponseMetadata` call: the response part only exists once a body has been added, and metadata set
earlier is dropped.

```cpp
provider
  .newSyncMessage("a request for a project by id")
  .given("i have a list of projects")
  .withBody("{\"id\": 1001}", "application/json")
  .withResponseBody("{\"id\": 1001, \"name\": \"Home Chores\"}", "application/json")
  .withResponseMetadata("contentType", "application/json");
```

### Running message tests

Message and plugin interactions don't need an HTTP mock server, so use `run_message_test` instead of `run_test`.
It invokes your callback and, if it returns `true`, writes the pact file:

```cpp
auto result = provider.run_message_test([] {
  // invoke your message handler/consumer code here
  return true;
});
EXPECT_TRUE(result.is_ok()) << "Test failed";
```

### Plugins (e.g. gRPC via the protobuf plugin)

Load a plugin on the `Pact` with `usingPlugin`, then configure the interaction's request/response using
`withPluginContents`/`withResponsePluginContents`, passing the plugin-specific JSON configuration (refer to the
plugin's own documentation for the expected format). Call `cleanupPlugins` once you're done to shut down the
plugin process:

```cpp
provider.withSpecification(PactSpecification_V4);
provider.usingPlugin("protobuf", "0.8.0");

provider
  .newSyncMessage("a gRPC request")
  .withPluginContents("application/protobuf", protobufInteractionJson);

auto result = provider.run_test("grpc", [](const auto* mock_server) {
  // Point the gRPC client at 127.0.0.1:mock_server->get_port().
  return true;
});
provider.cleanupPlugins();
```

For a gRPC service call, include both the `request` and `response` objects in
the JSON passed to `withPluginContents`. The protobuf plugin owns the gRPC mock
transport, so use `run_test("grpc", ...)`; reserve `run_message_test` for
plugin interactions that do not start a transport.

## Using the Conan package

Releases are published to the Pact Foundation Artifactory as a Conan 2 package.
Add the remote, then depend on `pact_cpp_consumer/0.2.0`:

```console
$ conan remote add pact-foundation https://pactfoundation.jfrog.io/artifactory/api/conan/pactfoundation-conan
$ conan search "pact_cpp_consumer/*" -r=pact-foundation
```

The package bundles the `pact_ffi` headers and library for its platform, so a
consumer project needs only `requires = "pact_cpp_consumer/0.2.0"` and
`find_package(pact_cpp_consumer CONFIG REQUIRED)`; see
[test_package](test_package) for a minimal project.

## Using the released artifacts

The compiled static library and header files are attached to each GitHub
release. Link the library into your test binary together with `pact_ffi`
(downloaded from the
[pact-reference releases](https://github.com/pact-foundation/pact-reference/releases))
and make the Boost and nlohmann_json headers available; only
`boost/throw_exception.hpp` is used, so no compiled Boost libraries are needed.

## Building the library

Requires CMake 3.16+, a C++17 compiler and the `pact_ffi` binaries for your
platform. `PACT_FFI_ROOT` must point at a directory holding `include/pact.h` and
the `lib/libpact_ffi-<os>-<arch>.*` files; the build fails at configure time
without it.

Without Conan, point `Boost_ROOT` at an include directory that contains
`boost/` (only headers are used); nlohmann_json is fetched by CMake:

```console
cmake -S consumer -B build \
  -DPACT_FFI_ROOT=/path/to/libpact_ffi-v0.5.6 \
  -DPACT_FFI_VERSION=v0.5.6 \
  -DBoost_ROOT=/opt/homebrew/opt/boost/include
cmake --build build --config Release
cmake --install build --prefix ./install
```

With the dependencies supplied by Conan 2:

```console
cd consumer
export PACT_FFI_ROOT=/path/to/libpact_ffi-v0.5.6
conan install conanfile.py --build=missing -s build_type=Release -s compiler.cppstd=17
cmake --preset conan-release -DPactUseConan=ON -DPactBuildTests=ON \
  -DPACT_FFI_VERSION=v0.5.6 -DPACT_FFI_ROOT="$PACT_FFI_ROOT"
cmake --build --preset conan-release
ctest --preset conan-release --output-on-failure
```

`build.sh --project=consumer --os=<linux|mac> --test` at the repository root
runs the Conan flow end to end, downloading `pact_ffi` first if it is missing.

## Dependencies

### Ubuntu

```
sudo apt install build-essential python3 python3-pip libbz2-dev libz-dev liblzma-dev libicu-dev libboost-all-dev cmake git
```

### Alpine

```
apk add build-base cmake curl python3 py3-pip bzip2-dev zlib-dev xz-dev openssl gzip linux-headers perl bash automake autoconf libtool m4
```

### macOS

```
brew install cmake boost conan
```

### Windows

Install CMake, Visual Studio with the C++ workload, and either Conan or a Boost
distribution to point `Boost_ROOT` at. See [build.ps1](../build.ps1) for the
invocation the release build uses.
