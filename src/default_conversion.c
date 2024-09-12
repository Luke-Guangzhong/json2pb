/**
 * @file default_conversion.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @date 2024-08-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>

#include "cjson2pb-c.h"

int
default_string_enum_conversion(const ProtobufCEnumDescriptor* const enum_desc, const char* const str)
{
    assert(str != NULL && enum_desc != NULL);
    const ProtobufCEnumValue* const enum_value = protobuf_c_enum_descriptor_get_value_by_name(enum_desc, str);
    if (NULL == enum_value) {
        return 0;
    } else {
        return enum_value->value;
    }
}

int
default_msg_convertor(ProtobufCMessage* const msg, const cJSON* const item)
{
    assert(msg != NULL && item != NULL && msg->descriptor != NULL);

    const ProtobufCFieldDescriptor*   field_desc = NULL;
    const ProtobufCMessageDescriptor* msg_desc   = msg->descriptor;
    const unsigned int                n_fields   = msg_desc->n_fields;

    for (int index = 0; index < n_fields; index++) {
        field_desc = msg_desc->fields + index;
        cvt_cjson_2_proto_c_field(cJSON_GetObjectItem(item, field_desc->name), msg, field_desc->name, NULL, NULL, BYTE_MODE_FILE_PATH);
    }

    return 0;
}