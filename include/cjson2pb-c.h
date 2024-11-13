/**
 * @file cjson2pb-c.h
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @date 2024-07-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _CJSON_2_PROTOBUF_H_
#define _CJSON_2_PROTOBUF_H_

#include <stdbool.h>

#include "cjson/cJSON.h"
#include "protobuf-c/protobuf-c.h"

typedef enum byte_mode {
    BYTE_MODE_FILE_PATH,
    BYTE_MODE_HEX,
    BYTE_MODE_BASE64
} ByteMode;

#define JSON_EXCEPTION_LIST(METHOD)                                                                                                                                                                             \
    METHOD(E_NULL_VALUE, 0x00000001, "json object is null"), METHOD(E_EMPTY_ARRAY, 0x00000002, "convert empty array to a repeated protobuf field"),                                                             \
        METHOD(E_UNACCEPTABLE_JSON_TYPE, 0x00000004, "convert unacceptable json type to a protobuf field"), METHOD(E_INVALID_FILEPATH, 0x00000008, "filepath for bytes field does not exist or is not a file"), \
        METHOD(E_INVALID_BASE64, 0x00000010, "error in base64 decoding"), METHOD(E_INVALID_HEX_STRING, 0x00000020, "error in hex decoding, possibly unsupported format"),                                       \
        METHOD(E_INVALID_NUMBER_STRING, 0x00000040, "cannot convert string to number with specified type"), METHOD(E_TYPE_OVERFLOW, 0x00000080, "number in string format is out of range for specified type"),  \
        METHOD(E_INVALID_ENUM, 0x00000100, "enum value (number provided by json) is out of range")

#define PROTOBUF_EXCEPTION_LIST(METHOD) METHOD(E_INVALID_FIELD_NAME, 0x00010000, "field name is not found in protobuf message"), METHOD(E_UNINITIALIZED, 0x00020000, "protobuf message has not been initialized")

#define USER_EXCEPTION_LIST(METHOD) METHOD(E_INVALID_ARG, 0x00100000, "invalid argument, possibly NULL value"), METHOD(E_INVALID_STRING_ENUM_CALLBACK, 0x00200000, "enum value (number provided by callback function) is out of range")

#define SYSTEM_EXCEPTION_LIST(METHOD) METHOD(E_FILE_OPEN, 0x01000000, "file open error"), METHOD(E_MEM_ALLOC, 0x02000000, "memory allocation error")

#define ENUM_K_V(KEY, VAL, DESC)  KEY = VAL
#define ONLY_ENUM(KEY, VAL, DESC) KEY
#define ONLY_DESC(KEY, VAL, DESC) DESC

#define JSON_EXCEPTION(METHOD) JSON_EXCEPTION_LIST(METHOD), E_JSON_EXCEPTION_MAX /* max value of json exception */

#define PROTOBUF_EXCEPTION(METHOD) PROTOBUF_EXCEPTION_LIST(METHOD), E_PROTOBUF_EXCEPTION_MAX /* max value of protobuf exception */

#define USER_EXCEPTION(METHOD) USER_EXCEPTION_LIST(METHOD), E_USER_EXCEPTION_MAX /* max value of user exception */

#define SYSTEM_EXCEPTION(METHOD) SYSTEM_EXCEPTION_LIST(METHOD), E_UNKNOWN /* unknown error, max value of system exception */

typedef enum rtn_code {
    E_SUCCESS = 0x00000000,

    /* JSON Exception */

    JSON_EXCEPTION(ENUM_K_V),

    /* Protobuf Exception */

    PROTOBUF_EXCEPTION(ENUM_K_V),

    /* Other Exception */

    USER_EXCEPTION(ENUM_K_V),

    /* System */

    SYSTEM_EXCEPTION(ENUM_K_V),

} RtnCode;

#define E_JSON_EXCEPTION_MASK     0x0000ffff /* json exception mask */
#define E_PROTOBUF_EXCEPTION_MASK 0x000f0000 /* protobuf exception mask */
#define E_USER_EXCEPTION_MASK     0x00f00000 /* user exception mask */
#define E_SYSTEM_EXCEPTION_MASK   0x0f000000 /* system exception mask */

static RtnCode rtn_code_enum_list[] = {JSON_EXCEPTION_LIST(ONLY_ENUM), PROTOBUF_EXCEPTION_LIST(ONLY_ENUM), USER_EXCEPTION_LIST(ONLY_ENUM), SYSTEM_EXCEPTION_LIST(ONLY_ENUM)};
static char*   rtn_code_desc_list[] = {JSON_EXCEPTION_LIST(ONLY_DESC), PROTOBUF_EXCEPTION_LIST(ONLY_DESC), USER_EXCEPTION_LIST(ONLY_DESC), SYSTEM_EXCEPTION_LIST(ONLY_DESC)};

#undef JSON_EXCEPTION_LIST
#undef PROTOBUF_EXCEPTION_LIST
#undef USER_EXCEPTION_LIST
#undef SYSTEM_EXCEPTION_LIST
#undef ENUM_K_V
#undef ONLY_ENUM
#undef JSON_EXCEPTION
#undef PROTOBUF_EXCEPTION
#undef USER_EXCEPTION
#undef SYSTEM_EXCEPTION

