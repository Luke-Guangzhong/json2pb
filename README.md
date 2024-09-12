json2pb-c

This is a tool to convert JSON to Protocol Buffer C code.

1. In this library, converting from JSON to Protobuf will follow the Protobuf schema. Fields in JSON that do not have a corresponding field in Protobuf will be ignored.

2. Conversion Type


| Protobuf Type               | Acceptable JSON Types |
| --------------------------- | --------------------- |
| `double`                    | `number`,`string`     |
| `float`                     | `number`,`string`     |
| `int32`,`sint32`,`sfixed32` | `number`,`string`     |
| `int64`,`sint64`,`sfixed64` | `number`,`string`     |
| `uint32`,`fixed32`          | `number`,`string`     |
| `uint64`,`fixed64`          | `number`,`string`     |
| `string`                    | `string`              |
| `bool`                      | `number`,`string`     |
| `bytes`                     | `string`              |
| `message`                   | `object`              |

note: we use strtoxxx functions to convert string to integer and float types.

If protobuf has `repeated`, accpet non-empty `array` in JSON. If JSON field is `null`, don't set it in protobuf.

If there is null value in array while corresponding protobuf field is `repeated`, the tool will keep the null value.

for exmaple:

```json
{
    "strength":[null,null],
    "name_list":["Alice","Bob","Charlie",null,"Eve"],
    "age":null
}
```

```shell
# parsed protobuf message
strength: 0
strength: 0
name_list: "Alice"
name_list: "Bob"
name_list: "Charlie"
name_list: "(null)"
name_list: "Eve"
# no age
```

> PS: About `bytes` type, there are three mode to convert JSON to Protobuf: base64, hex, and filepath. 
> 
> - base64: the JSON string is a base64 encoded string.
> - hex: the JSON string is a hex encoded string with space delimiter. Prefix `0x` is not accepted. Example: "00 ff "
> - filepath: the JSON string is a filepath. The tool will read the file content and convert it to bytes.
> 
> User should choose one of these modes according to their needs. Default is filepath.

under hex and base64 mode, if error occrued, we drop all string

Converting a string to enum, user should provide a function corresponding the function signature `int (*StringEnumCallback)(const char* const str)`

Converting a object to message, user should provide a function corresponding the function signature `int (*MsgConvertorCallback)(ProtobufCMessage* const msg, const cJSON* const root)`

for boolean value:

1. accept `true` and `false` in JSON.
2. accept number non-zero as true and `0` as false in JSON.
3. accept string enum and string enum callback function

for string:

1. must null-terminated

for uint64:

cjson use double to store number, so if the protobuf field is uint64, suggest to use string to store the number in json.

Default action

1. JSON key is match with Protobuf field name case sensitive.
2. If JSON value is string for Protobuf enum, only match the name in protobuf
3. string enum callback can be also provided for boolean value in protobuf
4. if conversion error occured, ignore and keep running

3. Dependencies

- `cJSON` library for JSON parsing.
- `protoc-c` compiler for generating C code from Protobuf schema.
- `trower-base64` library for base64 encoding and decoding.