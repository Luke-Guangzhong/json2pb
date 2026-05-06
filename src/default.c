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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson/cJSON_Utils.h"
#include "internal.h"

bool
default_string_bool_convertor(const char* const str)
{
    assert(NULL != str);
    return 0 == strncmp("true", str, 4);
}

int
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

j2p_expt_t
default_obj_msg_convertor(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg)
{
    assert(NULL != root);
    assert(NULL != item);
    assert(NULL != msg);
    assert(NULL != msg->descriptor);

    j2p_expt_t rtn = J2P_EXPT_SUCCESS;

    for (int i = 0; i < msg->descriptor->n_fields; i++) {
        const ProtobufCFieldDescriptor* field_desc = msg->descriptor->fields + i;
        if (cJSON_HasObjectItem(item, field_desc->name)) {
            rtn = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(item, field_desc->name), msg, field_desc->name, (j2p_add_cvt){0});
            if (rtn != J2P_EXPT_SUCCESS) {
                char* path = cJSONUtils_FindPointerFromObjectTo(root, cJSON_GetObjectItem(item, field_desc->name));
                printf("[EXCEPTION]: %s %s\n", path, j2p_expt_msg_list[rtn].desc);
                free(path);
            }
        } else {
            char* path = cJSONUtils_FindPointerFromObjectTo(root, item);
            printf("[EXCEPTION]: field %s not found in item %s\n", field_desc->name, path);
            free(path);
        }
    }

    return rtn;
}