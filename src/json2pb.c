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
    {J2P_EXPT_SUCCESS,                  "success"                                               },
    {J2P_EXPT_NULL_VALUE,               "null value in json"                                    },
    {J2P_EXPT_VALUE_OVERFLOW,           "json value overflow"                                   },
    {J2P_EXPT_UNACCEPTABLE_JSON_TYPE,   "unacceptable json type"                                },
    {J2P_EXPT_INVALID_NUMBER_STRING,    "invalid number string"                                 },
    {J2P_EXPT_EMPTY_ARRAY,              "empty array in json"                                   },
    {J2P_EXPT_PARTIAL_FAIL,             "partial fail when convert json array to repeated field"},
    {J2P_EXPT_NO_VALID_FOUND,           "all element in array are not valid for this field"     },
    {J2P_EXPT_JSON_GENERAL,             "general error in json"                                 },
    {J2P_EXPT_UNINITIALIZED,            "protobuf message not initialized"                      },
    {J2P_EXPT_FIELD_NOT_FOUND,          "specified field not found in protobuf message"         },
    {J2P_EXPT_FIELD_IS_DEPRECATED,      "specified field already deprecated in protobuf message"},
    {J2P_EXPT_ONEOF_ALREADY_SET,        "field with oneof already set in protobuf message"      },
    {J2P_EXPT_PB_GENERAL,               "general error in protobuf message"                     },
    {J2P_EXPT_INVALID_ARG,              "pass invalid argument to function"                     },
    {J2P_EXPT_INCORRECT_EXCEPTION_TYPE, "throw an unknown exception type or this one"           },
    {J2P_EXPT_CODE_GENERAL,             "general error in coding"                               },
    {J2P_EXPT_MEM_ALLOC_FAILED,         "memory allocation failed"                              },
    {J2P_EXPT_OS_GENERAL,               "general error in operating system"                     },
};

static j2p_expt_t cvt_int32_t(const cJSON* root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc);
static j2p_expt_t cvt_int64_t(const cJSON* const root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc);

// static j2p_expt_t cvt_uint32_t(const cJSON* root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc);
// static j2p_expt_t cvt_uint64_t(const cJSON* root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc);
// static j2p_expt_t cvt_float(const cJSON* root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc);
// static j2p_expt_t cvt_double(const cJSON* root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc);
// static j2p_expt_t cvt_string(const cJSON* root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc);
// static j2p_expt_t cvt_bool(const cJSON* root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc, const string_bool_convertor bool_cvt);

static j2p_expt_t
cvt_numeric(const cJSON* const root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc, const size_t elem_size, single_field_cvt_func cvt_func);

j2p_expt_t
cvt_json_2_pb_field(const cJSON*                root,
                    const cJSON*                item,
                    ProtobufCMessage* const     msg,
                    const char* const           field_name,
                    const string_bool_convertor bool_cvt,
                    const string_enum_convertor enum_cvt,
                    const j2p_file_t            file_type)
{
    assert(root != NULL);
    assert(msg != NULL);
    assert(item != NULL);
    assert(field_name != NULL);

    if (NULL == root || NULL == msg || NULL == item || NULL == field_name) {
        return J2P_EXPT_INVALID_ARG;
    }

    if (NULL == msg->descriptor) {
        return J2P_EXPT_UNINITIALIZED;
    }

    if (cJSON_IsNull(item)) {
        return J2P_EXPT_NULL_VALUE;
    }

    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->descriptor, field_name);
    if (NULL == field_desc) {
        return J2P_EXPT_FIELD_NOT_FOUND;
    }

    const bool is_oneof      = (field_desc->flags == PROTOBUF_C_FIELD_FLAG_ONEOF);
    const bool is_deprecated = (field_desc->flags == PROTOBUF_C_FIELD_FLAG_DEPRECATED);

    if (is_deprecated) {
        return J2P_EXPT_FIELD_IS_DEPRECATED;
    }

    if (is_oneof && *(int32_t*)((void*)msg + field_desc->quantifier_offset) != 0) {
        return J2P_EXPT_ONEOF_ALREADY_SET;
    }

    j2p_expt_t rtn = J2P_EXPT_SUCCESS;

    switch (field_desc->type) {
    case PROTOBUF_C_TYPE_INT32:
    case PROTOBUF_C_TYPE_SINT32:
    case PROTOBUF_C_TYPE_SFIXED32:
        // rtn = cvt_int32_t(root, msg, item, field_desc);
        rtn = cvt_numeric(root, msg, item, field_desc, sizeof(int32_t), (single_field_cvt_func)cvt_single_int32_t);
        break;
    case PROTOBUF_C_TYPE_INT64:
    case PROTOBUF_C_TYPE_SINT64:
    case PROTOBUF_C_TYPE_SFIXED64:
        rtn = cvt_int64_t(root, msg, item, field_desc);
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
    case PROTOBUF_C_TYPE_BOOL:
        // rtn = cvt_bool(root, msg, item, field_desc, bool_cvt);
        break;
    case PROTOBUF_C_TYPE_ENUM:
        // rtn = __cvt_enum__(msg, field_desc, root, string_enum);
        break;
    case PROTOBUF_C_TYPE_STRING:
        // rtn = cvt_string(root, msg, item, field_desc);
        break;
    case PROTOBUF_C_TYPE_MESSAGE:
        // rtn = __cvt_pb_msg__(msg, field_desc, root, msg_convertor);
        break;
    case PROTOBUF_C_TYPE_BYTES:
        // rtn = __cvt_bytes__(msg, field_desc, root, mode);
        break;
    default:
        // printf("field %s cannot processed in json for now\n",
        // field_desc->name);
        break;
    }

    return rtn;
}

