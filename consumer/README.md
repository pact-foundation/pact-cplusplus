# C++ Consumer DSL

This is a shared library (pact-cpp-consumer) that you can link into your test binaries and it provides a DSL for setting up and running a consumer test. It wraps the Pact-Rust [pact_mock_server_ffi](https://github.com/pact-foundation/pact-reference/tree/master/rust/pact_mock_server_ffi) library, which will need to be linked as well.

## Using the DSL

Documentation is [here in markdown](docs/dsl.md) and [in html](docs/html/index.html). For an example, see [consumer_test.cpp](test/src/consumer_test.cpp).

### Setup the expectations 

Firstly, you need to instantiate an instance of `pact_consumer::Pact` in your test. This class takes the name of the consumer
and provider as parameters, and can then be used to define the expectations.

For instance:

```c++
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

.h5 `Object(const std::unordered_map<std::string, IMatcher::Ptr> fields)`

Represents a JSON object (map of string fields to matchers).

.h5 `Integer(long value)`

Matchers an integer value (must be a number and have no decimal places).

.h5 `Integer(int value)`

Matchers an integer value (must be a number and have no decimal places).

.h5 `Integer()`

Matchers an integer value (must be a number and have no decimal places). A random example value will be generated.

.h5 `Like(long value)`

Matchers any value based on the value type.

.h5 `Like(int value)`

Matchers any value based on the value type.

.h5 `Like(double value)`

Matchers any value based on the value type.

.h5 `Like(std::string value)`

Matchers any value based on the value type.

.h5 `Like(const char *value)`

Matchers any value based on the value type.

.h5 `Like(bool value)`

Matchers any value based on the value type.

.h5 `DateTime(std::string format, std::string example)`

String value that must match the provided datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html).

.h5 `DateTime(std::string format)`

String value that must match the provided datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html). Example values will be generated using the current system date and time.

.h5 `Date(std::string format, std::string example)`

String value that must match the provided date format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html).

.h5 `Date(std::string format)`

String value that must match the provided date format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html). Example values will be generated using the current system date and time.


.h5 `Time(std::string format, std::string example)`

String value that must match the provided time format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html).

.h5 `Time(std::string format);

String value that must match the provided time format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html). Example values will be generated using the current system date and time.


.h5 `Matching(std::string regex, std::string example)`

String value that must match the regular expression.

.h5 `Matching(std::string regex)`

String value that must match the regular expression. Random values will be generated for the examples.

.h5 `EachLike(const IMatcher::Ptr obj)`

Array of values where each item in the array must match the provided template. Will generate an example array with one value.

.h5 `EachLike(int examples, const IMatcher::Ptr obj)`

Array of values where each item in the array must match the provided template. `examples` defines how many examples should
be generated in the example array.

.h5 `AtLeastOneLike(const IMatcher::Ptr obj)`

Array of values that must have at least one value and each item in the array must match the provided template. Will generate an example array with one value.

.h5 `AtLeastOneLike(int examples, const IMatcher::Ptr obj)`

Array of values that must have at least one value and each item in the array must match the provided template.`examples` defines how many examples should be generated in the example array.

.h5 `HexValue(const std::string example)`

Match a hexadecimal string value.

.h5 `HexValue()`

Match a hexadecimal string value. Random example will be generated.

.h5 `IPAddress(const std::string example)`

Match an IP Address.

.h5 `IPAddress()`

Match an IP Address. Will use 127.0.0.1 for examples.

.h5 `Numeric(int example)`

Match any numeric value. This will match any numeric type (integer or floating point).

.h5 `Numeric(long example)`

Match any numeric value. This will match any numeric type (integer or floating point).

.h5 `Numeric(float example)`

Match any numeric value. This will match any numeric type (integer or floating point).

.h5 `Numeric(double example)`

Match any numeric value. This will match any numeric type (integer or floating point).

.h5 `Numeric()`

Match any numeric value. Random decimal values will be generated for examples.

.h5 `Decimal(float example)`

Match a decimal value (number with atleast one digit after the decimal point).

.h5 `Decimal(double example)`

Match a decimal value (number with atleast one digit after the decimal point).

.h5 `Decimal()`

Match a decimal value (number with atleast one digit after the decimal point). Random values will be generated.

