# Pact C++ Verifier Reference

## Namespaces

| Name | Description |
|------|-------------|
| [`pact_verifier`](#pact_verifier) | Minimal, dependency-free HTTP/1.1 server used to host the provider state change endpoint required by Pact verification. |
| [`http`](#http) |  |

## Classes

| Name | Description |
|------|-------------|
| [`Impl`](#impl-7) |  |
| [`Impl`](#impl-9) |  |
| [`Impl`](#impl-1) |  |
| [`Impl`](#impl-4) |  |

## Macros

---

### PACT_CPP_VERIFIER_VERSION

```cpp
#define PACT_CPP_VERIFIER_VERSION "0.0.0"
```

Defined in src/verifier.cpp:12

---

### PACT_INVALID_SOCKET

```cpp
#define PACT_INVALID_SOCKET (-1)
```

Defined in src/http_server.cpp:26

---

### pact_close_socket

```cpp
#define pact_close_socket ::close
```

Defined in src/http_server.cpp:27

## Typedefs

---

### json

```cpp
using json = nlohmann::json
```

Defined in src/verifier.cpp:15

---

### pact_socket_t

```cpp
using pact_socket_t = int
```

Defined in src/http_server.cpp:25



## pact_verifier

Minimal, dependency-free HTTP/1.1 server used to host the provider state change endpoint required by Pact verification.

It is deliberately small (single connection at a time, `Connection: close`) because the Pact verifier drives the provider sequentially. It is also useful for standing up a stub provider in your own tests, but it is not intended to be a general purpose production web server.

### Classes

| Name | Description |
|------|-------------|
| [`ProviderStateParams`](#providerstateparams) | Parameters supplied with a provider state, as defined by the consumer with `given(state, params)`. |
| [`ProviderStateServer`](#providerstateserver) | Hosts the HTTP endpoint the Pact verifier calls to set up and tear down provider states. |
| [`ProviderMessage`](#providermessage) | A message produced by the provider, returned from a message handler. |
| [`MessageProviderServer`](#messageproviderserver) | Hosts the HTTP endpoint the Pact verifier calls to fetch messages from the provider. |
| [`VerificationResult`](#verificationresult) | The outcome of a verification run. |
| [`Verifier`](#verifier-1) | Verifies that a provider honours the contracts published by its consumers. |
| [`ProviderStateRequest`](#providerstaterequest) | A provider state change request received from the verifier. |
| [`MessageProviderState`](#messageproviderstate) | A provider state attached to a message interaction. |
| [`MessageRequest`](#messagerequest) | The verifier asking the provider to produce a message. |
| [`BrokerSelectorOptions`](#brokerselectoroptions) | Options for fetching pacts from a Pact Broker or PactFlow using consumer version selectors. |
| [`PublishOptions`](#publishoptions) | Options for publishing verification results back to the broker. |

### Enumerations

| Name | Description |
|------|-------------|
| [`LogLevel`](#loglevel)  | Log levels supported by the Pact core. |
| [`StateAction`](#stateaction)  | Whether the verifier is asking for a provider state to be set up before an interaction, or torn down afterwards. |

---

#### LogLevel

```cpp
enum LogLevel
```

Defined in include/verifier.h:28

Log levels supported by the Pact core.

| Value | Description |
|-------|-------------|
| `Off` |  |
| `Error` |  |
| `Warn` |  |
| `Info` |  |
| `Debug` |  |
| `Trace` |  |

---

#### StateAction

```cpp
enum StateAction
```

Defined in include/verifier.h:53

Whether the verifier is asking for a provider state to be set up before an interaction, or torn down afterwards.

| Value | Description |
|-------|-------------|
| `Setup` |  |
| `Teardown` |  |
### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| std::function< void(const [`ProviderStateRequest`](#providerstaterequest) &)> | [`StateHandler`](#statehandler)  | Callback that puts the provider into the required state. |
| std::function< std::string(const [`ProviderStateRequest`](#providerstaterequest) &)> | [`StateHandlerWithValues`](#statehandlerwithvalues)  | Callback that puts the provider into the required state and returns a JSON object of values. Those values are made available to the verifier for use by generators (`fromProviderState`). Return an empty string for no values. |
| std::function< [`ProviderMessage`](#providermessage)(const [`MessageRequest`](#messagerequest) &)> | [`MessageHandler`](#messagehandler)  | Callback that produces the message the provider would send. |

---

#### StateHandler

```cpp
using StateHandler = std::function< void(const ProviderStateRequest &)>
```

Type: std::function< void(const [`ProviderStateRequest`](#providerstaterequest) &)>

Defined in include/verifier.h:113

Callback that puts the provider into the required state.

---

#### StateHandlerWithValues

```cpp
using StateHandlerWithValues = std::function< std::string(const ProviderStateRequest &)>
```

Type: std::function< std::string(const [`ProviderStateRequest`](#providerstaterequest) &)>

Defined in include/verifier.h:120

Callback that puts the provider into the required state and returns a JSON object of values. Those values are made available to the verifier for use by generators (`fromProviderState`). Return an empty string for no values.

---

#### MessageHandler

```cpp
using MessageHandler = std::function< ProviderMessage(const MessageRequest &)>
```

Type: std::function< [`ProviderMessage`](#providermessage)(const [`MessageRequest`](#messagerequest) &)>

Defined in include/verifier.h:248

Callback that produces the message the provider would send.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init)  | Initialise the Pact library. Reads the log level from the `LOG_LEVEL` environment variable. |
| `bool` | [`log_to_stdout`](#log_to_stdout)  | Sends Pact core logs to standard output. |
| `bool` | [`log_to_stderr`](#log_to_stderr)  | Sends Pact core logs to standard error. |
| `bool` | [`log_to_file`](#log_to_file)  | Sends Pact core logs to a file. |
| `bool` | [`log_to_buffer`](#log_to_buffer)  | Buffers Pact core logs in memory. Required if you want to read [VerificationResult::logs](#logs) after a verification run. |

---

#### init

```cpp
void init()
```

Defined in src/verifier.cpp:98

Initialise the Pact library. Reads the log level from the `LOG_LEVEL` environment variable.

---

#### log_to_stdout

```cpp
bool log_to_stdout(LogLevel level = LogLevel::Info)
```

Defined in src/verifier.cpp:102

Sends Pact core logs to standard output.

---

#### log_to_stderr

```cpp
bool log_to_stderr(LogLevel level = LogLevel::Info)
```

Defined in src/verifier.cpp:106

Sends Pact core logs to standard error.

---

#### log_to_file

```cpp
bool log_to_file(const std::string & file_name, LogLevel level = LogLevel::Info)
```

Defined in src/verifier.cpp:110

Sends Pact core logs to a file.

---

#### log_to_buffer

```cpp
bool log_to_buffer(LogLevel level = LogLevel::Debug)
```

Defined in src/verifier.cpp:114

Buffers Pact core logs in memory. Required if you want to read [VerificationResult::logs](#logs) after a verification run.



## ProviderStateParams

```cpp
#include <verifier.h>
```

```cpp
class ProviderStateParams
```

Defined in include/verifier.h:66

Parameters supplied with a provider state, as defined by the consumer with `given(state, params)`.

Values are exposed as strings. JSON strings are unquoted; any other JSON value (numbers, booleans, objects, arrays) is returned as its raw JSON text. Use [json()](#json) if you need the untouched document.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ProviderStateParams`](#providerstateparams-1)  |  |
|  | [`ProviderStateParams`](#providerstateparams-2) `explicit` |  |
| `bool` | [`contains`](#contains) `const` | True if a parameter with this name was supplied |
| `std::optional< std::string >` | [`get`](#get) `const` | The parameter value, or an empty optional if it was not supplied |
| `std::string` | [`get_or`](#get_or) `const` | The parameter value, or the supplied default if it was not supplied |
| `std::optional< long long >` | [`get_int`](#get_int) `const` | The parameter parsed as an integer, or an empty optional if missing or not numeric |
| `std::optional< double >` | [`get_double`](#get_double) `const` | The parameter parsed as a double, or an empty optional if missing or not numeric |
| `std::optional< bool >` | [`get_bool`](#get_bool) `const` | The parameter parsed as a boolean, or an empty optional if missing or not a boolean |
| `const std::unordered_map< std::string, std::string > &` | [`values`](#values) `const` | All parameters keyed by name |
| `const std::string &` | [`json`](#json-1) `const` | The raw JSON document the parameters were parsed from |

---

#### ProviderStateParams

```cpp
ProviderStateParams()
```

Defined in include/verifier.h:68

---

#### ProviderStateParams

`explicit`

```cpp
explicit ProviderStateParams(const std::string & json)
```

Defined in include/verifier.h:69

---

#### contains

`const`

```cpp
bool contains(const std::string & name) const
```

Defined in include/verifier.h:72

True if a parameter with this name was supplied

---

#### get

`const`

```cpp
std::optional< std::string > get(const std::string & name) const
```

Defined in include/verifier.h:75

The parameter value, or an empty optional if it was not supplied

---

#### get_or

`const`

```cpp
std::string get_or(const std::string & name, const std::string & default_value) const
```

Defined in include/verifier.h:78

The parameter value, or the supplied default if it was not supplied

---

#### get_int

`const`

```cpp
std::optional< long long > get_int(const std::string & name) const
```

Defined in include/verifier.h:81

The parameter parsed as an integer, or an empty optional if missing or not numeric

---

#### get_double

`const`

```cpp
std::optional< double > get_double(const std::string & name) const
```

Defined in include/verifier.h:84

The parameter parsed as a double, or an empty optional if missing or not numeric

---

#### get_bool

`const`

```cpp
std::optional< bool > get_bool(const std::string & name) const
```

Defined in include/verifier.h:87

The parameter parsed as a boolean, or an empty optional if missing or not a boolean

---

#### values

`const`

```cpp
const std::unordered_map< std::string, std::string > & values() const
```

Defined in include/verifier.h:90

All parameters keyed by name

---

#### json

`const`

```cpp
const std::string & json() const
```

Defined in include/verifier.h:93

The raw JSON document the parameters were parsed from

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`params_json`](#params_json)  |  |
| `std::unordered_map< std::string, std::string >` | [`params`](#params)  |  |

---

#### params_json

```cpp
std::string params_json
```

Defined in include/verifier.h:96

---

#### params

```cpp
std::unordered_map< std::string, std::string > params
```

Defined in include/verifier.h:97



## ProviderStateServer

```cpp
#include <verifier.h>
```

```cpp
class ProviderStateServer
```

Defined in include/verifier.h:132

Hosts the HTTP endpoint the Pact verifier calls to set up and tear down provider states.

The verifier does not call your state handlers directly, it makes an HTTP request for each provider state, so a server has to be running for the duration of the verification. [Verifier::add_state_handler](#add_state_handler-1) starts and stops one of these for you; use this class directly only if you want to host the endpoint yourself (for example inside your real application under test).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ProviderStateServer`](#providerstateserver-1) `explicit` |  |
|  | [`~ProviderStateServer`](#providerstateserver-2)  |  |
|  | [`ProviderStateServer`](#providerstateserver-3)  | Deleted constructor. |
| [`ProviderStateServer`](#providerstateserver-1) & | [`operator=`](#operator)  | Deleted assignment operator. |
| `void` | [`add_state_handler`](#add_state_handler)  | Registers a handler for a single provider state |
| `void` | [`add_state_handler_with_values`](#add_state_handler_with_values)  | Registers a handler for a single provider state that returns generator values |
| `void` | [`set_default_handler`](#set_default_handler)  | Registers a handler invoked for any state without a specific handler |
| `void` | [`set_default_handler_with_values`](#set_default_handler_with_values)  | Registers a value returning handler invoked for any state without a specific handler |
| `void` | [`set_unknown_state_is_error`](#set_unknown_state_is_error)  | If true (the default), a state change request for a state with no registered handler and no default handler fails with a 500. Set to false to ignore them. |
| `bool` | [`start`](#start)  | Starts the server. Pass 0 to let the OS pick a free port. Returns false if the port could not be bound. |
| `void` | [`stop`](#stop)  | Stops the server. Safe to call more than once |
| `bool` | [`is_running`](#is_running) `const` | True while the server is running |
| `uint16_t` | [`get_port`](#get_port) `const` | The port the server is bound to |
| `std::string` | [`get_url`](#get_url) `const` | The full URL of the state change endpoint, to hand to the verifier |

---

#### ProviderStateServer

`explicit`

```cpp
explicit ProviderStateServer(std::string path = "/__pact/provider-state")
```

Defined in include/verifier.h:137

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `path` | `std::string` | Path the state change endpoint is served on |

---

#### ~ProviderStateServer

```cpp
~ProviderStateServer()
```

Defined in include/verifier.h:138

---

#### ProviderStateServer

```cpp
ProviderStateServer(const ProviderStateServer &) = delete
```

Defined in include/verifier.h:140

Deleted constructor.

---

#### operator=

```cpp
ProviderStateServer & operator=(const ProviderStateServer &) = delete
```

Defined in include/verifier.h:141

Deleted assignment operator.

---

#### add_state_handler

```cpp
void add_state_handler(const std::string & state, StateHandler handler)
```

Defined in include/verifier.h:144

Registers a handler for a single provider state

---

#### add_state_handler_with_values

```cpp
void add_state_handler_with_values(const std::string & state, StateHandlerWithValues handler)
```

Defined in include/verifier.h:147

Registers a handler for a single provider state that returns generator values

---

#### set_default_handler

```cpp
void set_default_handler(StateHandler handler)
```

Defined in include/verifier.h:150

Registers a handler invoked for any state without a specific handler

---

#### set_default_handler_with_values

```cpp
void set_default_handler_with_values(StateHandlerWithValues handler)
```

Defined in include/verifier.h:153

Registers a value returning handler invoked for any state without a specific handler

---

#### set_unknown_state_is_error

```cpp
void set_unknown_state_is_error(bool is_error)
```

Defined in include/verifier.h:159

If true (the default), a state change request for a state with no registered handler and no default handler fails with a 500. Set to false to ignore them.

---

#### start

```cpp
bool start(uint16_t port = 0, const std::string & host = "127.0.0.1")
```

Defined in include/verifier.h:165

Starts the server. Pass 0 to let the OS pick a free port. Returns false if the port could not be bound.

---

#### stop

```cpp
void stop()
```

Defined in include/verifier.h:168

Stops the server. Safe to call more than once

---

#### is_running

`const`

```cpp
bool is_running() const
```

Defined in include/verifier.h:171

True while the server is running

---

#### get_port

`const`

```cpp
uint16_t get_port() const
```

Defined in include/verifier.h:174

The port the server is bound to

---

#### get_url

`const`

```cpp
std::string get_url() const
```

Defined in include/verifier.h:177

The full URL of the state change endpoint, to hand to the verifier

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| std::unique_ptr< [`Impl`](#impl-1) > | [`impl`](#impl)  |  |

---

#### impl

```cpp
std::unique_ptr< Impl > impl
```

Type: std::unique_ptr< [`Impl`](#impl-1) >

Defined in include/verifier.h:181



## Impl

```cpp
struct Impl
```

Defined in src/verifier.cpp:215

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`path`](#path)  |  |
| [`http::Server`](#server-2) | [`server`](#server)  |  |
| std::unordered_map< std::string, [`StateHandlerWithValues`](#statehandlerwithvalues) > | [`handlers`](#handlers)  |  |
| [`StateHandlerWithValues`](#statehandlerwithvalues) | [`default_handler`](#default_handler)  |  |
| `bool` | [`unknown_state_is_error`](#unknown_state_is_error)  |  |
| `std::mutex` | [`lock`](#lock)  |  |

---

#### path

```cpp
std::string path
```

Defined in src/verifier.cpp:216

---

#### server

```cpp
http::Server server
```

Type: [`http::Server`](#server-2)

Defined in src/verifier.cpp:217

---

#### handlers

```cpp
std::unordered_map< std::string, StateHandlerWithValues > handlers
```

Type: std::unordered_map< std::string, [`StateHandlerWithValues`](#statehandlerwithvalues) >

Defined in src/verifier.cpp:218

---

#### default_handler

```cpp
StateHandlerWithValues default_handler
```

Type: [`StateHandlerWithValues`](#statehandlerwithvalues)

Defined in src/verifier.cpp:219

---

#### unknown_state_is_error

```cpp
bool unknown_state_is_error = true
```

Defined in src/verifier.cpp:220

---

#### lock

```cpp
std::mutex lock
```

Defined in src/verifier.cpp:221

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Impl`](#impl-2) `inline` `explicit` |  |
| `void` | [`handle`](#handle) `inline` |  |

---

#### Impl

`inline` `explicit`

```cpp
inline explicit Impl(std::string endpoint_path)
```

Defined in src/verifier.cpp:223

---

#### handle

`inline`

```cpp
inline void handle(const http::Request & request, http::Response & response)
```

Defined in src/verifier.cpp:229



## ProviderMessage

```cpp
#include <verifier.h>
```

```cpp
class ProviderMessage
```

Defined in include/verifier.h:190

A message produced by the provider, returned from a message handler.

For an asynchronous message this is the message the provider publishes. For a synchronous message it is the response the provider sends back.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ProviderMessage`](#providermessage-1) `explicit` |  |
| [`ProviderMessage`](#providermessage-1) & | [`with_metadata`](#with_metadata)  | Adds a string metadata value, e.g. the queue or topic name |
| [`ProviderMessage`](#providermessage-1) & | [`with_json_metadata`](#with_json_metadata)  | Adds a metadata value that is already a JSON document (number, object, array, ...) |
| `const std::string &` | [`body`](#body) `const` |  |
| `const std::string &` | [`content_type`](#content_type) `const` |  |
| `const std::unordered_map< std::string, std::string > &` | [`metadata`](#metadata) `const` | Metadata values, each held as JSON text |

---

#### ProviderMessage

`explicit`

```cpp
explicit ProviderMessage(std::string body = "", std::string content_type = "application/json")
```

Defined in include/verifier.h:196

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `body` | `std::string` | The message payload |
| `content_type` | `std::string` | Content type of the payload |

---

#### with_metadata

```cpp
ProviderMessage & with_metadata(const std::string & key, const std::string & value)
```

Defined in include/verifier.h:199

Adds a string metadata value, e.g. the queue or topic name

---

#### with_json_metadata

```cpp
ProviderMessage & with_json_metadata(const std::string & key, const std::string & json_value)
```

Defined in include/verifier.h:202

Adds a metadata value that is already a JSON document (number, object, array, ...)

---

#### body

`const`

```cpp
const std::string & body() const
```

Defined in include/verifier.h:204

---

#### content_type

`const`

```cpp
const std::string & content_type() const
```

Defined in include/verifier.h:205

---

#### metadata

`const`

```cpp
const std::unordered_map< std::string, std::string > & metadata() const
```

Defined in include/verifier.h:208

Metadata values, each held as JSON text

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`message_body`](#message_body)  |  |
| `std::string` | [`message_content_type`](#message_content_type)  |  |
| `std::unordered_map< std::string, std::string >` | [`message_metadata`](#message_metadata)  |  |

---

#### message_body

```cpp
std::string message_body
```

Defined in include/verifier.h:211

---

#### message_content_type

```cpp
std::string message_content_type
```

Defined in include/verifier.h:212

---

#### message_metadata

```cpp
std::unordered_map< std::string, std::string > message_metadata
```

Defined in include/verifier.h:213



## MessageProviderServer

```cpp
#include <verifier.h>
```

```cpp
class MessageProviderServer
```

Defined in include/verifier.h:259

Hosts the HTTP endpoint the Pact verifier calls to fetch messages from the provider.

Message pacts have no HTTP request to replay, so the verifier asks the provider to produce each message over a "message" transport, which is an HTTP endpoint the provider exposes purely for verification. [Verifier::add_message_handler](#add_message_handler-1) starts and stops one of these for you.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`MessageProviderServer`](#messageproviderserver-1) `explicit` |  |
|  | [`~MessageProviderServer`](#messageproviderserver-2)  |  |
|  | [`MessageProviderServer`](#messageproviderserver-3)  | Deleted constructor. |
| [`MessageProviderServer`](#messageproviderserver-1) & | [`operator=`](#operator-1)  | Deleted assignment operator. |
| `void` | [`add_message_handler`](#add_message_handler)  | Registers a handler for a single message, keyed on the interaction description |
| `void` | [`set_default_handler`](#set_default_handler-1)  | Registers a handler invoked for any message without a specific handler |
| `bool` | [`start`](#start-1)  | Starts the server. Pass 0 to let the OS pick a free port. Returns false if the port could not be bound. |
| `void` | [`stop`](#stop-1)  | Stops the server. Safe to call more than once |
| `bool` | [`is_running`](#is_running-1) `const` | True while the server is running |
| `uint16_t` | [`get_port`](#get_port-1) `const` | The port the server is bound to |
| `const std::string &` | [`get_path`](#get_path) `const` | The path the message endpoint is served on |
| `std::string` | [`get_url`](#get_url-1) `const` | The full URL of the message endpoint |

---

#### MessageProviderServer

`explicit`

```cpp
explicit MessageProviderServer(std::string path = "/__pact/message")
```

Defined in include/verifier.h:264

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `path` | `std::string` | Path the message endpoint is served on |

---

#### ~MessageProviderServer

```cpp
~MessageProviderServer()
```

Defined in include/verifier.h:265

---

#### MessageProviderServer

```cpp
MessageProviderServer(const MessageProviderServer &) = delete
```

Defined in include/verifier.h:267

Deleted constructor.

---

#### operator=

```cpp
MessageProviderServer & operator=(const MessageProviderServer &) = delete
```

Defined in include/verifier.h:268

Deleted assignment operator.

---

#### add_message_handler

```cpp
void add_message_handler(const std::string & description, MessageHandler handler)
```

Defined in include/verifier.h:271

Registers a handler for a single message, keyed on the interaction description

---

#### set_default_handler

```cpp
void set_default_handler(MessageHandler handler)
```

Defined in include/verifier.h:274

Registers a handler invoked for any message without a specific handler

---

#### start

```cpp
bool start(uint16_t port = 0, const std::string & host = "127.0.0.1")
```

Defined in include/verifier.h:280

Starts the server. Pass 0 to let the OS pick a free port. Returns false if the port could not be bound.

---

#### stop

```cpp
void stop()
```

Defined in include/verifier.h:283

Stops the server. Safe to call more than once

---

#### is_running

`const`

```cpp
bool is_running() const
```

Defined in include/verifier.h:286

True while the server is running

---

#### get_port

`const`

```cpp
uint16_t get_port() const
```

Defined in include/verifier.h:289

The port the server is bound to

---

#### get_path

`const`

```cpp
const std::string & get_path() const
```

Defined in include/verifier.h:292

The path the message endpoint is served on

---

#### get_url

`const`

```cpp
std::string get_url() const
```

Defined in include/verifier.h:295

The full URL of the message endpoint

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| std::unique_ptr< [`Impl`](#impl-4) > | [`impl`](#impl-3)  |  |

---

#### impl

```cpp
std::unique_ptr< Impl > impl
```

Type: std::unique_ptr< [`Impl`](#impl-4) >

Defined in include/verifier.h:299



## Impl

```cpp
struct Impl
```

Defined in src/verifier.cpp:441

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`path`](#path-1)  |  |
| [`http::Server`](#server-2) | [`server`](#server-1)  |  |
| std::unordered_map< std::string, [`MessageHandler`](#messagehandler) > | [`handlers`](#handlers-1)  |  |
| [`MessageHandler`](#messagehandler) | [`default_handler`](#default_handler-1)  |  |
| `std::mutex` | [`lock`](#lock-1)  |  |

---

#### path

```cpp
std::string path
```

Defined in src/verifier.cpp:442

---

#### server

```cpp
http::Server server
```

Type: [`http::Server`](#server-2)

Defined in src/verifier.cpp:443

---

#### handlers

```cpp
std::unordered_map< std::string, MessageHandler > handlers
```

Type: std::unordered_map< std::string, [`MessageHandler`](#messagehandler) >

Defined in src/verifier.cpp:444

---

#### default_handler

```cpp
MessageHandler default_handler
```

Type: [`MessageHandler`](#messagehandler)

Defined in src/verifier.cpp:445

---

#### lock

```cpp
std::mutex lock
```

Defined in src/verifier.cpp:446

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Impl`](#impl-5) `inline` `explicit` |  |
| `void` | [`handle`](#handle-1) `inline` |  |

---

#### Impl

`inline` `explicit`

```cpp
inline explicit Impl(std::string endpoint_path)
```

Defined in src/verifier.cpp:448

---

#### handle

`inline`

```cpp
inline void handle(const http::Request & request, http::Response & response)
```

Defined in src/verifier.cpp:454



## VerificationResult

```cpp
#include <verifier.h>
```

```cpp
class VerificationResult
```

Defined in include/verifier.h:348

The outcome of a verification run.

### Friends

| Name | Description |
|------|-------------|
| [`Verifier`](#verifier)  |  |

---

#### Verifier

```cpp
friend class Verifier
```

Defined in include/verifier.h:369

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`is_ok`](#is_ok) `const` | True if every interaction was verified successfully |
| `int` | [`result_code`](#result_code) `const` | Raw exit code from the verifier. Zero means success |
| `const std::string &` | [`output`](#output) `const` | Human readable verifier output, as printed by the Pact CLI verifier |
| `const std::string &` | [`logs`](#logs) `const` | [Verifier](#verifier-1) logs. Only populated if [log_to_buffer()](#log_to_buffer) was called first |
| `const std::string &` | [`json`](#json-2) `const` | Machine readable verification result as a JSON document |
| `void` | [`display`](#display) `const` | Writes the output to standard output |

---

#### is_ok

`const`

```cpp
bool is_ok() const
```

Defined in include/verifier.h:351

True if every interaction was verified successfully

---

#### result_code

`const`

```cpp
int result_code() const
```

Defined in include/verifier.h:354

Raw exit code from the verifier. Zero means success

---

#### output

`const`

```cpp
const std::string & output() const
```

Defined in include/verifier.h:357

Human readable verifier output, as printed by the Pact CLI verifier

---

#### logs

`const`

```cpp
const std::string & logs() const
```

Defined in include/verifier.h:360

[Verifier](#verifier-1) logs. Only populated if [log_to_buffer()](#log_to_buffer) was called first

---

#### json

`const`

```cpp
const std::string & json() const
```

Defined in include/verifier.h:363

Machine readable verification result as a JSON document

---

#### display

`const`

```cpp
void display() const
```

Defined in include/verifier.h:366

Writes the output to standard output

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`code`](#code)  |  |
| `std::string` | [`verifier_output`](#verifier_output)  |  |
| `std::string` | [`verifier_logs`](#verifier_logs)  |  |
| `std::string` | [`verifier_json`](#verifier_json)  |  |

---

#### code

```cpp
int code = -1
```

Defined in include/verifier.h:371

---

#### verifier_output

```cpp
std::string verifier_output
```

Defined in include/verifier.h:372

---

#### verifier_logs

```cpp
std::string verifier_logs
```

Defined in include/verifier.h:373

---

#### verifier_json

```cpp
std::string verifier_json
```

Defined in include/verifier.h:374



## Verifier

```cpp
#include <verifier.h>
```

```cpp
class Verifier
```

Defined in include/verifier.h:396

Verifies that a provider honours the contracts published by its consumers.

Every option exposed by the Pact FFI verifier is available here. Setters return a reference to the verifier so calls can be chained.

```cpp
pact_verifier::Verifier verifier;
verifier
  .set_provider_info("my-provider", "http", "localhost", 8080, "/")
  .add_file_source("./pacts/my-consumer-my-provider.json")
  .add_state_handler("the user exists", [](const auto& request) {
    seed_user(request.params.get_int("id").value_or(0));
  });

auto result = verifier.execute();
result.display();
```

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Verifier`](#verifier-2)  | Creates a verifier. The calling application is reported to the Pact Broker as "pact-cpp" at this library's version, which the broker records against the verification results. |
|  | [`~Verifier`](#verifier-3)  |  |
|  | [`Verifier`](#verifier-4)  | Deleted constructor. |
| [`Verifier`](#verifier-2) & | [`operator=`](#operator-2)  | Deleted assignment operator. |
|  | [`Verifier`](#verifier-5) `noexcept` |  |
| [`Verifier`](#verifier-2) & | [`operator=`](#operator-3) `noexcept` |  |
| [`Verifier`](#verifier-2) & | [`set_provider_info`](#set_provider_info)  | Details of the running provider the interactions are replayed against. Empty strings and a zero port fall back to the Pact defaults ([http://localhost:8080/](http://localhost:8080/)). |
| [`Verifier`](#verifier-2) & | [`add_provider_transport`](#add_provider_transport)  | Adds an additional transport for the provider, for message pacts or plugin based protocols. Use protocol "message" for non-plugin async messages. |
| [`Verifier`](#verifier-2) & | [`add_custom_header`](#add_custom_header)  | Adds a header to every request the verifier makes to the provider |
| [`Verifier`](#verifier-2) & | [`set_follow_redirects`](#set_follow_redirects)  | Whether the verifier should follow redirects returned by the provider. Default true |
| [`Verifier`](#verifier-2) & | [`set_verification_options`](#set_verification_options)  |  |
| [`Verifier`](#verifier-2) & | [`set_provider_state_url`](#set_provider_state_url)  | Tells the verifier to POST (or GET) state change requests to this URL. Use this when your application hosts the state change endpoint itself. |
| [`Verifier`](#verifier-2) & | [`add_state_handler`](#add_state_handler-1)  | Registers a handler for a provider state. The verifier hosts a state change endpoint on your behalf for the duration of [execute()](#execute). |
| [`Verifier`](#verifier-2) & | [`add_state_handler_with_values`](#add_state_handler_with_values-1)  | Registers a handler for a provider state that returns a JSON object of values for the verifier to feed into `fromProviderState` generators. |
| [`Verifier`](#verifier-2) & | [`set_default_state_handler`](#set_default_state_handler)  | Registers a handler used for any state without a specific handler |
| [`Verifier`](#verifier-2) & | [`set_default_state_handler_with_values`](#set_default_state_handler_with_values)  | Registers a value returning handler used for any state without a specific handler |
| [`Verifier`](#verifier-2) & | [`set_state_change_port`](#set_state_change_port)  | Port for the hosted state change endpoint. Defaults to 0, meaning the OS picks a free port. |
| [`Verifier`](#verifier-2) & | [`set_state_change_teardown`](#set_state_change_teardown)  | Whether the hosted endpoint should also receive teardown calls. Default true |
| [`Verifier`](#verifier-2) & | [`set_state_change_body`](#set_state_change_body)  | Whether state change data is sent as a body rather than query params. Default true |
| [`Verifier`](#verifier-2) & | [`add_message_handler`](#add_message_handler-1)  | Registers a handler that produces the message with this description. Works for both asynchronous and synchronous (request/response) messages. |
| [`Verifier`](#verifier-2) & | [`set_default_message_handler`](#set_default_message_handler)  | Registers a handler used for any message without a specific handler |
| [`Verifier`](#verifier-2) & | [`set_message_transport_port`](#set_message_transport_port)  | Port for the hosted message endpoint. Defaults to 0, meaning the OS picks a free port. |
| [`Verifier`](#verifier-2) & | [`set_message_endpoint_path`](#set_message_endpoint_path)  | Path the hosted message endpoint is served on. Defaults to "/__pact/message" |
| [`Verifier`](#verifier-2) & | [`set_filter_info`](#set_filter_info)  | Restricts which interactions are verified. |
| [`Verifier`](#verifier-2) & | [`set_consumer_filters`](#set_consumer_filters)  | Only verify pacts from these consumers |
| [`Verifier`](#verifier-2) & | [`add_file_source`](#add_file_source)  | Verifies a single pact file |
| [`Verifier`](#verifier-2) & | [`add_directory_source`](#add_directory_source)  | Verifies every pact in a directory that matches the provider name |
| [`Verifier`](#verifier-2) & | [`add_url_source`](#add_url_source)  | Fetches a single pact from a URL |
| [`Verifier`](#verifier-2) & | [`add_broker_source`](#add_broker_source)  | Fetches all the latest pacts for the provider from a broker |
| `bool` | [`add_broker_source_with_selectors`](#add_broker_source_with_selectors)  | Fetches pacts from a broker using consumer version selectors, pending pacts and work in progress pacts. |
| [`Verifier`](#verifier-2) & | [`set_publish_options`](#set_publish_options)  | Publishes verification results back to the broker |
| [`Verifier`](#verifier-2) & | [`set_coloured_output`](#set_coloured_output)  | Enables or disables ANSI colour codes in the verifier output. Default true |
| [`Verifier`](#verifier-2) & | [`set_no_pacts_is_error`](#set_no_pacts_is_error)  | Whether finding no pacts to verify is a failure. Default true |
| [`Verifier`](#verifier-2) & | [`set_strip_ansi_from_output`](#set_strip_ansi_from_output)  | Strips ANSI escape codes from [VerificationResult::output](#output). Default false |
| [`VerificationResult`](#verificationresult) | [`execute`](#execute)  | Runs the verification. If any state handlers were registered, the state change endpoint is started before and stopped after the run. |
| `VerifierHandle *` | [`get_handle`](#get_handle) `const` | The underlying FFI handle, for options not yet wrapped by this class |

---

#### Verifier

```cpp
Verifier()
```

Defined in include/verifier.h:403

Creates a verifier. The calling application is reported to the Pact Broker as "pact-cpp" at this library's version, which the broker records against the verification results.

---

#### ~Verifier

```cpp
~Verifier()
```

Defined in include/verifier.h:405

---

#### Verifier

```cpp
Verifier(const Verifier &) = delete
```

Defined in include/verifier.h:407

Deleted constructor.

---

#### operator=

```cpp
Verifier & operator=(const Verifier &) = delete
```

Defined in include/verifier.h:408

Deleted assignment operator.

---

#### Verifier

`noexcept`

```cpp
Verifier(Verifier && other) noexcept
```

Defined in include/verifier.h:409

---

#### operator=

`noexcept`

```cpp
Verifier & operator=(Verifier && other) noexcept
```

Defined in include/verifier.h:410

---

#### set_provider_info

```cpp
Verifier & set_provider_info(const std::string & name, const std::string & scheme = "http", const std::string & host = "localhost", uint16_t port = 8080, const std::string & path = "/")
```

Defined in include/verifier.h:421

Details of the running provider the interactions are replayed against. Empty strings and a zero port fall back to the Pact defaults ([http://localhost:8080/](http://localhost:8080/)).

---

#### add_provider_transport

```cpp
Verifier & add_provider_transport(const std::string & protocol, uint16_t port, const std::string & path = "", const std::string & scheme = "")
```

Defined in include/verifier.h:428

Adds an additional transport for the provider, for message pacts or plugin based protocols. Use protocol "message" for non-plugin async messages.

---

#### add_custom_header

```cpp
Verifier & add_custom_header(const std::string & name, const std::string & value)
```

Defined in include/verifier.h:432

Adds a header to every request the verifier makes to the provider

---

#### set_follow_redirects

```cpp
Verifier & set_follow_redirects(bool follow)
```

Defined in include/verifier.h:435

Whether the verifier should follow redirects returned by the provider. Default true

---

#### set_verification_options

```cpp
Verifier & set_verification_options(bool disable_ssl_verification, unsigned long request_timeout)
```

Defined in include/verifier.h:441

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `disable_ssl_verification` | `bool` | Skip TLS certificate checks, for self signed certs |
| `request_timeout` | `unsigned long` | Timeout in milliseconds for requests to the provider |

---

#### set_provider_state_url

```cpp
Verifier & set_provider_state_url(const std::string & url, bool teardown = true, bool body = true)
```

Defined in include/verifier.h:454

Tells the verifier to POST (or GET) state change requests to this URL. Use this when your application hosts the state change endpoint itself.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `teardown` | `bool` | Also send a teardown request after each interaction |
| `body` | `bool` | Send the state data as a JSON body rather than query parameters |

---

#### add_state_handler

```cpp
Verifier & add_state_handler(const std::string & state, StateHandler handler)
```

Defined in include/verifier.h:460

Registers a handler for a provider state. The verifier hosts a state change endpoint on your behalf for the duration of [execute()](#execute).

---

#### add_state_handler_with_values

```cpp
Verifier & add_state_handler_with_values(const std::string & state, StateHandlerWithValues handler)
```

Defined in include/verifier.h:466

Registers a handler for a provider state that returns a JSON object of values for the verifier to feed into `fromProviderState` generators.

---

#### set_default_state_handler

```cpp
Verifier & set_default_state_handler(StateHandler handler)
```

Defined in include/verifier.h:469

Registers a handler used for any state without a specific handler

---

#### set_default_state_handler_with_values

```cpp
Verifier & set_default_state_handler_with_values(StateHandlerWithValues handler)
```

Defined in include/verifier.h:472

Registers a value returning handler used for any state without a specific handler

---

#### set_state_change_port

```cpp
Verifier & set_state_change_port(uint16_t port)
```

Defined in include/verifier.h:478

Port for the hosted state change endpoint. Defaults to 0, meaning the OS picks a free port.

---

#### set_state_change_teardown

```cpp
Verifier & set_state_change_teardown(bool teardown)
```

Defined in include/verifier.h:481

Whether the hosted endpoint should also receive teardown calls. Default true

---

#### set_state_change_body

```cpp
Verifier & set_state_change_body(bool body)
```

Defined in include/verifier.h:484

Whether state change data is sent as a body rather than query params. Default true

---

#### add_message_handler

```cpp
Verifier & add_message_handler(const std::string & description, MessageHandler handler)
```

Defined in include/verifier.h:497

Registers a handler that produces the message with this description. Works for both asynchronous and synchronous (request/response) messages.

The verifier hosts a "message" transport endpoint on your behalf for the duration of [execute()](#execute), so you do not need to call add_provider_transport.

---

#### set_default_message_handler

```cpp
Verifier & set_default_message_handler(MessageHandler handler)
```

Defined in include/verifier.h:500

Registers a handler used for any message without a specific handler

---

#### set_message_transport_port

```cpp
Verifier & set_message_transport_port(uint16_t port)
```

Defined in include/verifier.h:506

Port for the hosted message endpoint. Defaults to 0, meaning the OS picks a free port.

---

#### set_message_endpoint_path

```cpp
Verifier & set_message_endpoint_path(const std::string & path)
```

Defined in include/verifier.h:509

Path the hosted message endpoint is served on. Defaults to "/__pact/message"

---

#### set_filter_info

```cpp
Verifier & set_filter_info(const std::string & filter_description = "", const std::string & filter_state = "", bool filter_no_state = false)
```

Defined in include/verifier.h:522

Restricts which interactions are verified.

##### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `filter_description` | `const std::string &` | Regular expression matched against the interaction description |
| `filter_state` | `const std::string &` | Regular expression matched against the provider state |
| `filter_no_state` | `bool` | Only verify interactions that have no provider state |

---

#### set_consumer_filters

```cpp
Verifier & set_consumer_filters(const std::vector< std::string > & consumers)
```

Defined in include/verifier.h:526

Only verify pacts from these consumers

---

#### add_file_source

```cpp
Verifier & add_file_source(const std::string & file)
```

Defined in include/verifier.h:533

Verifies a single pact file

---

#### add_directory_source

```cpp
Verifier & add_directory_source(const std::string & directory)
```

Defined in include/verifier.h:536

Verifies every pact in a directory that matches the provider name

---

#### add_url_source

```cpp
Verifier & add_url_source(const std::string & url, const std::string & username = "", const std::string & password = "", const std::string & token = "")
```

Defined in include/verifier.h:539

Fetches a single pact from a URL

---

#### add_broker_source

```cpp
Verifier & add_broker_source(const std::string & url, const std::string & username = "", const std::string & password = "", const std::string & token = "")
```

Defined in include/verifier.h:543

Fetches all the latest pacts for the provider from a broker

---

#### add_broker_source_with_selectors

```cpp
bool add_broker_source_with_selectors(const BrokerSelectorOptions & options)
```

Defined in include/verifier.h:552

Fetches pacts from a broker using consumer version selectors, pending pacts and work in progress pacts.

Returns false if any of the consumer version selectors were not valid JSON.

---

#### set_publish_options

```cpp
Verifier & set_publish_options(const PublishOptions & options)
```

Defined in include/verifier.h:559

Publishes verification results back to the broker

---

#### set_coloured_output

```cpp
Verifier & set_coloured_output(bool coloured)
```

Defined in include/verifier.h:562

Enables or disables ANSI colour codes in the verifier output. Default true

---

#### set_no_pacts_is_error

```cpp
Verifier & set_no_pacts_is_error(bool is_error)
```

Defined in include/verifier.h:565

Whether finding no pacts to verify is a failure. Default true

---

#### set_strip_ansi_from_output

```cpp
Verifier & set_strip_ansi_from_output(bool strip)
```

Defined in include/verifier.h:568

Strips ANSI escape codes from [VerificationResult::output](#output). Default false

---

#### execute

```cpp
VerificationResult execute()
```

Defined in include/verifier.h:574

Runs the verification. If any state handlers were registered, the state change endpoint is started before and stopped after the run.

---

#### get_handle

`const`

```cpp
VerifierHandle * get_handle() const
```

Defined in include/verifier.h:580

The underlying FFI handle, for options not yet wrapped by this class

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`logs_for_provider`](#logs_for_provider) `static` | Fetches buffered logs for a provider by name, outside of a verification run |

---

#### logs_for_provider

`static`

```cpp
static std::string logs_for_provider(const std::string & provider_name)
```

Defined in include/verifier.h:577

Fetches buffered logs for a provider by name, outside of a verification run

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| std::unique_ptr< [`Impl`](#impl-7) > | [`impl`](#impl-6)  |  |

---

#### impl

```cpp
std::unique_ptr< Impl > impl
```

Type: std::unique_ptr< [`Impl`](#impl-7) >

Defined in include/verifier.h:584



## Impl

```cpp
struct Impl
```

Defined in src/verifier.cpp:615

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `VerifierHandle *` | [`handle`](#handle-2)  |  |
| std::unique_ptr< [`ProviderStateServer`](#providerstateserver) > | [`state_server`](#state_server)  |  |
| `bool` | [`has_state_handlers`](#has_state_handlers)  |  |
| `bool` | [`state_change_teardown`](#state_change_teardown)  |  |
| `bool` | [`state_change_body`](#state_change_body)  |  |
| `uint16_t` | [`state_change_port`](#state_change_port)  |  |
| std::unique_ptr< [`MessageProviderServer`](#messageproviderserver) > | [`message_server`](#message_server)  |  |
| `bool` | [`has_message_handlers`](#has_message_handlers)  |  |
| `uint16_t` | [`message_transport_port`](#message_transport_port)  |  |
| `std::string` | [`message_endpoint_path`](#message_endpoint_path)  |  |
| `std::string` | [`provider_host`](#provider_host)  |  |
| `bool` | [`strip_ansi`](#strip_ansi)  |  |

---

#### handle

```cpp
VerifierHandle * handle = nullptr
```

Defined in src/verifier.cpp:616

---

#### state_server

```cpp
std::unique_ptr< ProviderStateServer > state_server
```

Type: std::unique_ptr< [`ProviderStateServer`](#providerstateserver) >

Defined in src/verifier.cpp:617

---

#### has_state_handlers

```cpp
bool has_state_handlers = false
```

Defined in src/verifier.cpp:618

---

#### state_change_teardown

```cpp
bool state_change_teardown = true
```

Defined in src/verifier.cpp:619

---

#### state_change_body

```cpp
bool state_change_body = true
```

Defined in src/verifier.cpp:620

---

#### state_change_port

```cpp
uint16_t state_change_port = 0
```

Defined in src/verifier.cpp:621

---

#### message_server

```cpp
std::unique_ptr< MessageProviderServer > message_server
```

Type: std::unique_ptr< [`MessageProviderServer`](#messageproviderserver) >

Defined in src/verifier.cpp:622

---

#### has_message_handlers

```cpp
bool has_message_handlers = false
```

Defined in src/verifier.cpp:623

---

#### message_transport_port

```cpp
uint16_t message_transport_port = 0
```

Defined in src/verifier.cpp:624

---

#### message_endpoint_path

```cpp
std::string message_endpoint_path = "/__pact/message"
```

Defined in src/verifier.cpp:625

---

#### provider_host

```cpp
std::string provider_host = "127.0.0.1"
```

Defined in src/verifier.cpp:626

---

#### strip_ansi

```cpp
bool strip_ansi = false
```

Defined in src/verifier.cpp:627

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`bind_host`](#bind_host) `const` `inline` | The hosted state change and message endpoints have to be reachable at the provider host, but only an IPv4 literal can be bound directly. |
| [`ProviderStateServer`](#providerstateserver) & | [`states`](#states) `inline` |  |
| [`MessageProviderServer`](#messageproviderserver) & | [`messages`](#messages) `inline` |  |

---

#### bind_host

`const` `inline`

```cpp
inline std::string bind_host() const
```

Defined in src/verifier.cpp:633

The hosted state change and message endpoints have to be reachable at the provider host, but only an IPv4 literal can be bound directly.

---

#### states

`inline`

```cpp
inline ProviderStateServer & states()
```

Defined in src/verifier.cpp:645

---

#### messages

`inline`

```cpp
inline MessageProviderServer & messages()
```

Defined in src/verifier.cpp:653



## ProviderStateRequest

```cpp
#include <verifier.h>
```

```cpp
struct ProviderStateRequest
```

Defined in include/verifier.h:103

A provider state change request received from the verifier.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`state`](#state)  | The provider state description, e.g. "the user exists" |
| [`StateAction`](#stateaction) | [`action`](#action)  | Whether this is a setup or teardown call |
| [`ProviderStateParams`](#providerstateparams) | [`params`](#params-1)  | Any parameters the consumer attached to the state |

---

#### state

```cpp
std::string state
```

Defined in include/verifier.h:105

The provider state description, e.g. "the user exists"

---

#### action

```cpp
StateAction action = StateAction::Setup
```

Type: [`StateAction`](#stateaction)

Defined in include/verifier.h:107

Whether this is a setup or teardown call

---

#### params

```cpp
ProviderStateParams params
```

Type: [`ProviderStateParams`](#providerstateparams)

Defined in include/verifier.h:109

Any parameters the consumer attached to the state



## MessageProviderState

```cpp
#include <verifier.h>
```

```cpp
struct MessageProviderState
```

Defined in include/verifier.h:219

A provider state attached to a message interaction.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`name`](#name)  |  |
| [`ProviderStateParams`](#providerstateparams) | [`params`](#params-2)  |  |

---

#### name

```cpp
std::string name
```

Defined in include/verifier.h:220

---

#### params

```cpp
ProviderStateParams params
```

Type: [`ProviderStateParams`](#providerstateparams)

Defined in include/verifier.h:221



## MessageRequest

```cpp
#include <verifier.h>
```

```cpp
struct MessageRequest
```

Defined in include/verifier.h:227

The verifier asking the provider to produce a message.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`description`](#description)  | The interaction description from the pact file |
| std::vector< [`MessageProviderState`](#messageproviderstate) > | [`provider_states`](#provider_states)  | Provider states attached to the interaction |
| `bool` | [`synchronous`](#synchronous)  | True for a synchronous (request/response) message, false for an asynchronous one |
| `std::string` | [`request_json`](#request_json)  | For synchronous messages, the request message the consumer sends, as raw JSON |
| `std::string` | [`request_body`](#request_body)  | For synchronous messages, the decoded body of the request message |
| `std::string` | [`request_content_type`](#request_content_type)  | For synchronous messages, the content type of the request message |

---

#### description

```cpp
std::string description
```

Defined in include/verifier.h:229

The interaction description from the pact file

---

#### provider_states

```cpp
std::vector< MessageProviderState > provider_states
```

Type: std::vector< [`MessageProviderState`](#messageproviderstate) >

Defined in include/verifier.h:232

Provider states attached to the interaction

---

#### synchronous

```cpp
bool synchronous = false
```

Defined in include/verifier.h:235

True for a synchronous (request/response) message, false for an asynchronous one

---

#### request_json

```cpp
std::string request_json
```

Defined in include/verifier.h:238

For synchronous messages, the request message the consumer sends, as raw JSON

---

#### request_body

```cpp
std::string request_body
```

Defined in include/verifier.h:241

For synchronous messages, the decoded body of the request message

---

#### request_content_type

```cpp
std::string request_content_type
```

Defined in include/verifier.h:244

For synchronous messages, the content type of the request message



## BrokerSelectorOptions

```cpp
#include <verifier.h>
```

```cpp
struct BrokerSelectorOptions
```

Defined in include/verifier.h:308

Options for fetching pacts from a Pact Broker or PactFlow using consumer version selectors.

**See also**: [https://docs.pact.io/pact_broker/advanced_topics/consumer_version_selectors](https://docs.pact.io/pact_broker/advanced_topics/consumer_version_selectors)

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`url`](#url)  | Base URL of the broker, e.g. "https://my-org.pactflow.io" |
| `std::string` | [`username`](#username)  | Basic auth username. Leave empty to omit |
| `std::string` | [`password`](#password)  | Basic auth password. Leave empty to omit |
| `std::string` | [`token`](#token)  | Bearer token. Leave empty to omit. Mutually exclusive with username/password |
| `bool` | [`enable_pending`](#enable_pending)  | Include pacts that are in pending state, so failures do not break the build |
| `std::string` | [`include_wip_pacts_since`](#include_wip_pacts_since)  | ISO date (YYYY-MM-DD). Include work in progress pacts created since this date |
| `std::vector< std::string >` | [`provider_tags`](#provider_tags)  | Tags to apply to the provider version |
| `std::string` | [`provider_branch`](#provider_branch)  | Branch the provider is being verified from |
| `std::vector< std::string >` | [`consumer_version_selectors`](#consumer_version_selectors)  | Consumer version selectors, each as a JSON object string |
| `std::vector< std::string >` | [`consumer_version_tags`](#consumer_version_tags)  | Legacy consumer version tags. Prefer consumer_version_selectors |

---

#### url

```cpp
std::string url
```

Defined in include/verifier.h:310

Base URL of the broker, e.g. "https://my-org.pactflow.io"

---

#### username

```cpp
std::string username
```

Defined in include/verifier.h:312

Basic auth username. Leave empty to omit

---

#### password

```cpp
std::string password
```

Defined in include/verifier.h:314

Basic auth password. Leave empty to omit

---

#### token

```cpp
std::string token
```

Defined in include/verifier.h:316

Bearer token. Leave empty to omit. Mutually exclusive with username/password

---

#### enable_pending

```cpp
bool enable_pending = false
```

Defined in include/verifier.h:318

Include pacts that are in pending state, so failures do not break the build

---

#### include_wip_pacts_since

```cpp
std::string include_wip_pacts_since
```

Defined in include/verifier.h:320

ISO date (YYYY-MM-DD). Include work in progress pacts created since this date

---

#### provider_tags

```cpp
std::vector< std::string > provider_tags
```

Defined in include/verifier.h:322

Tags to apply to the provider version

---

#### provider_branch

```cpp
std::string provider_branch
```

Defined in include/verifier.h:324

Branch the provider is being verified from

---

#### consumer_version_selectors

```cpp
std::vector< std::string > consumer_version_selectors
```

Defined in include/verifier.h:326

Consumer version selectors, each as a JSON object string

---

#### consumer_version_tags

```cpp
std::vector< std::string > consumer_version_tags
```

Defined in include/verifier.h:328

Legacy consumer version tags. Prefer consumer_version_selectors



## PublishOptions

```cpp
#include <verifier.h>
```

```cpp
struct PublishOptions
```

Defined in include/verifier.h:334

Options for publishing verification results back to the broker.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`provider_version`](#provider_version)  | Version of the provider being verified, e.g. a git SHA |
| `std::string` | [`build_url`](#build_url)  | URL of the CI build that ran the verification. Optional |
| `std::vector< std::string >` | [`provider_tags`](#provider_tags-1)  | Tags to apply to the provider version. Optional |
| `std::string` | [`provider_branch`](#provider_branch-1)  | Branch the provider is being verified from. Optional |

---

#### provider_version

```cpp
std::string provider_version
```

Defined in include/verifier.h:336

Version of the provider being verified, e.g. a git SHA

---

#### build_url

```cpp
std::string build_url
```

Defined in include/verifier.h:338

URL of the CI build that ran the verification. Optional

---

#### provider_tags

```cpp
std::vector< std::string > provider_tags
```

Defined in include/verifier.h:340

Tags to apply to the provider version. Optional

---

#### provider_branch

```cpp
std::string provider_branch
```

Defined in include/verifier.h:342

Branch the provider is being verified from. Optional



## http

### Classes

| Name | Description |
|------|-------------|
| [`Server`](#server-2) | A small HTTP server that runs on a background thread. |
| [`Request`](#request) | An incoming HTTP request. |
| [`Response`](#response) | The response to send back. Handlers mutate this in place. |

### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| std::function< void(const [`Request`](#request) &, [`Response`](#response) &)> | [`Handler`](#handler)  | Callback invoked for every request the server receives. |

---

#### Handler

```cpp
using Handler = std::function< void(const Request &, Response &)>
```

Type: std::function< void(const [`Request`](#request) &, [`Response`](#response) &)>

Defined in include/http_server.h:57

Callback invoked for every request the server receives.



## Server

```cpp
#include <http_server.h>
```

```cpp
class Server
```

Defined in include/http_server.h:62

A small HTTP server that runs on a background thread.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Server`](#server-3)  |  |
|  | [`~Server`](#server-4)  |  |
|  | [`Server`](#server-5)  | Deleted constructor. |
| [`Server`](#server-3) & | [`operator=`](#operator-4)  | Deleted assignment operator. |
| `void` | [`set_handler`](#set_handler)  | Sets the handler invoked for each request. Must be called before [start()](#start-2). |
| `bool` | [`start`](#start-2)  | Binds and starts listening on the given host and port, then serves requests on a background thread. Passing port 0 will allocate a free port, which can be read back with [get_port()](#get_port-2). |
| `void` | [`stop`](#stop-2)  | Stops the server and joins the background thread. Safe to call more than once. |
| `bool` | [`is_running`](#is_running-2) `const` | True while the server is accepting requests. |
| `uint16_t` | [`get_port`](#get_port-2) `const` | The port the server is bound to, or 0 if it is not running. |
| `std::string` | [`get_url`](#get_url-2) `const` | The base URL of the server, for example "http://127.0.0.1:52344". |

---

#### Server

```cpp
Server()
```

Defined in include/http_server.h:64

---

#### ~Server

```cpp
~Server()
```

Defined in include/http_server.h:65

---

#### Server

```cpp
Server(const Server &) = delete
```

Defined in include/http_server.h:67

Deleted constructor.

---

#### operator=

```cpp
Server & operator=(const Server &) = delete
```

Defined in include/http_server.h:68

Deleted assignment operator.

---

#### set_handler

```cpp
void set_handler(Handler handler)
```

Defined in include/http_server.h:73

Sets the handler invoked for each request. Must be called before [start()](#start-2).

---

#### start

```cpp
bool start(uint16_t port = 0, const std::string & host = "127.0.0.1")
```

Defined in include/http_server.h:82

Binds and starts listening on the given host and port, then serves requests on a background thread. Passing port 0 will allocate a free port, which can be read back with [get_port()](#get_port-2).

Returns false if the socket could not be bound.

---

#### stop

```cpp
void stop()
```

Defined in include/http_server.h:87

Stops the server and joins the background thread. Safe to call more than once.

---

#### is_running

`const`

```cpp
bool is_running() const
```

Defined in include/http_server.h:92

True while the server is accepting requests.

---

#### get_port

`const`

```cpp
uint16_t get_port() const
```

Defined in include/http_server.h:97

The port the server is bound to, or 0 if it is not running.

---

#### get_url

`const`

```cpp
std::string get_url() const
```

Defined in include/http_server.h:102

The base URL of the server, for example "http://127.0.0.1:52344".

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| std::unique_ptr< [`Impl`](#impl-9) > | [`impl`](#impl-8)  |  |

---

#### impl

```cpp
std::unique_ptr< Impl > impl
```

Type: std::unique_ptr< [`Impl`](#impl-9) >

Defined in include/http_server.h:106



## Impl

```cpp
struct Impl
```

Defined in src/http_server.cpp:261

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| [`pact_socket_t`](#pact_socket_t) | [`listener`](#listener)  |  |
| `std::atomic< bool >` | [`running`](#running)  |  |
| `std::thread` | [`worker`](#worker)  |  |
| [`Handler`](#handler) | [`handler`](#handler-1)  |  |
| `uint16_t` | [`port`](#port)  |  |
| `std::string` | [`host`](#host)  |  |

---

#### listener

```cpp
pact_socket_t listener = PACT_INVALID_SOCKET
```

Type: [`pact_socket_t`](#pact_socket_t)

Defined in src/http_server.cpp:262

---

#### running

```cpp
std::atomic< bool > running {false}
```

Defined in src/http_server.cpp:263

---

#### worker

```cpp
std::thread worker
```

Defined in src/http_server.cpp:264

---

#### handler

```cpp
Handler handler
```

Type: [`Handler`](#handler)

Defined in src/http_server.cpp:265

---

#### port

```cpp
uint16_t port = 0
```

Defined in src/http_server.cpp:266

---

#### host

```cpp
std::string host
```

Defined in src/http_server.cpp:267

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`serve`](#serve) `inline` |  |

---

#### serve

`inline`

```cpp
inline void serve()
```

Defined in src/http_server.cpp:269



## Request

```cpp
#include <http_server.h>
```

```cpp
struct Request
```

Defined in include/http_server.h:25

An incoming HTTP request.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`method`](#method)  | HTTP method, upper case (GET, POST, ...) |
| `std::string` | [`path`](#path-2)  | [Request](#request) path, with any query string removed |
| `std::string` | [`query`](#query)  | Raw query string (without the leading '?') |
| `std::string` | [`body`](#body-1)  | [Request](#request) body. Empty if the request had no body |
| `std::unordered_map< std::string, std::string >` | [`headers`](#headers)  | [Request](#request) headers. Header names are lower cased |
| `std::unordered_map< std::string, std::string >` | [`query_params`](#query_params)  | URL decoded query parameters |

---

#### method

```cpp
std::string method
```

Defined in include/http_server.h:27

HTTP method, upper case (GET, POST, ...)

---

#### path

```cpp
std::string path
```

Defined in include/http_server.h:29

[Request](#request) path, with any query string removed

---

#### query

```cpp
std::string query
```

Defined in include/http_server.h:31

Raw query string (without the leading '?')

---

#### body

```cpp
std::string body
```

Defined in include/http_server.h:33

[Request](#request) body. Empty if the request had no body

---

#### headers

```cpp
std::unordered_map< std::string, std::string > headers
```

Defined in include/http_server.h:35

[Request](#request) headers. Header names are lower cased

---

#### query_params

```cpp
std::unordered_map< std::string, std::string > query_params
```

Defined in include/http_server.h:37

URL decoded query parameters



## Response

```cpp
#include <http_server.h>
```

```cpp
struct Response
```

Defined in include/http_server.h:43

The response to send back. Handlers mutate this in place.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `unsigned int` | [`status`](#status)  | HTTP status code |
| `std::string` | [`body`](#body-2)  | [Response](#response) body |
| `std::string` | [`content_type`](#content_type-1)  | Value for the Content-Type header. Omitted if empty |
| `std::unordered_map< std::string, std::string >` | [`headers`](#headers-1)  | Any additional response headers |

---

#### status

```cpp
unsigned int status = 200
```

Defined in include/http_server.h:45

HTTP status code

---

#### body

```cpp
std::string body
```

Defined in include/http_server.h:47

[Response](#response) body

---

#### content_type

```cpp
std::string content_type = "application/json"
```

Defined in include/http_server.h:49

Value for the Content-Type header. Omitted if empty

---

#### headers

```cpp
std::unordered_map< std::string, std::string > headers
```

Defined in include/http_server.h:51

Any additional response headers



## Impl

```cpp
struct Impl
```

Defined in src/verifier.cpp:615

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `VerifierHandle *` | [`handle`](#handle-2)  |  |
| std::unique_ptr< [`ProviderStateServer`](#providerstateserver) > | [`state_server`](#state_server)  |  |
| `bool` | [`has_state_handlers`](#has_state_handlers)  |  |
| `bool` | [`state_change_teardown`](#state_change_teardown)  |  |
| `bool` | [`state_change_body`](#state_change_body)  |  |
| `uint16_t` | [`state_change_port`](#state_change_port)  |  |
| std::unique_ptr< [`MessageProviderServer`](#messageproviderserver) > | [`message_server`](#message_server)  |  |
| `bool` | [`has_message_handlers`](#has_message_handlers)  |  |
| `uint16_t` | [`message_transport_port`](#message_transport_port)  |  |
| `std::string` | [`message_endpoint_path`](#message_endpoint_path)  |  |
| `std::string` | [`provider_host`](#provider_host)  |  |
| `bool` | [`strip_ansi`](#strip_ansi)  |  |

---

#### handle

```cpp
VerifierHandle * handle = nullptr
```

Defined in src/verifier.cpp:616

---

#### state_server

```cpp
std::unique_ptr< ProviderStateServer > state_server
```

Type: std::unique_ptr< [`ProviderStateServer`](#providerstateserver) >

Defined in src/verifier.cpp:617

---

#### has_state_handlers

```cpp
bool has_state_handlers = false
```

Defined in src/verifier.cpp:618

---

#### state_change_teardown

```cpp
bool state_change_teardown = true
```

Defined in src/verifier.cpp:619

---

#### state_change_body

```cpp
bool state_change_body = true
```

Defined in src/verifier.cpp:620

---

#### state_change_port

```cpp
uint16_t state_change_port = 0
```

Defined in src/verifier.cpp:621

---

#### message_server

```cpp
std::unique_ptr< MessageProviderServer > message_server
```

Type: std::unique_ptr< [`MessageProviderServer`](#messageproviderserver) >

Defined in src/verifier.cpp:622

---

#### has_message_handlers

```cpp
bool has_message_handlers = false
```

Defined in src/verifier.cpp:623

---

#### message_transport_port

```cpp
uint16_t message_transport_port = 0
```

Defined in src/verifier.cpp:624

---

#### message_endpoint_path

```cpp
std::string message_endpoint_path = "/__pact/message"
```

Defined in src/verifier.cpp:625

---

#### provider_host

```cpp
std::string provider_host = "127.0.0.1"
```

Defined in src/verifier.cpp:626

---

#### strip_ansi

```cpp
bool strip_ansi = false
```

Defined in src/verifier.cpp:627

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`bind_host`](#bind_host) `const` `inline` | The hosted state change and message endpoints have to be reachable at the provider host, but only an IPv4 literal can be bound directly. |
| [`ProviderStateServer`](#providerstateserver) & | [`states`](#states) `inline` |  |
| [`MessageProviderServer`](#messageproviderserver) & | [`messages`](#messages) `inline` |  |

---

#### bind_host

`const` `inline`

```cpp
inline std::string bind_host() const
```

Defined in src/verifier.cpp:633

The hosted state change and message endpoints have to be reachable at the provider host, but only an IPv4 literal can be bound directly.

---

#### states

`inline`

```cpp
inline ProviderStateServer & states()
```

Defined in src/verifier.cpp:645

---

#### messages

`inline`

```cpp
inline MessageProviderServer & messages()
```

Defined in src/verifier.cpp:653



## Impl

```cpp
struct Impl
```

Defined in src/http_server.cpp:261

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| [`pact_socket_t`](#pact_socket_t) | [`listener`](#listener)  |  |
| `std::atomic< bool >` | [`running`](#running)  |  |
| `std::thread` | [`worker`](#worker)  |  |
| [`Handler`](#handler) | [`handler`](#handler-1)  |  |
| `uint16_t` | [`port`](#port)  |  |
| `std::string` | [`host`](#host)  |  |

---

#### listener

```cpp
pact_socket_t listener = PACT_INVALID_SOCKET
```

Type: [`pact_socket_t`](#pact_socket_t)

Defined in src/http_server.cpp:262

---

#### running

```cpp
std::atomic< bool > running {false}
```

Defined in src/http_server.cpp:263

---

#### worker

```cpp
std::thread worker
```

Defined in src/http_server.cpp:264

---

#### handler

```cpp
Handler handler
```

Type: [`Handler`](#handler)

Defined in src/http_server.cpp:265

---

#### port

```cpp
uint16_t port = 0
```

Defined in src/http_server.cpp:266

---

#### host

```cpp
std::string host
```

Defined in src/http_server.cpp:267

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`serve`](#serve) `inline` |  |

---

#### serve

`inline`

```cpp
inline void serve()
```

Defined in src/http_server.cpp:269



## Impl

```cpp
struct Impl
```

Defined in src/verifier.cpp:215

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`path`](#path)  |  |
| [`http::Server`](#server-2) | [`server`](#server)  |  |
| std::unordered_map< std::string, [`StateHandlerWithValues`](#statehandlerwithvalues) > | [`handlers`](#handlers)  |  |
| [`StateHandlerWithValues`](#statehandlerwithvalues) | [`default_handler`](#default_handler)  |  |
| `bool` | [`unknown_state_is_error`](#unknown_state_is_error)  |  |
| `std::mutex` | [`lock`](#lock)  |  |

---

#### path

```cpp
std::string path
```

Defined in src/verifier.cpp:216

---

#### server

```cpp
http::Server server
```

Type: [`http::Server`](#server-2)

Defined in src/verifier.cpp:217

---

#### handlers

```cpp
std::unordered_map< std::string, StateHandlerWithValues > handlers
```

Type: std::unordered_map< std::string, [`StateHandlerWithValues`](#statehandlerwithvalues) >

Defined in src/verifier.cpp:218

---

#### default_handler

```cpp
StateHandlerWithValues default_handler
```

Type: [`StateHandlerWithValues`](#statehandlerwithvalues)

Defined in src/verifier.cpp:219

---

#### unknown_state_is_error

```cpp
bool unknown_state_is_error = true
```

Defined in src/verifier.cpp:220

---

#### lock

```cpp
std::mutex lock
```

Defined in src/verifier.cpp:221

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Impl`](#impl-2) `inline` `explicit` |  |
| `void` | [`handle`](#handle) `inline` |  |

---

#### Impl

`inline` `explicit`

```cpp
inline explicit Impl(std::string endpoint_path)
```

Defined in src/verifier.cpp:223

---

#### handle

`inline`

```cpp
inline void handle(const http::Request & request, http::Response & response)
```

Defined in src/verifier.cpp:229



## Impl

```cpp
struct Impl
```

Defined in src/verifier.cpp:441

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`path`](#path-1)  |  |
| [`http::Server`](#server-2) | [`server`](#server-1)  |  |
| std::unordered_map< std::string, [`MessageHandler`](#messagehandler) > | [`handlers`](#handlers-1)  |  |
| [`MessageHandler`](#messagehandler) | [`default_handler`](#default_handler-1)  |  |
| `std::mutex` | [`lock`](#lock-1)  |  |

---

#### path

```cpp
std::string path
```

Defined in src/verifier.cpp:442

---

#### server

```cpp
http::Server server
```

Type: [`http::Server`](#server-2)

Defined in src/verifier.cpp:443

---

#### handlers

```cpp
std::unordered_map< std::string, MessageHandler > handlers
```

Type: std::unordered_map< std::string, [`MessageHandler`](#messagehandler) >

Defined in src/verifier.cpp:444

---

#### default_handler

```cpp
MessageHandler default_handler
```

Type: [`MessageHandler`](#messagehandler)

Defined in src/verifier.cpp:445

---

#### lock

```cpp
std::mutex lock
```

Defined in src/verifier.cpp:446

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Impl`](#impl-5) `inline` `explicit` |  |
| `void` | [`handle`](#handle-1) `inline` |  |

---

#### Impl

`inline` `explicit`

```cpp
inline explicit Impl(std::string endpoint_path)
```

Defined in src/verifier.cpp:448

---

#### handle

`inline`

```cpp
inline void handle(const http::Request & request, http::Response & response)
```

Defined in src/verifier.cpp:454

Generated by [Moxygen](https://0state.com/moxygen)