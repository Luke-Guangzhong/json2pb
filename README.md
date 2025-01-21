This is a tool to convert JSON to Protocol Buffer C code.

# Conversion Rules

1. In this library, converting from JSON to Protobuf will follow the Protobuf schema. Fields in JSON that do not have a corresponding field in Protobuf will be ignored.

2. **Conversion Type**

| Protobuf Type                 | Acceptable JSON Types |
| ----------------------------- | --------------------- |
| `double`                      | `number`, `string`    |
| `float`                       | `number`, `string`    |
| `int32`, `sint32`, `sfixed32` | `number`, `string`    |
| `int64`, `sint64`, `sfixed64` | `number`, `string`    |
| `uint32`, `fixed32`           | `number`, `string`    |
| `uint64`, `fixed64`           | `number`, `string`    |
| `string`                      | `string`              |
| `bool`                        | `number`, `string`    |
| `bytes`                       | `string`              |
| `message`                     | `object`              |
| `enum`                        | `number`, `string`    |

**Note:** We use `strtoxxx` functions to convert strings to integer and float types.

- If Protobuf has `repeated`, it accepts non-empty `array` in JSON. If the array is empty or the JSON field is `null`, it will not be set in Protobuf.
- If there is a `null` value in an array while the corresponding Protobuf field is `repeated`, the tool will keep the `null` value.

**Example:**

```json
{
    "strength": [null, null],
    "name_list": ["Alice", "Bob", "Charlie", null, "Eve"],
    "age": null
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

> PS: About bytes type, there are three modes to convert JSON to Protobuf: base64, hex, and filepath.
> 
> - base64: The JSON string is a base64 encoded string.
> - hex: The JSON string is a hex encoded string with space delimiter. Prefix `0x` is not accepted. Example: "00 ff "
> - filepath: The JSON string is a filepath. The tool will read the file content and convert it to bytes.
> 
> The user should choose one of these modes according to their needs. The default mode is filepath.

Under `hex` and `base64` modes, if an error occurs, all bytes are dropped.

Converting a string to an enum requires a function with the signature:

```c
int (*StringEnumCallback)(const ProtobufCEnumDescriptor* const enum_desc, const char* const str);
```

Converting an object to a message requires a function with the signature:

```c
RtnCode (*MsgConvertorCallback)(ProtobufCMessage* const msg, const cJSON* const root);
```

# Boolean Values

Accept true and false in JSON.

Accept non-zero numbers as true and 0 as false in JSON.

Accept string enum and string bool callback function with the signature:

```c
bool (*StringBoolCallback)(const char* const str);
```

# Strings

Must be null-terminated.

# uint64

Since `cJSON` uses `double` to store numbers, if the Protobuf field is `uint64`, it is recommended to use a string to store the number in JSON.

# Default Actions

1. JSON key matches Protobuf field name case-sensitive.
2. If JSON value is a string for Protobuf enum, it only matches the name in Protobuf case-insensitive.
3. String enum callback can also be provided for boolean values in Protobuf.
4. If a conversion error occurs, ignore it and keep running.

# Dependencies

1. `cJSON` library for JSON parsing.
2. `protoc-c` compiler for generating C code from Protobuf schema.
3. `trower-base64` library for base64 encoding and decoding.