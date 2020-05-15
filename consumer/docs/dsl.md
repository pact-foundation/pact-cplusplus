# namespace `pact_consumer`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[``matchers``](#namespacepact__consumer_1_1matchers)    | 
`class `[``Interaction``](#classpact__consumer_1_1Interaction)    | 
`class `[``MockServerHandle``](#classpact__consumer_1_1MockServerHandle)    | 
`class `[``Pact``](#classpact__consumer_1_1Pact)    | 
`class `[``PactJsonBuilder``](#classpact__consumer_1_1PactJsonBuilder)    | 
`class `[``PactTestResult``](#classpact__consumer_1_1PactTestResult)    | 

# namespace `matchers`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[``DateMatcher``](#classpact__consumer_1_1matchers_1_1DateMatcher)    | 
`class `[``DateTimeMatcher``](#classpact__consumer_1_1matchers_1_1DateTimeMatcher)    | 
`class `[``DecimalMatcher``](#classpact__consumer_1_1matchers_1_1DecimalMatcher)    | 
`class `[``EachlikeMatcher``](#classpact__consumer_1_1matchers_1_1EachlikeMatcher)    | 
`class `[``EqualsMatcher``](#classpact__consumer_1_1matchers_1_1EqualsMatcher)    | 
`class `[``HexadecimalMatcher``](#classpact__consumer_1_1matchers_1_1HexadecimalMatcher)    | 
`class `[``IMatcher``](#classpact__consumer_1_1matchers_1_1IMatcher)    | 
`class `[``IncludesMatcher``](#classpact__consumer_1_1matchers_1_1IncludesMatcher)    | 
`class `[``IntegerMatcher``](#classpact__consumer_1_1matchers_1_1IntegerMatcher)    | 
`class `[``IPAddressMatcher``](#classpact__consumer_1_1matchers_1_1IPAddressMatcher)    | 
`class `[``NullMatcher``](#classpact__consumer_1_1matchers_1_1NullMatcher)    | 
`class `[``NumberMatcher``](#classpact__consumer_1_1matchers_1_1NumberMatcher)    | 
`class `[``ObjectMatcher``](#classpact__consumer_1_1matchers_1_1ObjectMatcher)    | 
`class `[``RegexMatcher``](#classpact__consumer_1_1matchers_1_1RegexMatcher)    | 
`class `[``TimeMatcher``](#classpact__consumer_1_1matchers_1_1TimeMatcher)    | 
`class `[``TypeMatcher``](#classpact__consumer_1_1matchers_1_1TypeMatcher)    | 
`class `[``UrlMatcher``](#classpact__consumer_1_1matchers_1_1UrlMatcher)    | 
`class `[``UuidMatcher``](#classpact__consumer_1_1matchers_1_1UuidMatcher)    | 

# class `DateMatcher` {#classpact__consumer_1_1matchers_1_1DateMatcher}

```
class DateMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  DateMatcher(std::string f)` | 
`public inline  DateMatcher(std::string f,std::string e)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  DateMatcher(std::string f)` {#classpact__consumer_1_1matchers_1_1DateMatcher_1a6723b3dd8546a852f01a26958c5600a5}





### `public inline  DateMatcher(std::string f,std::string e)` {#classpact__consumer_1_1matchers_1_1DateMatcher_1a1154eab79153a1e0b89287b4712577c0}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1DateMatcher_1a607ec8245653311981a48af6d8a4af01}






# class `DateTimeMatcher` {#classpact__consumer_1_1matchers_1_1DateTimeMatcher}

```
class DateTimeMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  DateTimeMatcher(std::string f)` | 
`public inline  DateTimeMatcher(std::string f,std::string e)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  DateTimeMatcher(std::string f)` {#classpact__consumer_1_1matchers_1_1DateTimeMatcher_1a82391f7c27b81bf856541e2f9535c7ed}





### `public inline  DateTimeMatcher(std::string f,std::string e)` {#classpact__consumer_1_1matchers_1_1DateTimeMatcher_1a624fefce3ecef9cc4ec811905cc061d3}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1DateTimeMatcher_1a4bfa2b1633fc9a411fc0bb4ae5257cb6}






# class `DecimalMatcher` {#classpact__consumer_1_1matchers_1_1DecimalMatcher}

```
class DecimalMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  DecimalMatcher()` | 
`public inline  DecimalMatcher(float v)` | 
`public inline  DecimalMatcher(double v)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  DecimalMatcher()` {#classpact__consumer_1_1matchers_1_1DecimalMatcher_1a8cc13a35a6c01a7ad2034db6499d8148}





### `public inline  DecimalMatcher(float v)` {#classpact__consumer_1_1matchers_1_1DecimalMatcher_1a718f029d1aee790d29882e21c2d30bb6}





### `public inline  DecimalMatcher(double v)` {#classpact__consumer_1_1matchers_1_1DecimalMatcher_1aee590f8b4474b327b4ead0c65ede152f}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1DecimalMatcher_1a7d54db15ef35b2eb827f0fa9f3e67403}






# class `EachlikeMatcher` {#classpact__consumer_1_1matchers_1_1EachlikeMatcher}

```
class EachlikeMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  EachlikeMatcher(`[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` t)` | 
`public inline  EachlikeMatcher(int e,`[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` t)` | 
`public inline  EachlikeMatcher(int e,int m,`[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` t)` | 
`public inline  EachlikeMatcher(int e,int m,int mx,`[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` t)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  EachlikeMatcher(`[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` t)` {#classpact__consumer_1_1matchers_1_1EachlikeMatcher_1ae7bf8f7982ca2281d0dba65c762bdb46}





### `public inline  EachlikeMatcher(int e,`[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` t)` {#classpact__consumer_1_1matchers_1_1EachlikeMatcher_1a048dc280cac528a7ad7447fc7a7dce8b}





### `public inline  EachlikeMatcher(int e,int m,`[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` t)` {#classpact__consumer_1_1matchers_1_1EachlikeMatcher_1ac37dc7ce91825ae207e2040cb2e6c01e}





### `public inline  EachlikeMatcher(int e,int m,int mx,`[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` t)` {#classpact__consumer_1_1matchers_1_1EachlikeMatcher_1ace9a9d9048656816f662e5cd141da3a0}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1EachlikeMatcher_1a60a4f05c8a9f1d09273fd1da08aeb4ae}






# class `EqualsMatcher` {#classpact__consumer_1_1matchers_1_1EqualsMatcher}

```
class EqualsMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  EqualsMatcher(T v)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 
`public inline virtual std::string as_example() const` | 

## Members

### `public inline  EqualsMatcher(T v)` {#classpact__consumer_1_1matchers_1_1EqualsMatcher_1afe9e104f7dc744804e6956cc009304b9}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1EqualsMatcher_1a19cbad1fa3ef261b8aea853de8f195e5}





### `public inline virtual std::string as_example() const` {#classpact__consumer_1_1matchers_1_1EqualsMatcher_1a7034b7f61042ccce7228f6a3e7d36d32}






# class `HexadecimalMatcher` {#classpact__consumer_1_1matchers_1_1HexadecimalMatcher}

```
class HexadecimalMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  HexadecimalMatcher()` | 
`public inline  HexadecimalMatcher(std::string hex)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  HexadecimalMatcher()` {#classpact__consumer_1_1matchers_1_1HexadecimalMatcher_1a88d4e7f050653386a9bce507d1ebfb7c}





### `public inline  HexadecimalMatcher(std::string hex)` {#classpact__consumer_1_1matchers_1_1HexadecimalMatcher_1a0b59b83837f897532e92db8875208cba}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1HexadecimalMatcher_1a669207f1929f15220fa33d3251ee7047}






# class `IMatcher` {#classpact__consumer_1_1matchers_1_1IMatcher}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 
`public inline virtual std::string as_example() const` | 
`public inline virtual std::string as_regex() const` | 

## Members

### `public `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1IMatcher_1ad0e36b49acc57119b7db27a418cd9a5b}





### `public inline virtual std::string as_example() const` {#classpact__consumer_1_1matchers_1_1IMatcher_1a83c8f74ac502ccd4085d857548683b32}





### `public inline virtual std::string as_regex() const` {#classpact__consumer_1_1matchers_1_1IMatcher_1aeae8817a251ac312f7d68b6a4b62f01b}






# class `IncludesMatcher` {#classpact__consumer_1_1matchers_1_1IncludesMatcher}

```
class IncludesMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  IncludesMatcher(std::string v)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  IncludesMatcher(std::string v)` {#classpact__consumer_1_1matchers_1_1IncludesMatcher_1a55de88bf627606e5b9903db166269e15}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1IncludesMatcher_1af08ed47534033b8cfcd8c88c2ca07af6}






# class `IntegerMatcher` {#classpact__consumer_1_1matchers_1_1IntegerMatcher}

```
class IntegerMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  IntegerMatcher()` | 
`public inline  IntegerMatcher(long v)` | 
`public inline  IntegerMatcher(int v)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  IntegerMatcher()` {#classpact__consumer_1_1matchers_1_1IntegerMatcher_1a0991d669ed3b7d53466713a2e38d0281}





### `public inline  IntegerMatcher(long v)` {#classpact__consumer_1_1matchers_1_1IntegerMatcher_1a46f4b7c8cbbb1f6f19fc61c77cca9066}





### `public inline  IntegerMatcher(int v)` {#classpact__consumer_1_1matchers_1_1IntegerMatcher_1a1fe7460a246233d51bb1e2275ba15229}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1IntegerMatcher_1a42d49becb285591a6dacaa6c4737f03b}






# class `IPAddressMatcher` {#classpact__consumer_1_1matchers_1_1IPAddressMatcher}

```
class IPAddressMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  IPAddressMatcher()` | 
`public inline  IPAddressMatcher(std::string address)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  IPAddressMatcher()` {#classpact__consumer_1_1matchers_1_1IPAddressMatcher_1acfbc504a485c42a3561973f7e877afb5}





### `public inline  IPAddressMatcher(std::string address)` {#classpact__consumer_1_1matchers_1_1IPAddressMatcher_1ac44d7b87e25b7d99c7bde3a45f43011d}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1IPAddressMatcher_1a8650595d732644d12817d182f4c99976}






# class `NullMatcher` {#classpact__consumer_1_1matchers_1_1NullMatcher}

```
class NullMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1NullMatcher_1a6ac909e261e9ff32dcc5c69f9a40369e}






# class `NumberMatcher` {#classpact__consumer_1_1matchers_1_1NumberMatcher}

```
class NumberMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  NumberMatcher()` | 
`public inline  NumberMatcher(T v)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  NumberMatcher()` {#classpact__consumer_1_1matchers_1_1NumberMatcher_1a27c9dd090644dcc2df22fb437af8afcc}





### `public inline  NumberMatcher(T v)` {#classpact__consumer_1_1matchers_1_1NumberMatcher_1a8e9d378a6383ee84e356de3fa630d1c5}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1NumberMatcher_1a78c2577dce8fc71786bf237d044f3023}






# class `ObjectMatcher` {#classpact__consumer_1_1matchers_1_1ObjectMatcher}

```
class ObjectMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  ObjectMatcher(const std::unordered_map< std::string, `[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` > i_fields)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  ObjectMatcher(const std::unordered_map< std::string, `[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` > i_fields)` {#classpact__consumer_1_1matchers_1_1ObjectMatcher_1a180b4b1205e90677961cb68c2bb5e3da}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1ObjectMatcher_1aaeb0b78b715cc6fe1a531f82c7e838d6}






# class `RegexMatcher` {#classpact__consumer_1_1matchers_1_1RegexMatcher}

```
class RegexMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  RegexMatcher(std::string r)` | 
`public inline  RegexMatcher(std::string r,std::string e)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 
`public inline virtual std::string as_example() const` | 
`public inline virtual std::string as_regex() const` | 

## Members

### `public inline  RegexMatcher(std::string r)` {#classpact__consumer_1_1matchers_1_1RegexMatcher_1afbc8a3f78f38df77301fa271a06f05cb}





### `public inline  RegexMatcher(std::string r,std::string e)` {#classpact__consumer_1_1matchers_1_1RegexMatcher_1a43d35298ae2413d2cbb447b52b4b724f}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1RegexMatcher_1aba83023303c31c27f5b6a0dbe9fa03de}





### `public inline virtual std::string as_example() const` {#classpact__consumer_1_1matchers_1_1RegexMatcher_1af29f5e7788b898dc778838eeb20cb71e}





### `public inline virtual std::string as_regex() const` {#classpact__consumer_1_1matchers_1_1RegexMatcher_1a92604e824c8ef352caa1fa4a14894f48}






# class `TimeMatcher` {#classpact__consumer_1_1matchers_1_1TimeMatcher}

```
class TimeMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  TimeMatcher(std::string f)` | 
`public inline  TimeMatcher(std::string f,std::string e)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  TimeMatcher(std::string f)` {#classpact__consumer_1_1matchers_1_1TimeMatcher_1a3d4419eeb2265bd764ec231e5f738cc8}





### `public inline  TimeMatcher(std::string f,std::string e)` {#classpact__consumer_1_1matchers_1_1TimeMatcher_1ac046e8d7763d30ad98d14d5490a39c89}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1TimeMatcher_1a257ac9db3b5fb71a4a1fe0b6156606ce}






# class `TypeMatcher` {#classpact__consumer_1_1matchers_1_1TypeMatcher}

```
class TypeMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  TypeMatcher(T v)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  TypeMatcher(T v)` {#classpact__consumer_1_1matchers_1_1TypeMatcher_1a11577167bb784d1c1662a45ec8f78197}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1TypeMatcher_1ae96f1375296be7ea2a78e9fec7a6cca5}






# class `UrlMatcher` {#classpact__consumer_1_1matchers_1_1UrlMatcher}

```
class UrlMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  UrlMatcher(std::string b,std::vector< `[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` > f)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  UrlMatcher(std::string b,std::vector< `[`IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` > f)` {#classpact__consumer_1_1matchers_1_1UrlMatcher_1a30672a90c069786fe7e52485a20a7fed}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1UrlMatcher_1abb43488b2c0d7bc2fa87c102c34db57e}






# class `UuidMatcher` {#classpact__consumer_1_1matchers_1_1UuidMatcher}

```
class UuidMatcher
  : public pact_consumer::matchers::IMatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  UuidMatcher()` | 
`public inline  UuidMatcher(std::string uuid)` | 
`public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` | 

## Members

### `public inline  UuidMatcher()` {#classpact__consumer_1_1matchers_1_1UuidMatcher_1a1fd7cca306b66645c8d9c3714c76d14d}





### `public inline  UuidMatcher(std::string uuid)` {#classpact__consumer_1_1matchers_1_1UuidMatcher_1a341279e31305c6db7bd6952c6ea5f8af}





### `public virtual `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` getJson() const` {#classpact__consumer_1_1matchers_1_1UuidMatcher_1a3e71e2f36be6c3db39ef819bafb353ed}






# class `Interaction` {#classpact__consumer_1_1Interaction}




Class that defines an interaction between a consumer and provider

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public pact_mock_server_ffi::InteractionHandle interaction` | 
`public  Interaction(const `[`Pact`](#classpact__consumer_1_1Pact)` * parent,const char * description)` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` given(const char * provider_state) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` given(const char * provider_state,std::unordered_map< std::string, std::string > parameters) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` uponReceiving(const char * description) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` withRequest(const char * method,const char * path) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` withQuery(std::unordered_map< std::string, std::vector< std::string >> query) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` withHeaders(std::unordered_map< std::string, std::vector< std::string >> headers) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` withJsonBody(void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` withJsonBody(`[`pact_consumer::matchers::IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` body) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` withBinaryFile(std::string content_type,std::filesystem::path example_file) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` willRespondWith(size_t status) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` withResponseHeaders(std::unordered_map< std::string, std::vector< std::string >> headers) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` withResponseJsonBody(void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` withResponseJsonBody(`[`pact_consumer::matchers::IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` body) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` withResponseBinaryFile(std::string content_type,std::filesystem::path example_file) const` | 

## Members

### `public pact_mock_server_ffi::InteractionHandle interaction` {#classpact__consumer_1_1Interaction_1a89bc107360700c1ba7741061910d4485}





### `public  Interaction(const `[`Pact`](#classpact__consumer_1_1Pact)` * parent,const char * description)` {#classpact__consumer_1_1Interaction_1adfdeefe452d43765c863764c8010dacb}





### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` given(const char * provider_state) const` {#classpact__consumer_1_1Interaction_1ab317881a13f364c21ba4d773103f6fa9}



Adds the provider state to the interaction

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` given(const char * provider_state,std::unordered_map< std::string, std::string > parameters) const` {#classpact__consumer_1_1Interaction_1a5acf88deed519e9053db782bb836f1ed}



Adds a provider state with parameters

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` uponReceiving(const char * description) const` {#classpact__consumer_1_1Interaction_1a4d6a8acadad3b486a3996a5fd4819be2}



Sets the description for the interaction

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` withRequest(const char * method,const char * path) const` {#classpact__consumer_1_1Interaction_1ac29f5f3c8c0dfe941e00bee09334b341}



Sets the method and path for the request

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` withQuery(std::unordered_map< std::string, std::vector< std::string >> query) const` {#classpact__consumer_1_1Interaction_1ac73dc56507068dcf186dc76ebb848b30}



Sets the query parameters for the request

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` withHeaders(std::unordered_map< std::string, std::vector< std::string >> headers) const` {#classpact__consumer_1_1Interaction_1a8d4c0519d5e17bc14e4b062fd4361c71}



Sets the headers for the request

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` withJsonBody(void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback) const` {#classpact__consumer_1_1Interaction_1ac7cbcf570140343c007522be4091afb7}



Sets the body for the request using the callback. The callback will be invoked with a builder to construct the body.

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` withJsonBody(`[`pact_consumer::matchers::IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` body) const` {#classpact__consumer_1_1Interaction_1abeb14a7a082e64f78644d9803825e0cb}



Sets the body for the request using the provided body template

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` withBinaryFile(std::string content_type,std::filesystem::path example_file) const` {#classpact__consumer_1_1Interaction_1a32d4f4973538d33d5abe1889d0c8365f}



Sets the body for the request using the example file and content type. Note that this will attempt to load the entire example file in memory. Use small files for your testing.

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` willRespondWith(size_t status) const` {#classpact__consumer_1_1Interaction_1adb4b399b152b384682d17436ac648d45}



Sets the status code for the response

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` withResponseHeaders(std::unordered_map< std::string, std::vector< std::string >> headers) const` {#classpact__consumer_1_1Interaction_1a9d0915c5ab54bff6e33a9ca8e9464baa}



Sets the headers for the response

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` withResponseJsonBody(void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback) const` {#classpact__consumer_1_1Interaction_1afa30f026c1c2cb12d366ea23f27ff255}



Sets the body for the request using the callback. The callback will be invoked with a builder to construct the body.

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` withResponseJsonBody(`[`pact_consumer::matchers::IMatcher::Ptr`](#classpact__consumer_1_1matchers_1_1IMatcher_1a36aa47dbb9f50f5d8432977bc7ae9a5b)` body) const` {#classpact__consumer_1_1Interaction_1a82bea9bd7fd64a0ae56924c086891863}



Sets the body for the request using the provided body template

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` withResponseBinaryFile(std::string content_type,std::filesystem::path example_file) const` {#classpact__consumer_1_1Interaction_1a3489e6576ddb7e22fd6dbc45d35fefd8}



Sets the body for the response using the example file and content type. Note that this will attempt to load the entire example file in memory. Use small files for your testing.


# class `MockServerHandle` {#classpact__consumer_1_1MockServerHandle}




Mock server handle to the mock server started for the test

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  MockServerHandle(pact_mock_server_ffi::PactHandle)` | 
`public  ~MockServerHandle()` | 
`public bool started_ok() const` | 
`public std::string get_url() const` | 
`public int32_t get_port() const` | 

## Members

### `public  MockServerHandle(pact_mock_server_ffi::PactHandle)` {#classpact__consumer_1_1MockServerHandle_1a8ec5a62b26a7551101312afd3ea542a4}





### `public  ~MockServerHandle()` {#classpact__consumer_1_1MockServerHandle_1a515457a59d2e3a74e22ec558a8e5a490}





### `public bool started_ok() const` {#classpact__consumer_1_1MockServerHandle_1a5956a6aa4afb3cb782dae4ed58f7e5ad}



Indicates the mock server was started ok

### `public std::string get_url() const` {#classpact__consumer_1_1MockServerHandle_1a7b9177aab903fca361cac6be60f2a7cc}



The URL to the mock server

### `public int32_t get_port() const` {#classpact__consumer_1_1MockServerHandle_1ad3109a5b065ceac4e83e901c05625562}



The port the mock server is running on


# class `Pact` {#classpact__consumer_1_1Pact}




Class that defines a [Pact](#classpact__consumer_1_1Pact) between a consumer and provider

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public pact_mock_server_ffi::PactHandle pact` | 
`public std::string pact_directory` | 
`public  Pact(const char * consumer,const char * provider)` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` given(const char * provider_state) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` given(const char * provider_state,std::unordered_map< std::string, std::string > parameters) const` | 
`public `[`Interaction`](#classpact__consumer_1_1Interaction)` uponReceiving(const char * description) const` | 
`public `[`PactTestResult`](#classpact__consumer_1_1PactTestResult)` run_test(bool(*)(`[`MockServerHandle`](#classpact__consumer_1_1MockServerHandle) *)` callback) const` | 

## Members

### `public pact_mock_server_ffi::PactHandle pact` {#classpact__consumer_1_1Pact_1a43a945f3d3b363c9c251f5954e76b915}





### `public std::string pact_directory` {#classpact__consumer_1_1Pact_1a2d71145b2d9394a13b2ec104b031cdd2}



Directory to write pact files to

### `public  Pact(const char * consumer,const char * provider)` {#classpact__consumer_1_1Pact_1a907de956f18c074634ceba0980fa8630}



Constructs the [Pact](#classpact__consumer_1_1Pact) given the consumer and provider names

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` given(const char * provider_state) const` {#classpact__consumer_1_1Pact_1a630248129b4a610fd15d94d08c6fb6f5}



Creates a new iteraction with a defined provider state

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` given(const char * provider_state,std::unordered_map< std::string, std::string > parameters) const` {#classpact__consumer_1_1Pact_1a00772d7384c5484206c87990cd09cfdf}



Creates a new iteraction with a defined provider state and parameters

### `public `[`Interaction`](#classpact__consumer_1_1Interaction)` uponReceiving(const char * description) const` {#classpact__consumer_1_1Pact_1a05713ce4758ee154989b327c686dc18d}



Creates a new interaction with the provided description.

### `public `[`PactTestResult`](#classpact__consumer_1_1PactTestResult)` run_test(bool(*)(`[`MockServerHandle`](#classpact__consumer_1_1MockServerHandle) *)` callback) const` {#classpact__consumer_1_1Pact_1a3428f3cbc0ad30377867377efe84fef2}



Starts a mock server for this pact, and then passes it to the callback. The callback needs to return a boolean value to indicate of the test was successful.


# class `PactJsonBuilder` {#classpact__consumer_1_1PactJsonBuilder}




Builder class to help construct JSON bodies

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  PactJsonBuilder(const `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` * parent)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & eachLike(std::string name,unsigned int examples,void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & eachLike(std::string name,void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & atLeastOneLike(std::string name,unsigned int examples,void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & atLeastOneLike(std::string name,void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & integer(std::string name,int example)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & integer(std::string name)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & decimal(std::string name,double example)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & decimal(std::string name)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & number(std::string name,double example)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & number(std::string name,int example)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & number(std::string name)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & string(std::string name,std::string example)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & string(std::string name)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & regex(std::string name,std::string regex,std::string example)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & regex(std::string name,std::string regex)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & datetime(std::string name,std::string format,std::string example)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & datetime(std::string name,std::string format)` | 
`public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & boolean(std::string name,bool b)` | 
`public inline `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` get_json()` | 

## Members

### `public  PactJsonBuilder(const `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` * parent)` {#classpact__consumer_1_1PactJsonBuilder_1aa805a10ce64aa7a9fc7f0d3b3dcbe697}





### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & eachLike(std::string name,unsigned int examples,void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback)` {#classpact__consumer_1_1PactJsonBuilder_1a2636f4008e8c8a66adaf61ff9e858571}



Attribute that is an array where each item in the array must match the constructed template 
#### Parameters
* `name` Attribute name 


* `examples` Number of examples to generate (defaults to 1) 


* `callback` Callback that gets invoked to define the template

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & eachLike(std::string name,void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback)` {#classpact__consumer_1_1PactJsonBuilder_1a33e402dd87b5a63c00f6923b067091b7}



Attribute that is an array where each item in the array must match the constructed template. Will only generate one example. 
#### Parameters
* `name` Attribute name 


* `callback` Callback that gets invoked to define the template

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & atLeastOneLike(std::string name,unsigned int examples,void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback)` {#classpact__consumer_1_1PactJsonBuilder_1a29dac043b1bcb5e4a6a61021ea26cebc}



Attribute that is an array that has to have at least one element and each element must match the given template 
#### Parameters
* `name` Attribute name 


* `examples` Number of examples to generate (defaults to 1) 


* `callback` Callback that gets invoked to define the template

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & atLeastOneLike(std::string name,void(*)(`[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder) *)` callback)` {#classpact__consumer_1_1PactJsonBuilder_1ae69caf6e8088277037ec4757a0fa570d}



Attribute that is an array that has to have at least one element and each element must match the given template. Will only generate one example. 
#### Parameters
* `name` Attribute name 


* `callback` Callback that gets invoked to define the template

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & integer(std::string name,int example)` {#classpact__consumer_1_1PactJsonBuilder_1a1c2313cac83d46a0eae974638e5c41c7}



Attribute whose value must be an integer (must be a number and have no decimal places) 
#### Parameters
* `name` Attribute name 


* `example` Example value. If omitted a random value will be generated.

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & integer(std::string name)` {#classpact__consumer_1_1PactJsonBuilder_1ade5ddccbcbcf1751c626f322a233bfad}



Attribute whose value must be an integer (must be a number and have no decimal places). A random value will be generated for any examples. 
#### Parameters
* `name` Attribute name

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & decimal(std::string name,double example)` {#classpact__consumer_1_1PactJsonBuilder_1a09d0204658f06b97fbfcb35542cfef1d}



Attribute whose value must be a decimal number (must be a number and have decimal places) 
#### Parameters
* `name` Attribute name 


* `num` Example value. If omitted a random value will be generated.

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & decimal(std::string name)` {#classpact__consumer_1_1PactJsonBuilder_1a9415c87b39a702a7f1d842c8facf4024}



Attribute whose value must be a decimal number (must be a number and have decimal places). A random value will be generated for any examples. 
#### Parameters
* `name` Attribute name

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & number(std::string name,double example)` {#classpact__consumer_1_1PactJsonBuilder_1a8a129fb5eccbf58e79d04a6e2f4eb425}



Attribute whose value must be a number. 
#### Parameters
* `name` Attribute name 


* `num` Example value. If omitted a random integer value will be generated.

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & number(std::string name,int example)` {#classpact__consumer_1_1PactJsonBuilder_1a81ede32b19f5526f9eb63b4cf72e8c4a}



Attribute whose value must be a number. 
#### Parameters
* `name` Attribute name 


* `num` Example value. If omitted a random integer value will be generated.

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & number(std::string name)` {#classpact__consumer_1_1PactJsonBuilder_1ac7e41cb54bc646094a033f0f70ff47b4}



Attribute whose value must be a number. A random integer value will be generated for any examples. 
#### Parameters
* `name` Attribute name

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & string(std::string name,std::string example)` {#classpact__consumer_1_1PactJsonBuilder_1a64cd45052d249fac2df28f4e25ef9109}



Attribute whose value must be a string. 
#### Parameters
* `name` Attribute name 


* `example` Example value.

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & string(std::string name)` {#classpact__consumer_1_1PactJsonBuilder_1a72e13ca23c7d35334e446bb30198edf6}



Attribute whose value must be a string. A random string value will be generated for any examples. 
#### Parameters
* `name` Attribute name

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & regex(std::string name,std::string regex,std::string example)` {#classpact__consumer_1_1PactJsonBuilder_1a0465bbc9df6c990ed353d21ba83a58df}



Attribute whose value that must match the given regular expression. 
#### Parameters
* `name` Attribute name 


* `regex` Regular Expression to match 


* `example` Example value

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & regex(std::string name,std::string regex)` {#classpact__consumer_1_1PactJsonBuilder_1a98faa827ae350b7bcd5f038a61ee0dff}



Attribute whose value that must match the given regular expression. A random string value will be generated for any examples. 
#### Parameters
* `name` Attribute name 


* `regex` Regular Expression to match

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & datetime(std::string name,std::string format,std::string example)` {#classpact__consumer_1_1PactJsonBuilder_1a687934cbab5262ff82360fd92c44fd8f}



String value that must match the provided datetime format string. 
#### Parameters
* `name` Attribute name 


* `format` Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)


* `example` Example value to use. If omitted a value using the current system date and time will be generated.

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & datetime(std::string name,std::string format)` {#classpact__consumer_1_1PactJsonBuilder_1ac8fc12d46da35e07eb2b05192a1a8a98}



String value that must match the provided datetime format string. A random value will be 
#### Parameters
* `name` Attribute name generated for any examples. 


* `format` Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)

### `public `[`PactJsonBuilder`](#classpact__consumer_1_1PactJsonBuilder)` & boolean(std::string name,bool b)` {#classpact__consumer_1_1PactJsonBuilder_1afda7b8c27b11afb4c732c1544bc81030}



Value must be a boolean 
#### Parameters
* `name` Attribute name 


* `b` Boolean example value

### `public inline `[`json`](#consumer_8h_1ab701e3ac61a85b337ec5c1abaad6742d)` get_json()` {#classpact__consumer_1_1PactJsonBuilder_1a7bbc3c584b6236d0cfe87f8cd5ea7ad7}






# class `PactTestResult` {#classpact__consumer_1_1PactTestResult}




Result of the [Pact](#classpact__consumer_1_1Pact) test run

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  PactTestResult()` | 
`public void add_state(`[`TestResultState`](#namespacepact__consumer_1a52ee54250f0c10cc1cb80909bef7b68c)` state)` | 
`public void add_state(`[`TestResultState`](#namespacepact__consumer_1a52ee54250f0c10cc1cb80909bef7b68c)` state,std::string message)` | 
`public void add_state(`[`TestResultState`](#namespacepact__consumer_1a52ee54250f0c10cc1cb80909bef7b68c)` state,std::string message,std::string ex)` | 
`public bool is_ok() const` | 
`public void display_errors()` | 

## Members

### `public  PactTestResult()` {#classpact__consumer_1_1PactTestResult_1ad864d8a498509ff5b4bfa25eaf490a6c}





### `public void add_state(`[`TestResultState`](#namespacepact__consumer_1a52ee54250f0c10cc1cb80909bef7b68c)` state)` {#classpact__consumer_1_1PactTestResult_1a17c49c0a25f96a69b1e37dbdcc0b88ad}



Adds a test state to the result

### `public void add_state(`[`TestResultState`](#namespacepact__consumer_1a52ee54250f0c10cc1cb80909bef7b68c)` state,std::string message)` {#classpact__consumer_1_1PactTestResult_1a0e60ee0a59c060314b32bdaea6f3e67d}



Adds a test state with a message to the result

### `public void add_state(`[`TestResultState`](#namespacepact__consumer_1a52ee54250f0c10cc1cb80909bef7b68c)` state,std::string message,std::string ex)` {#classpact__consumer_1_1PactTestResult_1a7df2d4fade24fa975ae88e862de5b673}



Adds a test state with a message to the result

### `public bool is_ok() const` {#classpact__consumer_1_1PactTestResult_1af52c0d3521914b0398d0bc0d54cbd1a9}



If there are no mismatches and the user code did not fail

### `public void display_errors()` {#classpact__consumer_1_1PactTestResult_1ad559f0f824c26a6fd5f6c6b1fa89932a}



Logs all errors out


# namespace `pact_mock_server_ffi`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

