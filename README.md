这是一个将 JSON 转换为 Protocol Buffer 的工具。

# 转换规则

1. 在该库中，从 JSON 到 Protobuf 的转换将遵循 Protobuf 的模式。JSON 中没有对应 Protobuf 字段的字段将被忽略。

2. **转换类型**

| Protobuf 类型                   | 可接受的 JSON 类型       |
| ----------------------------- | ------------------ |
| `double`                      | `number`, `string` |
| `float`                       | `number`, `string` |
| `int32`, `sint32`, `sfixed32` | `number`, `string` |
| `int64`, `sint64`, `sfixed64` | `number`, `string` |
| `uint32`, `fixed32`           | `number`, `string` |
| `uint64`, `fixed64`           | `number`, `string` |
| `string`                      | `string`           |
| `bool`                        | `number`, `string` |
| `bytes`                       | `string`           |
| `message`                     | `object`           |
| `enum`                        | `number`, `string` |

**注意：** 我们使用 `strtoxxx` 函数将字符串转换为整数和浮点数类型。

- 如果 Protobuf 中有 `repeated`，则接受 JSON 中的非空 `array`。如果数组为空或 JSON 字段为 `null`，则不会在 Protobuf 中设置该字段。
- 如果数组中有 `null` 值，而对应的 Protobuf 字段是 `repeated`，工具将保留 `null` 值。

**示例：**

```json
{
    "strength": [null, null],
    "name_list": ["Alice", "Bob", "Charlie", null, "Eve"],
    "age": null
}
```

```shell
# 解析后的 protobuf 消息
strength: 0
strength: 0
name_list: "Alice"
name_list: "Bob"
name_list: "Charlie"
name_list: "(null)"
name_list: "Eve"
# 没有 age 字段
```

> PS： 关于 bytes 类型，有三种模式可以将 JSON 转换为 Protobuf：base64、hex 和 filepath。
> 
> + base64： JSON 字符串是 base64 编码的字符串。
> + hex： JSON 字符串是十六进制编码的字符串，使用空格分隔。不接受 0x 前缀。例如："00 ff "
> + filepath： JSON 字符串是文件路径。工具将读取文件内容并将其转换为字节。
> 
> 用户应根据需要选择其中一种模式。默认模式为 filepath。

在 `hex` 和 `base64` 模式下，如果发生错误，所有字节将被丢弃。

将字符串转换为枚举时，需要提供一个函数，其签名为：

```c
int (*StringEnumCallback)(const ProtobufCEnumDescriptor* const enum_desc, const char* const str);
```

将对象转换为消息(*message*)时，需要提供一个函数，其签名为：

```c
RtnCode (*MsgConvertorCallback)(ProtobufCMessage* const msg, const cJSON* const root);
```

# 布尔值

1. 接受 JSON 中的 true 和 false。
2. 接受非零数字作为 true，0 作为 false。
3. 接受字符串枚举和字符串布尔回调函数，其签名为：

```c
bool (*StringBoolCallback)(const char* const str);
```

# 字符串

必须以 null 结尾。

# uin64

由于 cJSON 使用 double 存储数字，如果 Protobuf 字段是 uint64，建议在 JSON 中使用字符串存储数字。该库会将其转换为 uint64 数据。

# 默认行为

1. JSON 键与 Protobuf 字段名称区分大小写匹配。
2. 如果 JSON 值是 Protobuf 枚举的字符串，则仅在不区分大小写的情况下匹配 Protobuf 中的名称。
3. 字符串枚举回调也可以用于 Protobuf 中的布尔值。
4. 如果转换发生错误，忽略并继续运行。

# 依赖项

1. cJSON 库用于解析 JSON。
2. protoc-c 编译器用于从 Protobuf 模式生成 C 代码。
3. trower-base64 库用于 base64 编码和解码。