static j2p_expt_t
cvt_int32_t(const cJSON* const root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc)
{
    assert(msg != NULL);
    assert(field_desc != NULL);
    assert(item != NULL);
    assert(msg->descriptor != NULL);
    assert(field_desc->type == PROTOBUF_C_TYPE_INT32 || field_desc->type == PROTOBUF_C_TYPE_SINT32 || field_desc->type == PROTOBUF_C_TYPE_SFIXED32);

    if (NULL == msg || NULL == field_desc || NULL == item || NULL == msg->descriptor ||
        !(field_desc->type != PROTOBUF_C_TYPE_INT32 || field_desc->type != PROTOBUF_C_TYPE_SINT32 || field_desc->type != PROTOBUF_C_TYPE_SFIXED32)) {
        return J2P_EXPT_INVALID_ARG;
    }

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);
    const bool is_oneof    = (field_desc->flags == PROTOBUF_C_FIELD_FLAG_ONEOF);

    if (is_repeated) {
        if (!cJSON_IsArray(item)) {
            return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
        }
        if (cJSON_GetArraySize(item) > 0) {
            uint64_t       count      = 0;
            const cJSON*   element    = NULL;
            const cJSON*   json_array = item;
            const uint64_t length     = cJSON_GetArraySize(json_array);
            j2p_expt_t     rtn        = J2P_EXPT_SUCCESS;
            int32_t* const array      = (int32_t*)calloc(length, sizeof(int32_t));
            if (NULL == array) {
                exit(EXIT_FAILURE);
            }

            cJSON_ArrayForEach(element, item)
            {
                rtn = cvt_single_int32_t(element, &array[count]);
                if (rtn != EXIT_SUCCESS) {
                    char* path = cJSONUtils_FindPointerFromObjectTo(root, element);
                    printf("[EXCEPTION]: %s %s\n", path, j2p_expt_msg_list[rtn].desc);
                    free(path);
                } else {
                    count++;
                }
            }

            if (count == 0) { /* no valid element found */
                free(array);
                return J2P_EXPT_NO_VALID_FOUND;
            }

            int32_t** field_ptr = (int32_t**)((void*)msg + field_desc->offset);
            *field_ptr          = (int32_t*)calloc(count, sizeof(int32_t));
            if (NULL == (*field_ptr)) {
                exit(EXIT_FAILURE);
            }

            memcpy((*field_ptr), array, count * sizeof(int32_t));
            *(size_t*)((void*)msg + field_desc->quantifier_offset) = count;

            free(array);
            if (count == length)
                return J2P_EXPT_SUCCESS;
            else
                return J2P_EXPT_PARTIAL_FAIL;
        } else {
            return J2P_EXPT_EMPTY_ARRAY;
        }
    } else {
        if (is_oneof) {
            (*(int32_t*)((void*)msg + field_desc->quantifier_offset)) = field_desc->id;
        }
        int32_t* field_ptr = (int32_t*)((void*)msg + field_desc->offset);
        return cvt_single_int32_t(item, field_ptr);
    }
}