/**
 * @brief Callback function to convert string to enum
 *
 * @param[in] enum_desc ProtobufCEnumDescriptor of the enum
 * @param[in] str string to be converted
 * @return valid enum value (for now, we don't check. You can call `int_range_lookup` to check it)
 */
typedef int (*StringEnumCallback)(const ProtobufCEnumDescriptor* const enum_desc, const char* const str);

/**
 * @brief Callback function to convert string to bool
 *
 * @param[in] str string to be converted
 * @return true or false
 */
typedef bool (*StringBoolCallback)(const char* const str);

/**
 * @brief Callback function to convert cJSON object to Protobuf Message
 *
 * In this function, you can assume the `msg` is already initialized with the correct type. If the conversion fails,
 * you should return non-zero value.
 *
 * @note Don't free `msg` in this function, it will be freed by the caller if the conversion is failed.
 *
 */
typedef RtnCode (*MsgConvertorCallback)(ProtobufCMessage* const msg, const cJSON* const root);

#ifdef __GNUC__
#define ALL_NONNULL  __attribute__((__nonnull__))
#define NONNULL(...) __attribute__((__nonnull__(__VA_ARGS__)))
#else
#define ALL_NONNULL
#define NONNULL(...)
#endif

/**
 * @brief Convert cjson item `root` to Protobuf Message `msg` with field `field_name`.
 *
 * If field is messge type, call `msg_convertor` to convert it. If field is string enum, call `string_enum` to convert it.
 *
 * @param[in] root cjson item
 * @param[in] msg Protobuf Message which should be initialized
 * @param[in] field_name field name in Protobuf Message
 * @param[in] string_enum callback function to convert string to enum
 * @param[in] string_bool callback function to convert string to bool
 * @param[in] msg_convertor callback function to convert message to message type field
 * @return
 */
uint32_t cvt_cjson_2_proto_c_field(const cJSON* const restrict root,
                                   ProtobufCMessage* const restrict msg,
                                   const char* const restrict field_name,
                                   StringEnumCallback const   string_enum,
                                   StringBoolCallback const   string_bool,
                                   MsgConvertorCallback const msg_convertor,
                                   const ByteMode             mode) NONNULL(1, 2, 3);

/**
 * @brief
 *
 * @param map
 * @param desc_list
 * @return const size_t
 */
const int parse_rtn_bit_map(const uint32_t map, char** const restrict desc_list);

/**
 * @brief
 *
 * @return const char*
 */
const char* json_2_proto_c_version(void);

/**
 * @brief
 *
 * @return const char*
 */
const char* json_2_protoc_dependencies(void);

int default_string_enum_conversion(const ProtobufCEnumDescriptor* const enum_desc, const char* const str);
int default_msg_convertor(ProtobufCMessage* const msg, const cJSON* const item);

RtnCode cvt_single_int32_t(int32_t* const field, const cJSON* const item) ALL_NONNULL;
RtnCode cvt_single_int64_t(int64_t* const field, const cJSON* const item) ALL_NONNULL;
RtnCode cvt_single_uint32_t(uint32_t* field, const cJSON* item) ALL_NONNULL;
RtnCode cvt_single_uint64_t(uint64_t* field, const cJSON* item) ALL_NONNULL;
RtnCode cvt_single_float(float* field, const cJSON* item) ALL_NONNULL;
RtnCode cvt_single_double(double* field, const cJSON* item) ALL_NONNULL;
RtnCode cvt_single_bool(bool* const field, const cJSON* const item, StringBoolCallback string_bool) NONNULL(1, 2);
RtnCode cvt_single_string(char** field, const cJSON* item) ALL_NONNULL;
RtnCode cvt_single_bytes(ProtobufCBinaryData* const field, const cJSON* const item, const ByteMode mode) ALL_NONNULL;
RtnCode cvt_single_enum(int* const field, const ProtobufCEnumDescriptor* const enum_desc, const cJSON* const item, StringEnumCallback string_enum) NONNULL(1, 2, 3);
RtnCode cvt_single_message(ProtobufCMessage* const msg, const cJSON* const item, MsgConvertorCallback msg_convertor) NONNULL(1, 2);

uint32_t cvt_cjson_2_pb_number(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name);
uint32_t cvt_cjson_2_pb_string(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name);
uint32_t cvt_cjson_2_pb_message(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name, MsgConvertorCallback const msg_convertor);
uint32_t cvt_cjson_2_pb_enum(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name, StringEnumCallback const string_enum);
uint32_t cvt_cjson_2_pb_byte(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name, const ByteMode mode);
uint32_t cvt_cjson_2_pb_bool(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name, StringBoolCallback const string_bool);

int                       int_range_lookup(unsigned n_ranges, const ProtobufCIntRange* ranges, int value);
const ProtobufCEnumValue* protobuf_c_enum_descriptor_get_value_by_name_case_insensitive(const ProtobufCEnumDescriptor* desc, const char* name);

#undef ALL_NONNULL
#undef NONNULL

#endif /* _CJSON_2_PROTOBUF_H_ */