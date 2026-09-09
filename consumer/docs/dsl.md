# Pact C++ Consumer Reference

## Namespaces

| Name | Description |
|------|-------------|
| [`pact_consumer`](#pact_consumer) |  |
| [`matchers`](#matchers) |  |

## Typedefs

---

### json

```cpp
using json = nlohmann::json
```

Defined in src/consumer.cpp:9



## pact_consumer

### Classes

| Name | Description |
|------|-------------|
| [`MockServerHandle`](#mockserverhandle) | Mock server handle to the mock server started for the test |
| [`PactTestResult`](#pacttestresult) | Result of the [Pact](#pact) test run |
| [`Pact`](#pact) | Class that defines a [Pact](#pact) between a consumer and provider |
| [`Interaction`](#interaction) | Class that defines an interaction between a consumer and provider |

### Enumerations

| Name | Description |
|------|-------------|
| [`InteractionType`](#interactiontype)  | The kind of interaction to create, controlling which pact_ffi constructor is used. |
| [`TestResultState`](#testresultstate)  | Type of [Pact](#pact) test result |

---

#### InteractionType

```cpp
enum InteractionType
```

Defined in include/consumer.h:26

The kind of interaction to create, controlling which pact_ffi constructor is used.

| Value | Description |
|-------|-------------|
| `Http` |  |
| `Message` |  |
| `SyncMessage` |  |

---

#### TestResultState

```cpp
enum TestResultState
```

Defined in include/consumer.h:62

Type of [Pact](#pact) test result

| Value | Description |
|-------|-------------|
| `Mismatches` |  |
| `UserCodeFailed` |  |
| `PactFileError` |  |
| `MockServerFailed` |  |
### Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init)  | Initilise the pact test library |

---

#### init

```cpp
void init()
```

Defined in src/consumer.cpp:12

Initilise the pact test library



## MockServerHandle

```cpp
#include <consumer.h>
```

```cpp
class MockServerHandle
```

Defined in include/consumer.h:35

Mock server handle to the mock server started for the test

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`MockServerHandle`](#mockserverhandle-1)  |  |
|  | [`~MockServerHandle`](#mockserverhandle-2)  |  |
| `bool` | [`started_ok`](#started_ok) `const` | Indicates the mock server was started ok |
| `std::string` | [`get_url`](#get_url) `const` | The URL to the mock server |
| `int32_t` | [`get_port`](#get_port) `const` | The port the mock server is running on |

---

#### MockServerHandle

```cpp
MockServerHandle(PactHandle)
```

Defined in include/consumer.h:37

---

#### ~MockServerHandle

```cpp
~MockServerHandle()
```

Defined in include/consumer.h:38

---

#### started_ok

`const`

```cpp
bool started_ok() const
```

Defined in include/consumer.h:43

Indicates the mock server was started ok

---

#### get_url

`const`

```cpp
std::string get_url() const
```

Defined in include/consumer.h:48

The URL to the mock server

---

#### get_port

`const`

```cpp
int32_t get_port() const
```

Defined in include/consumer.h:53

The port the mock server is running on

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int32_t` | [`port`](#port)  |  |

---

#### port

```cpp
int32_t port
```

Defined in include/consumer.h:56



## PactTestResult

```cpp
#include <consumer.h>
```

```cpp
class PactTestResult
```

Defined in include/consumer.h:72

Result of the [Pact](#pact) test run

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PactTestResult`](#pacttestresult-1)  |  |
| `void` | [`add_state`](#add_state)  | Adds a test state to the result |
| `void` | [`add_state`](#add_state-1)  | Adds a test state with a message to the result |
| `void` | [`add_state`](#add_state-2)  | Adds a test state with a message to the result |
| `bool` | [`is_ok`](#is_ok) `const` | If there are no mismatches and the user code did not fail |
| `void` | [`display_errors`](#display_errors)  | Logs all errors out |

---

#### PactTestResult

```cpp
PactTestResult()
```

Defined in include/consumer.h:74

---

#### add_state

```cpp
void add_state(TestResultState state)
```

Defined in include/consumer.h:79

Adds a test state to the result

---

#### add_state

```cpp
void add_state(TestResultState state, std::string message)
```

Defined in include/consumer.h:84

Adds a test state with a message to the result

---

#### add_state

```cpp
void add_state(TestResultState state, std::string message, std::string ex)
```

Defined in include/consumer.h:89

Adds a test state with a message to the result

---

#### is_ok

`const`

```cpp
bool is_ok() const
```

Defined in include/consumer.h:94

If there are no mismatches and the user code did not fail

---

#### display_errors

```cpp
void display_errors()
```

Defined in include/consumer.h:99

Logs all errors out

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `unsigned int` | [`status`](#status)  |  |
| `std::vector< std::string >` | [`messages`](#messages)  |  |
| `std::optional< std::string >` | [`ex`](#ex)  |  |

---

#### status

```cpp
unsigned int status = 0
```

Defined in include/consumer.h:102

---

#### messages

```cpp
std::vector< std::string > messages
```

Defined in include/consumer.h:103

---

#### ex

```cpp
std::optional< std::string > ex
```

Defined in include/consumer.h:104



## Pact

```cpp
#include <consumer.h>
```

```cpp
class Pact
```

Defined in include/consumer.h:110

Class that defines a [Pact](#pact) between a consumer and provider

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PactHandle` | [`pact`](#pact-1)  |  |
| `std::string` | [`pact_directory`](#pact_directory)  | Directory to write pact files to |

---

#### pact

```cpp
PactHandle pact
```

Defined in include/consumer.h:172

---

#### pact_directory

```cpp
std::string pact_directory
```

Defined in include/consumer.h:177

Directory to write pact files to

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Pact`](#pact-2)  | Constructs the [Pact](#pact) given the consumer and provider names |
| [`Interaction`](#interaction) | [`given`](#given) `const` | Creates a new iteraction with a defined provider state |
| [`Interaction`](#interaction) | [`given`](#given-1) `const` | Creates a new iteraction with a defined provider state and parameters |
| [`Interaction`](#interaction) | [`uponReceiving`](#uponreceiving) `const` | Creates a new interaction with the provided description. |
| `void` | [`withSpecification`](#withspecification) `const` | Sets the [Pact](#pact) specification version to generate the pact file for (V1 - V4). Required for V4-only features such as sync/async messages and plugins. |
| `bool` | [`usingPlugin`](#usingplugin) `const` | Adds a plugin (e.g. protobuf/gRPC) to be used by the test. The plugin must be installed for this to succeed. Returns true on success. |
| `void` | [`cleanupPlugins`](#cleanupplugins) `const` | Shuts down any plugins that are no longer required by this [Pact](#pact). |
| [`Interaction`](#interaction) | [`newMessage`](#newmessage) `const` | Creates a new V4 asynchronous message interaction with the provided description. |
| [`Interaction`](#interaction) | [`newSyncMessage`](#newsyncmessage) `const` | Creates a new V4 synchronous (request/response) message interaction with the provided description. |
| [`PactTestResult`](#pacttestresult) | [`run_test`](#run_test) `const` | Starts a mock server for this pact, and then passes it to the callback. The callback needs to return a boolean value to indicate of the test was successful. |
| [`PactTestResult`](#pacttestresult) | [`run_message_test`](#run_message_test) `const` | Runs a message/plugin test that doesn't require a mock server (e.g. async/sync messages, or plugin-driven interactions such as gRPC). The callback needs to return a boolean value to indicate if the test was successful, after which the pact file is written. |

---

#### Pact

```cpp
Pact(const char * consumer, const char * provider)
```

Defined in include/consumer.h:115

Constructs the [Pact](#pact) given the consumer and provider names

---

#### given

`const`

```cpp
Interaction given(const char * provider_state) const
```

Defined in include/consumer.h:120

Creates a new iteraction with a defined provider state

---

#### given

`const`

```cpp
Interaction given(const char * provider_state, const std::unordered_map< std::string, std::string > & parameters) const
```

Defined in include/consumer.h:125

Creates a new iteraction with a defined provider state and parameters

---

#### uponReceiving

`const`

```cpp
Interaction uponReceiving(const char * description) const
```

Defined in include/consumer.h:130

Creates a new interaction with the provided description.

---

#### withSpecification

`const`

```cpp
void withSpecification(PactSpecification version) const
```

Defined in include/consumer.h:136

Sets the [Pact](#pact) specification version to generate the pact file for (V1 - V4). Required for V4-only features such as sync/async messages and plugins.

---

#### usingPlugin

`const`

```cpp
bool usingPlugin(const std::string & plugin_name, const std::string & plugin_version = "") const
```

Defined in include/consumer.h:142

Adds a plugin (e.g. protobuf/gRPC) to be used by the test. The plugin must be installed for this to succeed. Returns true on success.

---

#### cleanupPlugins

`const`

```cpp
void cleanupPlugins() const
```

Defined in include/consumer.h:147

Shuts down any plugins that are no longer required by this [Pact](#pact).

---

#### newMessage

`const`

```cpp
Interaction newMessage(const char * description) const
```

Defined in include/consumer.h:152

Creates a new V4 asynchronous message interaction with the provided description.

---

#### newSyncMessage

`const`

```cpp
Interaction newSyncMessage(const char * description) const
```

Defined in include/consumer.h:157

Creates a new V4 synchronous (request/response) message interaction with the provided description.

---

#### run_test

`const`

```cpp
PactTestResult run_test(std::function< bool(const MockServerHandle *)> callback) const
```

Defined in include/consumer.h:163

Starts a mock server for this pact, and then passes it to the callback. The callback needs to return a boolean value to indicate of the test was successful.

---

#### run_message_test

`const`

```cpp
PactTestResult run_message_test(std::function< bool()> callback) const
```

Defined in include/consumer.h:170

Runs a message/plugin test that doesn't require a mock server (e.g. async/sync messages, or plugin-driven interactions such as gRPC). The callback needs to return a boolean value to indicate if the test was successful, after which the pact file is written.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`consumer`](#consumer)  |  |
| `std::string` | [`provider`](#provider)  |  |

---

#### consumer

```cpp
std::string consumer
```

Defined in include/consumer.h:180

---

#### provider

```cpp
std::string provider
```

Defined in include/consumer.h:181



## Interaction

```cpp
#include <consumer.h>
```

```cpp
class Interaction
```

Defined in include/consumer.h:187

Class that defines an interaction between a consumer and provider

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `InteractionHandle` | [`interaction`](#interaction-1)  |  |

---

#### interaction

```cpp
InteractionHandle interaction
```

Defined in include/consumer.h:298

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Interaction`](#interaction-2)  |  |
| [`Interaction`](#interaction-2) | [`given`](#given-2) `const` | Adds the provider state to the interaction |
| [`Interaction`](#interaction-2) | [`given`](#given-3) `const` | Adds a provider state with parameters |
| [`Interaction`](#interaction-2) | [`uponReceiving`](#uponreceiving-1) `const` | Sets the description for the interaction |
| [`Interaction`](#interaction-2) | [`withRequest`](#withrequest) `const` | Sets the method and path for the request |
| [`Interaction`](#interaction-2) | [`withQuery`](#withquery) `const` | Sets the query parameters for the request |
| [`Interaction`](#interaction-2) | [`withHeaders`](#withheaders) `const` | Sets the headers for the request |
| [`Interaction`](#interaction-2) | [`withBody`](#withbody) `const` | Sets the body for the request to the string contents. |
| [`Interaction`](#interaction-2) | [`withJsonBody`](#withjsonbody) `const` | Sets the body for the request using the provided body template. |
| [`Interaction`](#interaction-2) | [`withMetadata`](#withmetadata) `const` | Sets metadata on a message (or the request part of a synchronous message), such as the queue name or message type. Only relevant for message interactions. |
| [`Interaction`](#interaction-2) | [`withResponseMetadata`](#withresponsemetadata) `const` | Sets metadata on the response part of a synchronous message. |
| [`Interaction`](#interaction-2) | [`withPluginContents`](#withplugincontents) `const` | Configures the request part of the interaction using a plugin (e.g. protobuf/gRPC). The contents is a JSON string passed on to the plugin to configure the interaction; refer to the plugin documentation for the expected format. |
| [`Interaction`](#interaction-2) | [`withResponsePluginContents`](#withresponseplugincontents) `const` | Configures the response part of the interaction using a plugin (e.g. protobuf/gRPC). |
| [`Interaction`](#interaction-2) | [`withBinaryFile`](#withbinaryfile) `const` | Sets the body for the request using the example file and content type. Note that this will attempt to load the entire example file in memory. Use small files for your testing. |
| [`Interaction`](#interaction-2) | [`withMultipartFileUpload`](#withmultipartfileupload) `const` | Sets the body for the request as a MIME multipart body using the example file and content type. Note that this will attempt to load the entire example file in memory. Use small files for your testing. |
| [`Interaction`](#interaction-2) | [`willRespondWith`](#willrespondwith) `const` | Sets the status code for the response |
| [`Interaction`](#interaction-2) | [`withResponseHeaders`](#withresponseheaders) `const` | Sets the headers for the response |
| [`Interaction`](#interaction-2) | [`withResponseBody`](#withresponsebody) `const` | Sets the body for the response to the string contents. |
| [`Interaction`](#interaction-2) | [`withResponseJsonBody`](#withresponsejsonbody) `const` | Sets the body for the request using the provided body template |
| [`Interaction`](#interaction-2) | [`withResponseBinaryFile`](#withresponsebinaryfile) `const` | Sets the body for the response using the example file and content type. Note that this will attempt to load the entire example file in memory. Use small files for your testing. |
| [`Interaction`](#interaction-2) | [`withResponseMultipartFileUpload`](#withresponsemultipartfileupload) `const` | Sets the body for the response as a MIME multipart body using the example file and content type. Note that this will attempt to load the entire example file in memory. Use small files for your testing. |

---

#### Interaction

```cpp
Interaction(const Pact * parent, const char * description, InteractionType type = InteractionType::Http)
```

Defined in include/consumer.h:189

---

#### given

`const`

```cpp
Interaction given(const char * provider_state) const
```

Defined in include/consumer.h:194

Adds the provider state to the interaction

---

#### given

`const`

```cpp
Interaction given(const char * provider_state, const std::unordered_map< std::string, std::string > & parameters) const
```

Defined in include/consumer.h:199

Adds a provider state with parameters

---

#### uponReceiving

`const`

```cpp
Interaction uponReceiving(const char * description) const
```

Defined in include/consumer.h:204

Sets the description for the interaction

---

#### withRequest

`const`

```cpp
Interaction withRequest(const char * method, const char * path) const
```

Defined in include/consumer.h:209

Sets the method and path for the request

---

#### withQuery

`const`

```cpp
Interaction withQuery(const std::unordered_map< std::string, std::vector< std::string > > & query) const
```

Defined in include/consumer.h:214

Sets the query parameters for the request

---

#### withHeaders

`const`

```cpp
Interaction withHeaders(const std::unordered_map< std::string, std::vector< std::string > > & headers) const
```

Defined in include/consumer.h:219

Sets the headers for the request

---

#### withBody

`const`

```cpp
Interaction withBody(const std::string & body, const std::string & content_type) const
```

Defined in include/consumer.h:224

Sets the body for the request to the string contents.

---

#### withJsonBody

`const`

```cpp
Interaction withJsonBody(pact_consumer::matchers::IMatcher::Ptr body) const
```

Defined in include/consumer.h:229

Sets the body for the request using the provided body template.

---

#### withMetadata

`const`

```cpp
Interaction withMetadata(const std::string & key, const std::string & value) const
```

Defined in include/consumer.h:235

Sets metadata on a message (or the request part of a synchronous message), such as the queue name or message type. Only relevant for message interactions.

---

#### withResponseMetadata

`const`

```cpp
Interaction withResponseMetadata(const std::string & key, const std::string & value) const
```

Defined in include/consumer.h:240

Sets metadata on the response part of a synchronous message.

---

#### withPluginContents

`const`

```cpp
Interaction withPluginContents(const std::string & content_type, const std::string & contents) const
```

Defined in include/consumer.h:247

Configures the request part of the interaction using a plugin (e.g. protobuf/gRPC). The contents is a JSON string passed on to the plugin to configure the interaction; refer to the plugin documentation for the expected format.

---

#### withResponsePluginContents

`const`

```cpp
Interaction withResponsePluginContents(const std::string & content_type, const std::string & contents) const
```

Defined in include/consumer.h:252

Configures the response part of the interaction using a plugin (e.g. protobuf/gRPC).

---

#### withBinaryFile

`const`

```cpp
Interaction withBinaryFile(const std::string & content_type, const std::filesystem::path & example_file) const
```

Defined in include/consumer.h:258

Sets the body for the request using the example file and content type. Note that this will attempt to load the entire example file in memory. Use small files for your testing.

---

#### withMultipartFileUpload

`const`

```cpp
Interaction withMultipartFileUpload(const std::string & part_name, const std::string & content_type, const std::filesystem::path & example_file) const
```

Defined in include/consumer.h:264

Sets the body for the request as a MIME multipart body using the example file and content type. Note that this will attempt to load the entire example file in memory. Use small files for your testing.

---

#### willRespondWith

`const`

```cpp
Interaction willRespondWith(size_t status) const
```

Defined in include/consumer.h:269

Sets the status code for the response

---

#### withResponseHeaders

`const`

```cpp
Interaction withResponseHeaders(const std::unordered_map< std::string, std::vector< std::string > > & headers) const
```

Defined in include/consumer.h:274

Sets the headers for the response

---

#### withResponseBody

`const`

```cpp
Interaction withResponseBody(const std::string & body, const std::string & content_type) const
```

Defined in include/consumer.h:279

Sets the body for the response to the string contents.

---

#### withResponseJsonBody

`const`

```cpp
Interaction withResponseJsonBody(pact_consumer::matchers::IMatcher::Ptr body) const
```

Defined in include/consumer.h:284

Sets the body for the request using the provided body template

---

#### withResponseBinaryFile

`const`

```cpp
Interaction withResponseBinaryFile(const std::string & content_type, const std::filesystem::path & example_file) const
```

Defined in include/consumer.h:290

Sets the body for the response using the example file and content type. Note that this will attempt to load the entire example file in memory. Use small files for your testing.

---

#### withResponseMultipartFileUpload

`const`

```cpp
Interaction withResponseMultipartFileUpload(const std::string & part_name, const std::string & content_type, const std::filesystem::path & example_file) const
```

Defined in include/consumer.h:296

Sets the body for the response as a MIME multipart body using the example file and content type. Note that this will attempt to load the entire example file in memory. Use small files for your testing.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`description`](#description)  |  |
| const [`Pact`](#pact) * | [`pact`](#pact-3)  |  |

---

#### description

```cpp
std::string description
```

Defined in include/consumer.h:301

---

#### pact

```cpp
const Pact * pact
```

Type: const [`Pact`](#pact) *

Defined in include/consumer.h:302



## matchers

### Classes

| Name | Description |
|------|-------------|
| [`IMatcher`](#imatcher) |  |
| [`ObjectMatcher`](#objectmatcher) |  |
| [`IntegerMatcher`](#integermatcher) |  |
| [`DecimalMatcher`](#decimalmatcher) |  |
| [`TypeMatcher`](#typematcher) |  |
| [`NumberMatcher`](#numbermatcher) |  |
| [`EqualsMatcher`](#equalsmatcher) |  |
| [`DateTimeMatcher`](#datetimematcher) |  |
| [`DateMatcher`](#datematcher) |  |
| [`TimeMatcher`](#timematcher) |  |
| [`RegexMatcher`](#regexmatcher) |  |
| [`EachlikeMatcher`](#eachlikematcher) |  |
| [`ArrayContainsMatcher`](#arraycontainsmatcher) |  |
| [`HexadecimalMatcher`](#hexadecimalmatcher) |  |
| [`IPAddressMatcher`](#ipaddressmatcher) |  |
| [`UuidMatcher`](#uuidmatcher) |  |
| [`IncludesMatcher`](#includesmatcher) |  |
| [`NullMatcher`](#nullmatcher) |  |
| [`UrlMatcher`](#urlmatcher) |  |

### Functions

| Return | Name | Description |
|--------|------|-------------|
| [`IMatcher::Ptr`](#ptr) | [`Object`](#object)  | Represents a JSON object (map of string fields) |
| [`IMatcher::Ptr`](#ptr) | [`Integer`](#integer)  | Matchers an integer value (must be a number and have no decimal places). |
| [`IMatcher::Ptr`](#ptr) | [`Integer`](#integer-1)  | Matchers an integer value (must be a number and have no decimal places). |
| [`IMatcher::Ptr`](#ptr) | [`Integer`](#integer-2)  | Matchers an integer value (must be a number and have no decimal places). A random example value will be generated. |
| [`IMatcher::Ptr`](#ptr) | [`Like`](#like)  | Matchers value based on the value type |
| [`IMatcher::Ptr`](#ptr) | [`Like`](#like-1)  | Matchers value based on the value type |
| [`IMatcher::Ptr`](#ptr) | [`Like`](#like-2)  | Matchers value based on the value type |
| [`IMatcher::Ptr`](#ptr) | [`Like`](#like-3)  | Matchers value based on the value type |
| [`IMatcher::Ptr`](#ptr) | [`Like`](#like-4)  | Matchers value based on the value type |
| [`IMatcher::Ptr`](#ptr) | [`Like`](#like-5)  | Matchers value based on the value type |
| [`IMatcher::Ptr`](#ptr) | [`DateTime`](#datetime)  | String value that must match the provided datetime format string. |
| [`IMatcher::Ptr`](#ptr) | [`DateTime`](#datetime-1)  | String value that must match the provided datetime format string. Example values will be generated using the current system date and time. |
| [`IMatcher::Ptr`](#ptr) | [`Date`](#date)  | String value that must match the provided date format string. |
| [`IMatcher::Ptr`](#ptr) | [`Date`](#date-1)  | String value that must match the provided date format string. Example values will be generated using the current system date and time. |
| [`IMatcher::Ptr`](#ptr) | [`Time`](#time)  | String value that must match the provided time format string. |
| [`IMatcher::Ptr`](#ptr) | [`Time`](#time-1)  | String value that must match the provided time format string. Example values will be generated using the current system date and time. |
| [`IMatcher::Ptr`](#ptr) | [`Matching`](#matching)  | String value that must match the regular expression |
| [`IMatcher::Ptr`](#ptr) | [`Matching`](#matching-1)  | String value that must match the regular expression. Random values will be generated for the examples. |
| [`IMatcher::Ptr`](#ptr) | [`EachLike`](#eachlike)  | Array of values where each item in the array must match the provided template. |
| [`IMatcher::Ptr`](#ptr) | [`EachLike`](#eachlike-1)  | Array of values where each item in the array must match the provided template. |
| [`IMatcher::Ptr`](#ptr) | [`AtLeastOneLike`](#atleastonelike)  | Array of values that must have at least one value and each item in the array must match the provided template. |
| [`IMatcher::Ptr`](#ptr) | [`AtLeastOneLike`](#atleastonelike-1)  | Array of values that must have at least one value and each item in the array must match the provided template. |
| [`IMatcher::Ptr`](#ptr) | [`HexValue`](#hexvalue)  | Match a hexadecimal value |
| [`IMatcher::Ptr`](#ptr) | [`HexValue`](#hexvalue-1)  | Match a hexadecimal value. Random examples will be generated. |
| [`IMatcher::Ptr`](#ptr) | [`IPAddress`](#ipaddress)  | Match an IP Address |
| [`IMatcher::Ptr`](#ptr) | [`IPAddress`](#ipaddress-1)  | Match an IP Address. Will use 127.0.0.1 for examples. |
| [`IMatcher::Ptr`](#ptr) | [`Numeric`](#numeric)  | Match a numeric value. This will match any numeric type (integer or floating point). |
| [`IMatcher::Ptr`](#ptr) | [`Numeric`](#numeric-1)  | Match a numeric value. This will match any numeric type (integer or floating point). |
| [`IMatcher::Ptr`](#ptr) | [`Numeric`](#numeric-2)  | Match a numeric value. This will match any numeric type (integer or floating point). |
| [`IMatcher::Ptr`](#ptr) | [`Numeric`](#numeric-3)  | Match a numeric value. This will match any numeric type (integer or floating point). |
| [`IMatcher::Ptr`](#ptr) | [`Numeric`](#numeric-4)  | Match a numeric value. Random decimal values will be generated for examples. |
| [`IMatcher::Ptr`](#ptr) | [`Decimal`](#decimal)  | Match a decimal value (number with atleast one digit after the decimal point) |
| [`IMatcher::Ptr`](#ptr) | [`Decimal`](#decimal-1)  | Match a decimal value (number with atleast one digit after the decimal point) |
| [`IMatcher::Ptr`](#ptr) | [`Decimal`](#decimal-2)  | Match a decimal value (number with atleast one digit after the decimal point). Random values will be generated for examples. |
| [`IMatcher::Ptr`](#ptr) | [`Uuid`](#uuid)  | Match a universally unique identifier (UUID) |
| [`IMatcher::Ptr`](#ptr) | [`Uuid`](#uuid-1)  | Match a universally unique identifier (UUID). Random values will be used for examples. |
| [`IMatcher::Ptr`](#ptr) | [`AtMostLike`](#atmostlike)  | Array with maximum size and each element like the template object. |
| [`IMatcher::Ptr`](#ptr) | [`AtMostLike`](#atmostlike-1)  | Array with maximum size and each element like the template object. |
| [`IMatcher::Ptr`](#ptr) | [`MinArrayLike`](#minarraylike)  | Array with minimum size and each element like the template object. |
| [`IMatcher::Ptr`](#ptr) | [`MinArrayLike`](#minarraylike-1)  | Array with minimum size and each element like the template object. |
| [`IMatcher::Ptr`](#ptr) | [`ConstrainedArrayLike`](#constrainedarraylike)  | Array with minimum and maximum size and each element like the template object. |
| [`IMatcher::Ptr`](#ptr) | [`ConstrainedArrayLike`](#constrainedarraylike-1)  | Array with minimum and maximum size and each element like the template object. |
| [`IMatcher::Ptr`](#ptr) | [`EqualTo`](#equalto)  | Match by equality. This is mainly used to reset the cascading type matchers. |
| [`IMatcher::Ptr`](#ptr) | [`EqualTo`](#equalto-1)  | Match by equality. This is mainly used to reset the cascading type matchers. |
| [`IMatcher::Ptr`](#ptr) | [`EqualTo`](#equalto-2)  | Match by equality. This is mainly used to reset the cascading type matchers. |
| [`IMatcher::Ptr`](#ptr) | [`EqualTo`](#equalto-3)  | Match by equality. This is mainly used to reset the cascading type matchers. |
| [`IMatcher::Ptr`](#ptr) | [`EqualTo`](#equalto-4)  | Match by equality. This is mainly used to reset the cascading type matchers. |
| [`IMatcher::Ptr`](#ptr) | [`EqualTo`](#equalto-5)  | Match by equality. This is mainly used to reset the cascading type matchers. |
| [`IMatcher::Ptr`](#ptr) | [`EqualTo`](#equalto-6)  | Match by equality. This is mainly used to reset the cascading type matchers. |
| [`IMatcher::Ptr`](#ptr) | [`IncludesStr`](#includesstr)  | Matches if the string value contains the given value |
| [`IMatcher::Ptr`](#ptr) | [`NullValue`](#nullvalue)  | Matches a JSON null value |
| [`IMatcher::Ptr`](#ptr) | [`Url`](#url)  | Matches a URL composed of a base path and a list of path fragments |
| [`IMatcher::Ptr`](#ptr) | [`Url`](#url-1)  | Matches a URL composed of a base path and a list of path fragments. The base URL from the mock server will be used. |
| [`IMatcher::Ptr`](#ptr) | [`ArrayContaining`](#arraycontaining)  | Matches the items in an array against a number of variants. Matching is successful if each variant occurs once in the array. Variants may be objects containing matching rules. |

---

#### Object

```cpp
IMatcher::Ptr Object(const std::unordered_map< std::string, IMatcher::Ptr > & fields)
```

Defined in src/matchers.cpp:12

Represents a JSON object (map of string fields)

---

#### Integer

```cpp
IMatcher::Ptr Integer(long value)
```

Defined in src/matchers.cpp:16

Matchers an integer value (must be a number and have no decimal places).

---

#### Integer

```cpp
IMatcher::Ptr Integer(int value)
```

Defined in src/matchers.cpp:20

Matchers an integer value (must be a number and have no decimal places).

---

#### Integer

```cpp
IMatcher::Ptr Integer()
```

Defined in src/matchers.cpp:24

Matchers an integer value (must be a number and have no decimal places). A random example value will be generated.

---

#### Like

```cpp
IMatcher::Ptr Like(long value)
```

Defined in src/matchers.cpp:28

Matchers value based on the value type

---

#### Like

```cpp
IMatcher::Ptr Like(int value)
```

Defined in src/matchers.cpp:32

Matchers value based on the value type

---

#### Like

```cpp
IMatcher::Ptr Like(double value)
```

Defined in src/matchers.cpp:36

Matchers value based on the value type

---

#### Like

```cpp
IMatcher::Ptr Like(const std::string & value)
```

Defined in src/matchers.cpp:40

Matchers value based on the value type

---

#### Like

```cpp
IMatcher::Ptr Like(const char * value)
```

Defined in src/matchers.cpp:44

Matchers value based on the value type

---

#### Like

```cpp
IMatcher::Ptr Like(bool value)
```

Defined in src/matchers.cpp:48

Matchers value based on the value type

---

#### DateTime

```cpp
IMatcher::Ptr DateTime(const std::string & format, const std::string & example)
```

Defined in src/matchers.cpp:52

String value that must match the provided datetime format string.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `format` | `const std::string &` | Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html) |
| `example` | `const std::string &` | Example value to use |

---

#### DateTime

```cpp
IMatcher::Ptr DateTime(const std::string & format)
```

Defined in src/matchers.cpp:56

String value that must match the provided datetime format string. Example values will be generated using the current system date and time.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `format` | `const std::string &` | Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html) |

---

#### Date

```cpp
IMatcher::Ptr Date(const std::string & format, const std::string & example)
```

Defined in src/matchers.cpp:60

String value that must match the provided date format string.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `format` | `const std::string &` | Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html) |
| `example` | `const std::string &` | Example value to use |

---

#### Date

```cpp
IMatcher::Ptr Date(const std::string & format)
```

Defined in src/matchers.cpp:64

String value that must match the provided date format string. Example values will be generated using the current system date and time.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `format` | `const std::string &` | Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html) |

---

#### Time

```cpp
IMatcher::Ptr Time(const std::string & format, const std::string & example)
```

Defined in src/matchers.cpp:68

String value that must match the provided time format string.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `format` | `const std::string &` | Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html) |
| `example` | `const std::string &` | Example value to use |

---

#### Time

```cpp
IMatcher::Ptr Time(const std::string & format)
```

Defined in src/matchers.cpp:72

String value that must match the provided time format string. Example values will be generated using the current system date and time.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `format` | `const std::string &` | Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html) |

---

#### Matching

```cpp
IMatcher::Ptr Matching(const std::string & regex, const std::string & example)
```

Defined in src/matchers.cpp:76

String value that must match the regular expression

---

#### Matching

```cpp
IMatcher::Ptr Matching(const std::string & regex)
```

Defined in src/matchers.cpp:80

String value that must match the regular expression. Random values will be generated for the examples.

---

#### EachLike

```cpp
IMatcher::Ptr EachLike(const IMatcher::Ptr obj)
```

Defined in src/matchers.cpp:84

Array of values where each item in the array must match the provided template.

---

#### EachLike

```cpp
IMatcher::Ptr EachLike(int examples, const IMatcher::Ptr obj)
```

Defined in src/matchers.cpp:88

Array of values where each item in the array must match the provided template.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `examples` | `int` | The number of example values to generate |

---

#### AtLeastOneLike

```cpp
IMatcher::Ptr AtLeastOneLike(const IMatcher::Ptr obj)
```

Defined in src/matchers.cpp:92

Array of values that must have at least one value and each item in the array must match the provided template.

---

#### AtLeastOneLike

```cpp
IMatcher::Ptr AtLeastOneLike(int examples, const IMatcher::Ptr obj)
```

Defined in src/matchers.cpp:96

Array of values that must have at least one value and each item in the array must match the provided template.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `examples` | `int` | The number of example values to generate |

---

#### HexValue

```cpp
IMatcher::Ptr HexValue(const std::string & example)
```

Defined in src/matchers.cpp:100

Match a hexadecimal value

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `example` | `const std::string &` | Example value |

---

#### HexValue

```cpp
IMatcher::Ptr HexValue()
```

Defined in src/matchers.cpp:104

Match a hexadecimal value. Random examples will be generated.

---

#### IPAddress

```cpp
IMatcher::Ptr IPAddress(const std::string & example)
```

Defined in src/matchers.cpp:108

Match an IP Address

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `example` | `const std::string &` | Example value |

---

#### IPAddress

```cpp
IMatcher::Ptr IPAddress()
```

Defined in src/matchers.cpp:112

Match an IP Address. Will use 127.0.0.1 for examples.

---

#### Numeric

```cpp
IMatcher::Ptr Numeric(int example)
```

Defined in src/matchers.cpp:116

Match a numeric value. This will match any numeric type (integer or floating point).

---

#### Numeric

```cpp
IMatcher::Ptr Numeric(long example)
```

Defined in src/matchers.cpp:120

Match a numeric value. This will match any numeric type (integer or floating point).

---

#### Numeric

```cpp
IMatcher::Ptr Numeric(float example)
```

Defined in src/matchers.cpp:124

Match a numeric value. This will match any numeric type (integer or floating point).

---

#### Numeric

```cpp
IMatcher::Ptr Numeric(double example)
```

Defined in src/matchers.cpp:128

Match a numeric value. This will match any numeric type (integer or floating point).

---

#### Numeric

```cpp
IMatcher::Ptr Numeric()
```

Defined in src/matchers.cpp:132

Match a numeric value. Random decimal values will be generated for examples.

---

#### Decimal

```cpp
IMatcher::Ptr Decimal(float example)
```

Defined in src/matchers.cpp:136

Match a decimal value (number with atleast one digit after the decimal point)

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `example` | `float` | Example value |

---

#### Decimal

```cpp
IMatcher::Ptr Decimal(double example)
```

Defined in src/matchers.cpp:140

Match a decimal value (number with atleast one digit after the decimal point)

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `example` | `double` | Example value |

---

#### Decimal

```cpp
IMatcher::Ptr Decimal()
```

Defined in src/matchers.cpp:144

Match a decimal value (number with atleast one digit after the decimal point). Random values will be generated for examples.

---

#### Uuid

```cpp
IMatcher::Ptr Uuid(const std::string & example)
```

Defined in src/matchers.cpp:148

Match a universally unique identifier (UUID)

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `example` | `const std::string &` | value to use for examples |

---

#### Uuid

```cpp
IMatcher::Ptr Uuid()
```

Defined in src/matchers.cpp:152

Match a universally unique identifier (UUID). Random values will be used for examples.

---

#### AtMostLike

```cpp
IMatcher::Ptr AtMostLike(int max, const IMatcher::Ptr obj)
```

Defined in src/matchers.cpp:156

Array with maximum size and each element like the template object.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `max` | `int` | The maximum size of the array |

---

#### AtMostLike

```cpp
IMatcher::Ptr AtMostLike(int max, int examples, const IMatcher::Ptr obj)
```

Defined in src/matchers.cpp:160

Array with maximum size and each element like the template object.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `max` | `int` | The maximum size of the array |
| `examples` | `int` | Number of examples to generate. |

---

#### MinArrayLike

```cpp
IMatcher::Ptr MinArrayLike(int min, const IMatcher::Ptr obj)
```

Defined in src/matchers.cpp:164

Array with minimum size and each element like the template object.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `min` | `int` | The minimum size of the array |

---

#### MinArrayLike

```cpp
IMatcher::Ptr MinArrayLike(int min, int examples, const IMatcher::Ptr obj)
```

Defined in src/matchers.cpp:168

Array with minimum size and each element like the template object.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `min` | `int` | The minimum size of the array |
| `examples` | `int` | Number of examples to generate. |

---

#### ConstrainedArrayLike

```cpp
IMatcher::Ptr ConstrainedArrayLike(int min, int max, const IMatcher::Ptr obj)
```

Defined in src/matchers.cpp:172

Array with minimum and maximum size and each element like the template object.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `min` | `int` | The minimum size of the array |
| `max` | `int` | The maximum size of the array |

---

#### ConstrainedArrayLike

```cpp
IMatcher::Ptr ConstrainedArrayLike(int min, int max, int examples, const IMatcher::Ptr obj)
```

Defined in src/matchers.cpp:176

Array with minimum and maximum size and each element like the template object.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `min` | `int` | The minimum size of the array |
| `max` | `int` | The maximum size of the array |
| `examples` | `int` | number of examples to generate. |

---

#### EqualTo

```cpp
IMatcher::Ptr EqualTo(int value)
```

Defined in src/matchers.cpp:180

Match by equality. This is mainly used to reset the cascading type matchers.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `int` | Value to match to |

---

#### EqualTo

```cpp
IMatcher::Ptr EqualTo(long value)
```

Defined in src/matchers.cpp:184

Match by equality. This is mainly used to reset the cascading type matchers.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `long` | Value to match to |

---

#### EqualTo

```cpp
IMatcher::Ptr EqualTo(float value)
```

Defined in src/matchers.cpp:188

Match by equality. This is mainly used to reset the cascading type matchers.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `float` | Value to match to |

---

#### EqualTo

```cpp
IMatcher::Ptr EqualTo(double value)
```

Defined in src/matchers.cpp:192

Match by equality. This is mainly used to reset the cascading type matchers.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `double` | Value to match to |

---

#### EqualTo

```cpp
IMatcher::Ptr EqualTo(const std::string & value)
```

Defined in src/matchers.cpp:196

Match by equality. This is mainly used to reset the cascading type matchers.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `const std::string &` | Value to match to |

---

#### EqualTo

```cpp
IMatcher::Ptr EqualTo(const char * value)
```

Defined in src/matchers.cpp:200

Match by equality. This is mainly used to reset the cascading type matchers.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `const char *` | Value to match to |

---

#### EqualTo

```cpp
IMatcher::Ptr EqualTo(bool value)
```

Defined in src/matchers.cpp:204

Match by equality. This is mainly used to reset the cascading type matchers.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `bool` | Value to match to |

---

#### IncludesStr

```cpp
IMatcher::Ptr IncludesStr(const std::string & value)
```

Defined in src/matchers.cpp:208

Matches if the string value contains the given value

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `const std::string &` | String value that must be present |

---

#### NullValue

```cpp
IMatcher::Ptr NullValue()
```

Defined in src/matchers.cpp:212

Matches a JSON null value

---

#### Url

```cpp
IMatcher::Ptr Url(const std::string & basePath, const std::vector< IMatcher::Ptr > & pathFragments)
```

Defined in src/matchers.cpp:216

Matches a URL composed of a base path and a list of path fragments

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `basePath` | `const std::string &` | Base path of the URL |
| `pathFragments` | const std::vector< [`IMatcher::Ptr`](#ptr) > & | list of path fragments, can be regular expressions. Only the Equals and Matching matchers will work. |

---

#### Url

```cpp
IMatcher::Ptr Url(const std::vector< IMatcher::Ptr > & pathFragments)
```

Defined in src/matchers.cpp:220

Matches a URL composed of a base path and a list of path fragments. The base URL from the mock server will be used.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pathFragments` | const std::vector< [`IMatcher::Ptr`](#ptr) > & | list of path fragments, can be regular expressions. Only the Equals and Matching matchers will work. |

---

#### ArrayContaining

```cpp
IMatcher::Ptr ArrayContaining(const std::vector< IMatcher::Ptr > & variants)
```

Defined in src/matchers.cpp:224

Matches the items in an array against a number of variants. Matching is successful if each variant occurs once in the array. Variants may be objects containing matching rules.



## IMatcher

```cpp
#include <matchers.h>
```

```cpp
class IMatcher
```

Defined in include/matchers.h:13

> **Subclassed by:** [`ArrayContainsMatcher`](#arraycontainsmatcher), [`DateMatcher`](#datematcher), [`DateTimeMatcher`](#datetimematcher), [`DecimalMatcher`](#decimalmatcher), [`EachlikeMatcher`](#eachlikematcher), [`EqualsMatcher< T >`](#equalsmatcher), [`HexadecimalMatcher`](#hexadecimalmatcher), [`IPAddressMatcher`](#ipaddressmatcher), [`IncludesMatcher`](#includesmatcher), [`IntegerMatcher`](#integermatcher), [`NullMatcher`](#nullmatcher), [`NumberMatcher< T >`](#numbermatcher), [`ObjectMatcher`](#objectmatcher), [`RegexMatcher`](#regexmatcher), [`TimeMatcher`](#timematcher), [`TypeMatcher< T >`](#typematcher), [`UrlMatcher`](#urlmatcher), [`UuidMatcher`](#uuidmatcher)

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`getJson`](#getjson) `virtual` `const` |  |
| `std::string` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `std::string` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:16

##### Reimplemented by

- [`getJson`](#getjson-12)
- [`getJson`](#getjson-8)
- [`getJson`](#getjson-7)
- [`getJson`](#getjson-3)
- [`getJson`](#getjson-11)
- [`getJson`](#getjson-6)
- [`getJson`](#getjson-13)
- [`getJson`](#getjson-16)
- [`getJson`](#getjson-2)
- [`getJson`](#getjson-14)
- [`getJson`](#getjson-17)
- [`getJson`](#getjson-5)
- [`getJson`](#getjson-1)
- [`getJson`](#getjson-10)
- [`getJson`](#getjson-9)
- [`getJson`](#getjson-4)
- [`getJson`](#getjson-18)
- [`getJson`](#getjson-15)

---

#### as_example

`virtual` `const` `inline`

```cpp
virtual inline std::string as_example() const
```

Defined in include/matchers.h:17

##### Reimplemented by

- [`as_example`](#as_example-1)
- [`as_example`](#as_example-2)

---

#### as_regex

`virtual` `const` `inline`

```cpp
virtual inline std::string as_regex() const
```

Defined in include/matchers.h:18

##### Reimplemented by

- [`as_regex`](#as_regex-1)

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr)  |  |

---

#### Ptr

```cpp
using Ptr = std::shared_ptr< IMatcher >
```

Type: std::shared_ptr< [`IMatcher`](#imatcher) >

Defined in include/matchers.h:15



## ObjectMatcher

```cpp
#include <matchers.h>
```

```cpp
class ObjectMatcher
```

Defined in include/matchers.h:21

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`ObjectMatcher`](#objectmatcher-1) | `function` | Declared here |
| [`getJson`](#getjson-1) | `function` | Declared here |
| [`fields`](#fields) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ObjectMatcher`](#objectmatcher-1) `inline` |  |
| `std::string` | [`getJson`](#getjson-1) `virtual` `const` |  |

---

#### ObjectMatcher

`inline`

```cpp
inline ObjectMatcher(const std::unordered_map< std::string, IMatcher::Ptr > & i_fields)
```

Defined in include/matchers.h:23

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:25

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| std::unordered_map< std::string, [`IMatcher::Ptr`](#ptr) > | [`fields`](#fields)  |  |

---

#### fields

```cpp
std::unordered_map< std::string, IMatcher::Ptr > fields
```

Type: std::unordered_map< std::string, [`IMatcher::Ptr`](#ptr) >

Defined in include/matchers.h:28



## IntegerMatcher

```cpp
#include <matchers.h>
```

```cpp
class IntegerMatcher
```

Defined in include/matchers.h:31

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`IntegerMatcher`](#integermatcher-1) | `function` | Declared here |
| [`IntegerMatcher`](#integermatcher-2) | `function` | Declared here |
| [`IntegerMatcher`](#integermatcher-3) | `function` | Declared here |
| [`getJson`](#getjson-2) | `function` | Declared here |
| [`value`](#value) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`IntegerMatcher`](#integermatcher-1) `inline` |  |
|  | [`IntegerMatcher`](#integermatcher-2) `inline` |  |
|  | [`IntegerMatcher`](#integermatcher-3) `inline` |  |
| `std::string` | [`getJson`](#getjson-2) `virtual` `const` |  |

---

#### IntegerMatcher

`inline`

```cpp
inline IntegerMatcher()
```

Defined in include/matchers.h:33

---

#### IntegerMatcher

`inline`

```cpp
inline IntegerMatcher(long v)
```

Defined in include/matchers.h:34

---

#### IntegerMatcher

`inline`

```cpp
inline IntegerMatcher(int v)
```

Defined in include/matchers.h:35

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:37

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::optional< long >` | [`value`](#value)  |  |

---

#### value

```cpp
std::optional< long > value
```

Defined in include/matchers.h:40



## DecimalMatcher

```cpp
#include <matchers.h>
```

```cpp
class DecimalMatcher
```

Defined in include/matchers.h:43

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`DecimalMatcher`](#decimalmatcher-1) | `function` | Declared here |
| [`DecimalMatcher`](#decimalmatcher-2) | `function` | Declared here |
| [`DecimalMatcher`](#decimalmatcher-3) | `function` | Declared here |
| [`getJson`](#getjson-3) | `function` | Declared here |
| [`value`](#value-1) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`DecimalMatcher`](#decimalmatcher-1) `inline` |  |
|  | [`DecimalMatcher`](#decimalmatcher-2) `inline` |  |
|  | [`DecimalMatcher`](#decimalmatcher-3) `inline` |  |
| `std::string` | [`getJson`](#getjson-3) `virtual` `const` |  |

---

#### DecimalMatcher

`inline`

```cpp
inline DecimalMatcher()
```

Defined in include/matchers.h:45

---

#### DecimalMatcher

`inline`

```cpp
inline DecimalMatcher(float v)
```

Defined in include/matchers.h:46

---

#### DecimalMatcher

`inline`

```cpp
inline DecimalMatcher(double v)
```

Defined in include/matchers.h:47

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:49

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::optional< double >` | [`value`](#value-1)  |  |

---

#### value

```cpp
std::optional< double > value
```

Defined in include/matchers.h:52



## TypeMatcher

```cpp
#include <matchers.h>
```

```cpp
template<typename T>
class TypeMatcher
```

Defined in include/matchers.h:56

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`TypeMatcher`](#typematcher-1) | `function` | Declared here |
| [`getJson`](#getjson-4) | `function` | Declared here |
| [`value`](#value-2) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`TypeMatcher`](#typematcher-1) `inline` |  |
| `std::string` | [`getJson`](#getjson-4) `virtual` `const` |  |

---

#### TypeMatcher

`inline`

```cpp
inline TypeMatcher(T v)
```

Defined in include/matchers.h:58

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:60

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `T` | [`value`](#value-2)  |  |

---

#### value

```cpp
T value
```

Defined in include/matchers.h:63



## NumberMatcher

```cpp
#include <matchers.h>
```

```cpp
template<typename T>
class NumberMatcher
```

Defined in include/matchers.h:67

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`NumberMatcher`](#numbermatcher-1) | `function` | Declared here |
| [`NumberMatcher`](#numbermatcher-2) | `function` | Declared here |
| [`getJson`](#getjson-5) | `function` | Declared here |
| [`value`](#value-3) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`NumberMatcher`](#numbermatcher-1) `inline` |  |
|  | [`NumberMatcher`](#numbermatcher-2) `inline` |  |
| `std::string` | [`getJson`](#getjson-5) `virtual` `const` |  |

---

#### NumberMatcher

`inline`

```cpp
inline NumberMatcher()
```

Defined in include/matchers.h:69

---

#### NumberMatcher

`inline`

```cpp
inline NumberMatcher(T v)
```

Defined in include/matchers.h:70

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:72

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::optional< T >` | [`value`](#value-3)  |  |

---

#### value

```cpp
std::optional< T > value
```

Defined in include/matchers.h:75



## EqualsMatcher

```cpp
#include <matchers.h>
```

```cpp
template<typename T>
class EqualsMatcher
```

Defined in include/matchers.h:79

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`EqualsMatcher`](#equalsmatcher-1) | `function` | Declared here |
| [`getJson`](#getjson-6) | `function` | Declared here |
| [`as_example`](#as_example-1) | `function` | Declared here |
| [`value`](#value-4) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`EqualsMatcher`](#equalsmatcher-1) `inline` |  |
| `std::string` | [`getJson`](#getjson-6) `virtual` `const` |  |
| `std::string` | [`as_example`](#as_example-1) `virtual` `const` `inline` |  |

---

#### EqualsMatcher

`inline`

```cpp
inline EqualsMatcher(T v)
```

Defined in include/matchers.h:81

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:83

##### Reimplements

- [`getJson`](#getjson)

---

#### as_example

`virtual` `const` `inline`

```cpp
virtual inline std::string as_example() const
```

Defined in include/matchers.h:85

##### Reimplements

- [`as_example`](#as_example)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `T` | [`value`](#value-4)  |  |

---

#### value

```cpp
T value
```

Defined in include/matchers.h:92



## DateTimeMatcher

```cpp
#include <matchers.h>
```

```cpp
class DateTimeMatcher
```

Defined in include/matchers.h:95

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`DateTimeMatcher`](#datetimematcher-1) | `function` | Declared here |
| [`DateTimeMatcher`](#datetimematcher-2) | `function` | Declared here |
| [`getJson`](#getjson-7) | `function` | Declared here |
| [`format`](#format) | `variable` | Declared here |
| [`example`](#example) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`DateTimeMatcher`](#datetimematcher-1) `inline` |  |
|  | [`DateTimeMatcher`](#datetimematcher-2) `inline` |  |
| `std::string` | [`getJson`](#getjson-7) `virtual` `const` |  |

---

#### DateTimeMatcher

`inline`

```cpp
inline DateTimeMatcher(const std::string & f)
```

Defined in include/matchers.h:97

---

#### DateTimeMatcher

`inline`

```cpp
inline DateTimeMatcher(const std::string & f, const std::string & e)
```

Defined in include/matchers.h:98

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:100

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`format`](#format)  |  |
| `std::string` | [`example`](#example)  |  |

---

#### format

```cpp
std::string format
```

Defined in include/matchers.h:103

---

#### example

```cpp
std::string example
```

Defined in include/matchers.h:103



## DateMatcher

```cpp
#include <matchers.h>
```

```cpp
class DateMatcher
```

Defined in include/matchers.h:106

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`DateMatcher`](#datematcher-1) | `function` | Declared here |
| [`DateMatcher`](#datematcher-2) | `function` | Declared here |
| [`getJson`](#getjson-8) | `function` | Declared here |
| [`format`](#format-1) | `variable` | Declared here |
| [`example`](#example-1) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`DateMatcher`](#datematcher-1) `inline` |  |
|  | [`DateMatcher`](#datematcher-2) `inline` |  |
| `std::string` | [`getJson`](#getjson-8) `virtual` `const` |  |

---

#### DateMatcher

`inline`

```cpp
inline DateMatcher(const std::string & f)
```

Defined in include/matchers.h:108

---

#### DateMatcher

`inline`

```cpp
inline DateMatcher(const std::string & f, const std::string & e)
```

Defined in include/matchers.h:109

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:111

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`format`](#format-1)  |  |
| `std::string` | [`example`](#example-1)  |  |

---

#### format

```cpp
std::string format
```

Defined in include/matchers.h:114

---

#### example

```cpp
std::string example
```

Defined in include/matchers.h:114



## TimeMatcher

```cpp
#include <matchers.h>
```

```cpp
class TimeMatcher
```

Defined in include/matchers.h:117

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`TimeMatcher`](#timematcher-1) | `function` | Declared here |
| [`TimeMatcher`](#timematcher-2) | `function` | Declared here |
| [`getJson`](#getjson-9) | `function` | Declared here |
| [`format`](#format-2) | `variable` | Declared here |
| [`example`](#example-2) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`TimeMatcher`](#timematcher-1) `inline` |  |
|  | [`TimeMatcher`](#timematcher-2) `inline` |  |
| `std::string` | [`getJson`](#getjson-9) `virtual` `const` |  |

---

#### TimeMatcher

`inline`

```cpp
inline TimeMatcher(const std::string & f)
```

Defined in include/matchers.h:119

---

#### TimeMatcher

`inline`

```cpp
inline TimeMatcher(const std::string & f, const std::string & e)
```

Defined in include/matchers.h:120

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:122

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`format`](#format-2)  |  |
| `std::string` | [`example`](#example-2)  |  |

---

#### format

```cpp
std::string format
```

Defined in include/matchers.h:125

---

#### example

```cpp
std::string example
```

Defined in include/matchers.h:125



## RegexMatcher

```cpp
#include <matchers.h>
```

```cpp
class RegexMatcher
```

Defined in include/matchers.h:128

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`RegexMatcher`](#regexmatcher-1) | `function` | Declared here |
| [`RegexMatcher`](#regexmatcher-2) | `function` | Declared here |
| [`getJson`](#getjson-10) | `function` | Declared here |
| [`as_example`](#as_example-2) | `function` | Declared here |
| [`as_regex`](#as_regex-1) | `function` | Declared here |
| [`regex`](#regex) | `variable` | Declared here |
| [`example`](#example-3) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`RegexMatcher`](#regexmatcher-1) `inline` |  |
|  | [`RegexMatcher`](#regexmatcher-2) `inline` |  |
| `std::string` | [`getJson`](#getjson-10) `virtual` `const` |  |
| `std::string` | [`as_example`](#as_example-2) `virtual` `const` `inline` |  |
| `std::string` | [`as_regex`](#as_regex-1) `virtual` `const` `inline` |  |

---

#### RegexMatcher

`inline`

```cpp
inline RegexMatcher(const std::string & r)
```

Defined in include/matchers.h:130

---

#### RegexMatcher

`inline`

```cpp
inline RegexMatcher(const std::string & r, const std::string & e)
```

Defined in include/matchers.h:131

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:133

##### Reimplements

- [`getJson`](#getjson)

---

#### as_example

`virtual` `const` `inline`

```cpp
virtual inline std::string as_example() const
```

Defined in include/matchers.h:135

##### Reimplements

- [`as_example`](#as_example)

---

#### as_regex

`virtual` `const` `inline`

```cpp
virtual inline std::string as_regex() const
```

Defined in include/matchers.h:136

##### Reimplements

- [`as_regex`](#as_regex)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`regex`](#regex)  |  |
| `std::string` | [`example`](#example-3)  |  |

---

#### regex

```cpp
std::string regex
```

Defined in include/matchers.h:139

---

#### example

```cpp
std::string example
```

Defined in include/matchers.h:139



## EachlikeMatcher

```cpp
#include <matchers.h>
```

```cpp
class EachlikeMatcher
```

Defined in include/matchers.h:142

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`EachlikeMatcher`](#eachlikematcher-1) | `function` | Declared here |
| [`EachlikeMatcher`](#eachlikematcher-2) | `function` | Declared here |
| [`EachlikeMatcher`](#eachlikematcher-3) | `function` | Declared here |
| [`EachlikeMatcher`](#eachlikematcher-4) | `function` | Declared here |
| [`getJson`](#getjson-11) | `function` | Declared here |
| [`examples`](#examples) | `variable` | Declared here |
| [`min`](#min) | `variable` | Declared here |
| [`max`](#max) | `variable` | Declared here |
| [`obj`](#obj) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`EachlikeMatcher`](#eachlikematcher-1) `inline` |  |
|  | [`EachlikeMatcher`](#eachlikematcher-2) `inline` |  |
|  | [`EachlikeMatcher`](#eachlikematcher-3) `inline` |  |
|  | [`EachlikeMatcher`](#eachlikematcher-4) `inline` |  |
| `std::string` | [`getJson`](#getjson-11) `virtual` `const` |  |

---

#### EachlikeMatcher

`inline`

```cpp
inline EachlikeMatcher(IMatcher::Ptr t)
```

Defined in include/matchers.h:144

---

#### EachlikeMatcher

`inline`

```cpp
inline EachlikeMatcher(int e, IMatcher::Ptr t)
```

Defined in include/matchers.h:145

---

#### EachlikeMatcher

`inline`

```cpp
inline EachlikeMatcher(int e, int m, IMatcher::Ptr t)
```

Defined in include/matchers.h:146

---

#### EachlikeMatcher

`inline`

```cpp
inline EachlikeMatcher(int e, int m, int mx, IMatcher::Ptr t)
```

Defined in include/matchers.h:147

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:149

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`examples`](#examples)  |  |
| `std::optional< int >` | [`min`](#min)  |  |
| `std::optional< int >` | [`max`](#max)  |  |
| [`IMatcher::Ptr`](#ptr) | [`obj`](#obj)  |  |

---

#### examples

```cpp
int examples = 1
```

Defined in include/matchers.h:152

---

#### min

```cpp
std::optional< int > min
```

Defined in include/matchers.h:153

---

#### max

```cpp
std::optional< int > max
```

Defined in include/matchers.h:154

---

#### obj

```cpp
IMatcher::Ptr obj
```

Type: [`IMatcher::Ptr`](#ptr)

Defined in include/matchers.h:155



## ArrayContainsMatcher

```cpp
#include <matchers.h>
```

```cpp
class ArrayContainsMatcher
```

Defined in include/matchers.h:158

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`ArrayContainsMatcher`](#arraycontainsmatcher-1) | `function` | Declared here |
| [`getJson`](#getjson-12) | `function` | Declared here |
| [`variants`](#variants) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ArrayContainsMatcher`](#arraycontainsmatcher-1) `inline` |  |
| `std::string` | [`getJson`](#getjson-12) `virtual` `const` |  |

---

#### ArrayContainsMatcher

`inline`

```cpp
inline ArrayContainsMatcher(const std::vector< IMatcher::Ptr > & i_variants)
```

Defined in include/matchers.h:160

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:162

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| std::vector< [`IMatcher::Ptr`](#ptr) > | [`variants`](#variants)  |  |

---

#### variants

```cpp
std::vector< IMatcher::Ptr > variants
```

Type: std::vector< [`IMatcher::Ptr`](#ptr) >

Defined in include/matchers.h:165



## HexadecimalMatcher

```cpp
#include <matchers.h>
```

```cpp
class HexadecimalMatcher
```

Defined in include/matchers.h:168

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`HexadecimalMatcher`](#hexadecimalmatcher-1) | `function` | Declared here |
| [`HexadecimalMatcher`](#hexadecimalmatcher-2) | `function` | Declared here |
| [`getJson`](#getjson-13) | `function` | Declared here |
| [`example`](#example-4) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`HexadecimalMatcher`](#hexadecimalmatcher-1) `inline` |  |
|  | [`HexadecimalMatcher`](#hexadecimalmatcher-2) `inline` |  |
| `std::string` | [`getJson`](#getjson-13) `virtual` `const` |  |

---

#### HexadecimalMatcher

`inline`

```cpp
inline HexadecimalMatcher()
```

Defined in include/matchers.h:170

---

#### HexadecimalMatcher

`inline`

```cpp
inline HexadecimalMatcher(const std::string & hex)
```

Defined in include/matchers.h:171

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:173

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`example`](#example-4)  |  |

---

#### example

```cpp
std::string example
```

Defined in include/matchers.h:176



## IPAddressMatcher

```cpp
#include <matchers.h>
```

```cpp
class IPAddressMatcher
```

Defined in include/matchers.h:179

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`IPAddressMatcher`](#ipaddressmatcher-1) | `function` | Declared here |
| [`IPAddressMatcher`](#ipaddressmatcher-2) | `function` | Declared here |
| [`getJson`](#getjson-14) | `function` | Declared here |
| [`example`](#example-5) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`IPAddressMatcher`](#ipaddressmatcher-1) `inline` |  |
|  | [`IPAddressMatcher`](#ipaddressmatcher-2) `inline` |  |
| `std::string` | [`getJson`](#getjson-14) `virtual` `const` |  |

---

#### IPAddressMatcher

`inline`

```cpp
inline IPAddressMatcher()
```

Defined in include/matchers.h:181

---

#### IPAddressMatcher

`inline`

```cpp
inline IPAddressMatcher(const std::string & address)
```

Defined in include/matchers.h:182

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:184

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`example`](#example-5)  |  |

---

#### example

```cpp
std::string example
```

Defined in include/matchers.h:187



## UuidMatcher

```cpp
#include <matchers.h>
```

```cpp
class UuidMatcher
```

Defined in include/matchers.h:190

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`UuidMatcher`](#uuidmatcher-1) | `function` | Declared here |
| [`UuidMatcher`](#uuidmatcher-2) | `function` | Declared here |
| [`getJson`](#getjson-15) | `function` | Declared here |
| [`example`](#example-6) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`UuidMatcher`](#uuidmatcher-1) `inline` |  |
|  | [`UuidMatcher`](#uuidmatcher-2) `inline` |  |
| `std::string` | [`getJson`](#getjson-15) `virtual` `const` |  |

---

#### UuidMatcher

`inline`

```cpp
inline UuidMatcher()
```

Defined in include/matchers.h:192

---

#### UuidMatcher

`inline`

```cpp
inline UuidMatcher(const std::string & uuid)
```

Defined in include/matchers.h:193

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:195

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`example`](#example-6)  |  |

---

#### example

```cpp
std::string example
```

Defined in include/matchers.h:198



## IncludesMatcher

```cpp
#include <matchers.h>
```

```cpp
class IncludesMatcher
```

Defined in include/matchers.h:201

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`IncludesMatcher`](#includesmatcher-1) | `function` | Declared here |
| [`getJson`](#getjson-16) | `function` | Declared here |
| [`value`](#value-5) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`IncludesMatcher`](#includesmatcher-1) `inline` |  |
| `std::string` | [`getJson`](#getjson-16) `virtual` `const` |  |

---

#### IncludesMatcher

`inline`

```cpp
inline IncludesMatcher(const std::string & v)
```

Defined in include/matchers.h:203

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:205

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`value`](#value-5)  |  |

---

#### value

```cpp
std::string value
```

Defined in include/matchers.h:208



## NullMatcher

```cpp
#include <matchers.h>
```

```cpp
class NullMatcher
```

Defined in include/matchers.h:211

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`getJson`](#getjson-17) | `function` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`getJson`](#getjson-17) `virtual` `const` |  |

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:213

##### Reimplements

- [`getJson`](#getjson)



## UrlMatcher

```cpp
#include <matchers.h>
```

```cpp
class UrlMatcher
```

Defined in include/matchers.h:216

> **Inherits:** [`IMatcher`](#imatcher)

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`UrlMatcher`](#urlmatcher-1) | `function` | Declared here |
| [`getJson`](#getjson-18) | `function` | Declared here |
| [`basePath`](#basepath) | `variable` | Declared here |
| [`pathFragments`](#pathfragments) | `variable` | Declared here |
| [`getJson`](#getjson) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_example`](#as_example) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`as_regex`](#as_regex) | `function` | Inherited from [`IMatcher`](#imatcher) |
| [`Ptr`](#ptr) | `typedef` | Inherited from [`IMatcher`](#imatcher) |

### Inherited from [`IMatcher`](#imatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`getJson`](#getjson) `virtual` `const` |  |
| `function` | [`as_example`](#as_example) `virtual` `const` `inline` |  |
| `function` | [`as_regex`](#as_regex) `virtual` `const` `inline` |  |
| `typedef` | [`Ptr`](#ptr)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`UrlMatcher`](#urlmatcher-1) `inline` |  |
| `std::string` | [`getJson`](#getjson-18) `virtual` `const` |  |

---

#### UrlMatcher

`inline`

```cpp
inline UrlMatcher(const std::string & b, const std::vector< IMatcher::Ptr > & f)
```

Defined in include/matchers.h:218

---

#### getJson

`virtual` `const`

```cpp
virtual std::string getJson() const
```

Defined in include/matchers.h:220

##### Reimplements

- [`getJson`](#getjson)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`basePath`](#basepath)  |  |
| std::vector< [`IMatcher::Ptr`](#ptr) > | [`pathFragments`](#pathfragments)  |  |

---

#### basePath

```cpp
std::string basePath
```

Defined in include/matchers.h:223

---

#### pathFragments

```cpp
std::vector< IMatcher::Ptr > pathFragments
```

Type: std::vector< [`IMatcher::Ptr`](#ptr) >

Defined in include/matchers.h:224

Generated by [Moxygen](https://0state.com/moxygen)