.h5 `Uuid(const std::string example)`

Match a universally unique identifier (UUID).

.h5 `Uuid()`

Match a universally unique identifier (UUID). Random values will be used for examples.

.h5 `AtMostLike(int max, const IMatcher::Ptr obj)`

Array with maximum size and each element like the template object. Will generate an example array with one value.

.h5 `AtMostLike(int max, int examples, const IMatcher::Ptr obj)`

Array with maximum size and each element like the template object. `examples` defines how many examples should
be generated in the example array.

.h5 `MinArrayLike(int min, const IMatcher::Ptr obj)`

Array with minimum size and each element like the template object. Will generate an example array with one value.

.h5 `MinArrayLike(int min, int examples, const IMatcher::Ptr obj)`

Array with minimum size and each element like the template object. `examples` defines how many examples should
be generated in the example array.

.h5 `ConstrainedArrayLike(int min, int max, const IMatcher::Ptr obj)`

Array with minimum and maximum size and each element like the template object. Will generate an example array with one value.

.h5 `ConstrainedArrayLike(int min, int max, int examples, const IMatcher::Ptr obj)`

Array with minimum and maximum size and each element like the template object. `examples` defines how many examples should
be generated in the example array.

.h5 `EqualTo(int value)`

Match by equality. This is mainly used to reset the cascading type matchers.

.h5 `EqualTo(long value)`

Match by equality. This is mainly used to reset the cascading type matchers.

.h5 `EqualTo(float value)`

Match by equality. This is mainly used to reset the cascading type matchers.

.h5 `EqualTo(double value)`

Match by equality. This is mainly used to reset the cascading type matchers.

.h5 `EqualTo(std::string value)`

Match by equality. This is mainly used to reset the cascading type matchers.

.h5 `EqualTo(bool value)`

Match by equality. This is mainly used to reset the cascading type matchers.

.h5 `IncludesStr(std::string value)`

Matches if the string value contains the given value.

.h5 `NullValue()`

Matches a JSON null value.

.h5 `Url(std::string basePath, std::vector<IMatcher::Ptr> pathFragments)`

Matches a URL composed of a base path and a list of path fragments. The path fragments can be matched using the either `EqualTo` or `Matching` matchers.

for example:

```c++
Object({ "url", Url("http://localhost/tasks", { EqualTo("1001"), Matching("\\d+", "200") }) })
```

This will generate 

```json
{
  "url": "http://localhost/tasks/1001/200"
}
```

and configure a regex matcher to match `.*1001/\Q\d+\E$`

### Execute the test

The actual test is then run with the `run_test` method. This takes a clsoure to execute and will startup a mock server before and the validate the result afterwards.

For example:

```c++
auto result = provider.run_test([] (auto mock_server) {
    // setup any client code you have. The mock_server parameter has the base address of the mock server started for the test
    TodoClient todo;
    todo.serverUrl = mock_server->get_url();

    std::vector<Project> projects = todo.getProjects();

    return true; // return a boolean value here to indicate if all is OK
});
```

The `run_test` method will return a `PactTestResult`. Your test should check that this value is ok.

## Using the Conan package

The library has been released to Bintray as a Conan package. To use it in your project, you need to add https://api.bintray.com/conan/pact-foundation/conan as a remote.

For example:

```console
$ conan remote add pact-foundation https://api.bintray.com/conan/pact-foundation/conan
$ conan search pact_cpp_consumer -r=pact-foundation
Existing package recipes:

pact_cpp_consumer/0.0.1@pact/beta
```

You can then use the library by adding `pact_cpp_consumer/0.0.1@pact/beta` to the dependencies to your project conan file.

## Building the library

To build the library, you need to do the following:

1. Install Python
2. Install CMake
3. Install a C++ compiler
4. Use python/pip to install conan

```
pip install wheel setuptools
pip install conan
```

5. Create a build directory and cd into it
6. use conan to install all the dependencies

```
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan remote add pact-foundation https://api.bintray.com/conan/pact-foundation/conan
conan install .. --build missing
```

7. use cmake to build the library

```
 cmake ..
 cmake --build . --config Release
```

On Windows, you may need to set 64 bit architecture.

```
cmake .. -A x64
cmake --build . --config Release
```