static j2p_expt_t
cvt_int64_t(const cJSON* const root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc)
{
    assert(msg != NULL);
    assert(field_desc != NULL);
    assert(item != NULL);
    assert(msg->descriptor != NULL);
    assert(field_desc->type == PROTOBUF_C_TYPE_INT64 || field_desc->type == PROTOBUF_C_TYPE_SINT64 || field_desc->type == PROTOBUF_C_TYPE_SFIXED64);

    if (NULL == msg || NULL == field_desc || NULL == item || NULL == msg->descriptor ||
        !(field_desc->type != PROTOBUF_C_TYPE_INT64 || field_desc->type != PROTOBUF_C_TYPE_SINT64 || field_desc->type != PROTOBUF_C_TYPE_SFIXED64)) {
        return J2P_EXPT_INVALID_ARG;
    }

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);
    const bool is_oneof    = (field_desc->flags == PROTOBUF_C_FIELD_FLAG_ONEOF);

    if (is_repeated) {
        if (!cJSON_IsArray(item)) {
            return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
        }
        if (cJSON_GetArraySize(item) > 0) {
            uint64_t       count      = 0;
            const cJSON*   element    = NULL;
            const cJSON*   json_array = item;
            const uint64_t length     = cJSON_GetArraySize(json_array);
            j2p_expt_t     rtn        = J2P_EXPT_SUCCESS;
            int64_t* const array      = (int64_t*)calloc(length, sizeof(int64_t));
            if (NULL == array) {
                exit(EXIT_FAILURE);
            }

            cJSON_ArrayForEach(element, item)
            {
                rtn = cvt_single_int64_t(element, &array[count]);
                if (rtn != EXIT_SUCCESS) {
                    char* path = cJSONUtils_FindPointerFromObjectTo(root, element);
                    printf("[EXCEPTION]: %s %s\n", path, j2p_expt_msg_list[rtn].desc);
                    free(path);
                } else {
                    count++;
                }
            }

            if (count == 0) { /* no valid element found */
                free(array);
                return J2P_EXPT_NO_VALID_FOUND;
            }

            int64_t** field_ptr = (int64_t**)((void*)msg + field_desc->offset);
            *field_ptr          = (int64_t*)calloc(count, sizeof(int64_t));
            if (NULL == (*field_ptr)) {
                exit(EXIT_FAILURE);
            }

            memcpy((*field_ptr), array, count * sizeof(int64_t));
            *(size_t*)((void*)msg + field_desc->quantifier_offset) = count;

            free(array);
            if (count == length)
                return J2P_EXPT_SUCCESS;
            else
                return J2P_EXPT_PARTIAL_FAIL;
        } else {
            return J2P_EXPT_EMPTY_ARRAY;
        }
    } else {
        if (is_oneof) {
            (*(int32_t*)((void*)msg + field_desc->quantifier_offset)) = field_desc->id;
        }
        int64_t* field_ptr = (int64_t*)((void*)msg + field_desc->offset);
        return cvt_single_int64_t(item, field_ptr);
    }
}

static j2p_expt_t
cvt_numeric(const cJSON* const root, ProtobufCMessage* msg, const cJSON* item, const ProtobufCFieldDescriptor* field_desc, const size_t elem_size, single_field_cvt_func cvt_func)
{
    assert(msg != NULL);
    assert(field_desc != NULL);
    assert(item != NULL);
    assert(msg->descriptor != NULL);
    assert(field_desc->type != PROTOBUF_C_TYPE_STRING && field_desc->type != PROTOBUF_C_TYPE_MESSAGE && field_desc->type != PROTOBUF_C_TYPE_BYTES);

    if (NULL == msg || NULL == field_desc || NULL == item || NULL == msg->descriptor ||
        !(field_desc->type != PROTOBUF_C_TYPE_STRING && field_desc->type != PROTOBUF_C_TYPE_MESSAGE && field_desc->type != PROTOBUF_C_TYPE_BYTES)) {
        return J2P_EXPT_INVALID_ARG;
    }

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);
    const bool is_oneof    = (field_desc->flags == PROTOBUF_C_FIELD_FLAG_ONEOF);

    if (is_repeated) {
        if (!cJSON_IsArray(item)) {
            return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
        }
        if (cJSON_GetArraySize(item) > 0) {
            uint64_t       count      = 0;
            const cJSON*   element    = NULL;
            const cJSON*   json_array = item;
            const uint64_t length     = cJSON_GetArraySize(json_array);
            j2p_expt_t     rtn        = J2P_EXPT_SUCCESS;
            void* const    array      = (void*)calloc(length, elem_size);
            if (NULL == array) {
                exit(EXIT_FAILURE);
            }

            cJSON_ArrayForEach(element, item)
            {
                rtn = cvt_func(element, array + (count * elem_size));
                if (rtn != EXIT_SUCCESS) {
                    char* path = cJSONUtils_FindPointerFromObjectTo(root, element);
                    printf("[EXCEPTION]: %s %s\n", path, j2p_expt_msg_list[rtn].desc);
                    free(path);
                } else {
                    count++;
                }
            }

            if (count == 0) { /* no valid element found */
                free(array);
                return J2P_EXPT_NO_VALID_FOUND;
            }

            void** field_ptr = (void**)((void*)msg + field_desc->offset);
            *field_ptr       = (void*)calloc(count, elem_size);
            if (NULL == (*field_ptr)) {
                exit(EXIT_FAILURE);
            }

            memcpy((*field_ptr), array, count * elem_size);
            *(size_t*)((void*)msg + field_desc->quantifier_offset) = count;

            free(array);
            if (count == length)
                return J2P_EXPT_SUCCESS;
            else
                return J2P_EXPT_PARTIAL_FAIL;
        } else {
            return J2P_EXPT_EMPTY_ARRAY;
        }
    } else {
        if (is_oneof) {
            (*(int32_t*)((void*)msg + field_desc->quantifier_offset)) = field_desc->id;
        }
        void* field_ptr = (void*)msg + field_desc->offset;
        return cvt_func(item, field_ptr);
    }
}