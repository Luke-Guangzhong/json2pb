/**
 * @file default.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <string.h>

#include "internal.h"

bool
default_string_bool_convertor(const char* const str)
{
    assert(NULL != str);
    return 0 == strncmp("true", str, 4);
}

bool
default_string_bool_convertor_v2(const ProtobufCFieldDescriptor* const field_desc, const char* const str)
{
    assert(NULL != str);
    (void)field_desc;
    return 0 == strncmp("true", str, 4);
}

int
default_string_enum_convertor(const ProtobufCEnumDescriptor* const enum_desc, const char* const str)
{
    assert(NULL != enum_desc);
    assert(NULL != str);
    const ProtobufCEnumValue* enum_value = protobuf_c_enum_descriptor_get_value_by_name_case_insensitive(enum_desc, str);
    if (NULL == enum_value) {
        return -1;
    }
    return enum_value->value;
}