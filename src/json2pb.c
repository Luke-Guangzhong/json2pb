/**
 * @file json2pb.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <assert.h>
#include <protobuf-c/protobuf-c.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson/cJSON.h"
#include "cjson/cJSON_Utils.h"
#include "internal.h"
#include "json2pb.h"

const j2p_expt_msg j2p_expt_msg_list[] = {
    {JSON2PB_SUCCESS,                  "success"                                               },
    {JSON2PB_NULL_VALUE,               "null value in json"                                    },
    {JSON2PB_VALUE_OVERFLOW,           "json value overflow"                                   },
    {JSON2PB_UNACCEPTABLE_JSON_TYPE,   "unacceptable json type"                                },
    {JSON2PB_INVALID_NUMBER_STRING,    "invalid number string"                                 },
    {JSON2PB_EMPTY_ARRAY,              "empty array in json"                                   },
    {JSON2PB_JSON_GENERAL,             "general error in json"                                 },
    {JSON2PB_UNINITIALIZED,            "protobuf message not initialized"                      },
    {JSON2PB_FIELD_NOT_FOUND,          "specified field not found in protobuf message"         },
    {JSON2PB_FIELD_IS_DEPRECATED,      "specified field already deprecated in protobuf message"},
    {JSON2PB_ONEOF_ALREADY_SET,        "field with oneof already set in protobuf message"      },
    {JSON2PB_PB_GENERAL,               "general error in protobuf message"                     },
    {JSON2PB_INVALID_ARG,              "pass invalid argument to function"                     },
    {JSON2PB_INCORRECT_EXCEPTION_TYPE, "throw an unknown exception type or this one"           },
    {JSON2PB_CODE_GENERAL,             "general error in coding"                               },
    {JSON2PB_MEM_ALLOC_FAILED,         "memory allocation failed"                              },
    {JSON2PB_OS_GENERAL,               "general error in operating system"                     },
};

static j2p_expt_t cvt_int32_t(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const ProtobufCFieldDescriptor* restrict field_desc);
static j2p_expt_t cvt_int64_t(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const ProtobufCFieldDescriptor* restrict field_desc);
static j2p_expt_t cvt_uint32_t(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const ProtobufCFieldDescriptor* restrict field_desc);
static j2p_expt_t cvt_uint64_t(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const ProtobufCFieldDescriptor* restrict field_desc);
static j2p_expt_t cvt_float(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const ProtobufCFieldDescriptor* restrict field_desc);
static j2p_expt_t cvt_double(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const ProtobufCFieldDescriptor* restrict field_desc);
static j2p_expt_t cvt_string(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const ProtobufCFieldDescriptor* restrict field_desc);
static j2p_expt_t cvt_bool(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const ProtobufCFieldDescriptor* restrict field_desc, const string_bool_convertor bool_cvt);

j2p_expt_t
cvt_cjson_2_proto_c_field(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const char* restrict field_name, const string_bool_convertor bool_cvt)
{
    assert(root != NULL);
    assert(msg != NULL);
    assert(item != NULL);
    assert(field_name != NULL);

    if (NULL == root || NULL == msg || NULL == item || NULL == field_name) {
        return JSON2PB_INVALID_ARG;
    }

    if (NULL == msg->descriptor) {
        return JSON2PB_UNINITIALIZED;
    }

    if (cJSON_IsNull(item)) {
        return JSON2PB_NULL_VALUE;
    }

    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->descriptor, field_name);
    if (NULL == field_desc) {
        return JSON2PB_FIELD_NOT_FOUND;
    }

    j2p_expt_t rtn = JSON2PB_SUCCESS;

    switch (field_desc->type) {
    case PROTOBUF_C_TYPE_INT32:
    case PROTOBUF_C_TYPE_SINT32:
    case PROTOBUF_C_TYPE_SFIXED32:
        rtn = cvt_int32_t(root, msg, item, field_desc);
        break;
    case PROTOBUF_C_TYPE_INT64:
    case PROTOBUF_C_TYPE_SINT64:
    case PROTOBUF_C_TYPE_SFIXED64:
        // rtn = cvt_int64_t(root, msg, item, field_desc);
        break;
    case PROTOBUF_C_TYPE_UINT32:
    case PROTOBUF_C_TYPE_FIXED32:
        // rtn = cvt_uint32_t(root, msg, item, field_desc);
        break;
    case PROTOBUF_C_TYPE_UINT64:
    case PROTOBUF_C_TYPE_FIXED64:
        // rtn = cvt_uint64_t(root, msg, item, field_desc);
        break;
    case PROTOBUF_C_TYPE_FLOAT:
        // rtn = cvt_float(root, msg, item, field_desc);
        break;
    case PROTOBUF_C_TYPE_DOUBLE:
        // rtn = cvt_double(root, msg, item, field_desc);
        break;
    case PROTOBUF_C_TYPE_STRING:
        // rtn = cvt_string(root, msg, item, field_desc);
        break;
    case PROTOBUF_C_TYPE_MESSAGE:
        // rtn = __cvt_pb_msg__(msg, field_desc, root, msg_convertor);
        break;
    case PROTOBUF_C_TYPE_BOOL:
        // rtn = cvt_bool(root, msg, item, field_desc, bool_cvt);
        break;
    case PROTOBUF_C_TYPE_BYTES:
        // rtn = __cvt_bytes__(msg, field_desc, root, mode);
        break;
    case PROTOBUF_C_TYPE_ENUM:
        // rtn = __cvt_enum__(msg, field_desc, root, string_enum);
        break;
    default:
        // printf("field %s cannot processed in json for now\n", field_desc->name);
        break;
    }

    return rtn;
}

static j2p_expt_t
cvt_int32_t(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const ProtobufCFieldDescriptor* restrict field_desc)
{
    assert(msg != NULL);
    assert(field_desc != NULL);
    assert(item != NULL);
    assert(msg->descriptor != NULL);
    assert(field_desc->type == PROTOBUF_C_TYPE_INT32 || field_desc->type == PROTOBUF_C_TYPE_SINT32 || field_desc->type == PROTOBUF_C_TYPE_SFIXED32);

    const bool is_repeated   = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);
    const bool is_oneof      = (field_desc->flags == PROTOBUF_C_FIELD_FLAG_ONEOF);
    const bool is_deprecated = (field_desc->flags == PROTOBUF_C_FIELD_FLAG_DEPRECATED);

    if (is_deprecated) {
        return JSON2PB_FIELD_IS_DEPRECATED;
    }

    if (is_repeated) {
        if (!cJSON_IsArray(item)) {
            return JSON2PB_UNACCEPTABLE_JSON_TYPE;
        }
        if (cJSON_GetArraySize(item) > 0) {
            uint64_t       count      = 0;
            const cJSON*   element    = NULL;
            const cJSON*   json_array = item;
            const uint64_t length     = cJSON_GetArraySize(json_array);
            j2p_expt_t     rtn        = JSON2PB_SUCCESS;
            int32_t* const array      = (int32_t*)calloc(length, sizeof(int32_t));
            if (NULL == array) {
                exit(EXIT_FAILURE);
            }

            cJSON_ArrayForEach(element, item)
            {
                rtn = cvt_single_int32_t(root, element, &array[count]);
                if (rtn != EXIT_SUCCESS) {
                    printf("[EXCEPTION]: %s %s\n", )
                } else {
                    count++;
                }
            }

            if (rtn->msg->type != JSON2PB_SUCCESS) { /* error occurred */
                free(array);
                return rtn;
            }

            if (count == 0) { /* no valid element found */
                free(array);
                FREE_JSON2PB_EXCEPTION(rtn);
                JSON2PB_THROW_EXCEPTION(JSON2PB_EMPTY_ARRAY);
            }

            int32_t** field_ptr = (int32_t**)((void*)msg + field_desc->offset);
            *field_ptr          = (int32_t*)calloc(count, sizeof(int32_t));
            if (NULL == (*field_ptr)) {
                free(array);
                JSON2PB_THROW_EXCEPTION(JSON2PB_MEM_ALLOC_FAILED);
            }

            memcpy((*field_ptr), array, count * sizeof(int32_t));
            *(size_t*)((void*)msg + field_desc->quantifier_offset) = count;

            free(array);
            JSON2PB_THROW_EXCEPTION(JSON2PB_SUCCESS);
        } else {
            return JSON2PB_EMPTY_ARRAY;
        }
    } else {
        if (is_oneof) {
            if ((*(int32_t*)((void*)msg + field_desc->quantifier_offset)) != 0) {
                return JSON2PB_ONEOF_ALREADY_SET;
            } else {
                (*(int32_t*)((void*)msg + field_desc->quantifier_offset)) = field_desc->id;
            }
        }
        int32_t* field_ptr = (int32_t*)((void*)msg + field_desc->offset);
        return cvt_single_int32_t(root, item, field_ptr);
    }
}
