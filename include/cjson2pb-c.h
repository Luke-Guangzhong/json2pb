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

#include "global.h"

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

/**
 * @brief for enum value, it help to check if the value is in the range of the enum.
 *
 * @param n_ranges n_value_ranges in the ProtobufCEnumDescriptor
 * @param ranges value_ranges in the ProtobufCEnumDescriptor
 * @param value value to be checked
 * @return int
 * @return >0 if the value is in the range of the enum.(which is the index of the range in the ProtobufCEnumDescriptor)
 * @return -1 if the value is not in the range of the enum.
 */
int int_range_lookup(unsigned n_ranges, const ProtobufCIntRange* ranges, int value);

/**
 * @brief the protobuf_c_enum_descriptor_get_value_by_name's case-sensitive version.
 *
 * @param desc enum descriptor
 * @param name name of the enum value
 * @return const ProtobufCEnumValue*
 * @retval NULL if the name is not found in the enum descriptor.
 */
const ProtobufCEnumValue* protobuf_c_enum_descriptor_get_value_by_name_case_insensitive(const ProtobufCEnumDescriptor* desc, const char* name);

#undef ALL_NONNULL
#undef NONNULL

#endif /* _CJSON_2_PROTOBUF_H_ */