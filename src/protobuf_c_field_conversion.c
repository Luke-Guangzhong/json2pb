/**
 * @file protobuf_c_field_conversion.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @date 2024-07-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _GNU_SOURCE
#define _GUN_SOURCE
#endif

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson/cJSON.h"
#include "cjson2pb-c.h"
#include "trower-base64/ver.h"

/* Internal Functions */

/* Declarations */

static uint32_t __cvt_int32_t__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item) __attribute__((__nonnull__));
static uint32_t __cvt_int64_t__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item) __attribute__((__nonnull__));
static uint32_t __cvt_uint32_t__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item) __attribute__((__nonnull__));
static uint32_t __cvt_uint64_t__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item) __attribute__((__nonnull__));
static uint32_t __cvt_float__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item) __attribute__((__nonnull__));
static uint32_t __cvt_double__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item) __attribute__((__nonnull__));
static uint32_t __cvt_bool__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item, StringEnumCallback string_enum) __attribute__((__nonnull__(1, 2, 3)));
static uint32_t __cvt_string__(ProtobufCMessage* const msg, const ProtobufCFieldDescriptor* const field_desc, const cJSON* const item) __attribute__((__nonnull__));
static uint32_t __cvt_bytes__(ProtobufCMessage* const msg, const ProtobufCFieldDescriptor* const field_desc, const cJSON* const item, const ByteMode mode) __attribute__((__nonnull__));
static uint32_t __cvt_enum__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item, StringEnumCallback string_enum) __attribute__((__nonnull__(1, 2, 3)));
static uint32_t __cvt_pb_msg__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item, MsgConvertorCallback msg_convertor) __attribute__((__nonnull__(1, 2, 3)));

/* Definitions */

static uint32_t
__cvt_int32_t__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && (field_desc->type == PROTOBUF_C_TYPE_INT32 || field_desc->type == PROTOBUF_C_TYPE_SINT32 || field_desc->type == PROTOBUF_C_TYPE_SFIXED32));

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);

    if (is_repeated) {
        if (!cJSON_IsArray(item)) {
            return E_UNACCEPTABLE_JSON_TYPE;
        }
        if (cJSON_GetArraySize(item) > 0) {
            uint32_t       rtn_code = E_SUCCESS;
            uint64_t       index    = 0;
            const cJSON*   element  = NULL;
            const uint64_t length   = cJSON_GetArraySize(item);
            int32_t* const array    = (int32_t*)calloc(length, sizeof(int32_t));
            if (NULL == array) {
                return E_MEM_ALLOC;
            }

            cJSON_ArrayForEach(element, item)
            {
                rtn_code |= cvt_single_int32_t(&array[index], element);
                index++;
            }

            /* TODO: check if array is valid, if not, free it and retrun an error code here */

            *(int32_t**)((void*)msg + field_desc->offset)          = array;
            *(size_t*)((void*)msg + field_desc->quantifier_offset) = length;
            return rtn_code;
        } else {
            return E_EMPTY_ARRAY;
        }
    } else {
        int32_t* field_ptr = (int32_t*)((void*)msg + field_desc->offset);
        return cvt_single_int32_t(field_ptr, item);
    }
}

static uint32_t
__cvt_int64_t__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && (field_desc->type == PROTOBUF_C_TYPE_INT64 || field_desc->type == PROTOBUF_C_TYPE_SINT64 || field_desc->type == PROTOBUF_C_TYPE_SFIXED64));

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);

    if (is_repeated) {
        if (!cJSON_IsArray(item)) {
            return E_UNACCEPTABLE_JSON_TYPE;
        }
        if (cJSON_GetArraySize(item) > 0) {
            uint32_t       rtn_code = E_SUCCESS;
            uint64_t       index    = 0;
            const cJSON*   element  = NULL;
            const uint64_t length   = cJSON_GetArraySize(item);
            int64_t* const array    = (int64_t*)calloc(length, sizeof(int64_t));
            if (NULL == array) {
                return E_MEM_ALLOC;
            }

            cJSON_ArrayForEach(element, item)
            {
                rtn_code |= cvt_single_int64_t(&array[index], element);
                index++;
            }

            *(int64_t**)((void*)msg + field_desc->offset)          = array;
            *(size_t*)((void*)msg + field_desc->quantifier_offset) = length;
            return rtn_code;
        } else {
            return E_EMPTY_ARRAY;
        }
    } else {
        int64_t* field_ptr = (int64_t*)((void*)msg + field_desc->offset);
        return cvt_single_int64_t(field_ptr, item);
    }
}

static uint32_t
__cvt_uint32_t__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && (field_desc->type == PROTOBUF_C_TYPE_UINT32 || field_desc->type == PROTOBUF_C_TYPE_FIXED32));

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);

    if (is_repeated) {
        if (!cJSON_IsArray(item)) {
            return E_UNACCEPTABLE_JSON_TYPE;
        }
        if (cJSON_GetArraySize(item) > 0) {
            uint32_t        rtn_code = E_SUCCESS;
            uint64_t        index    = 0;
            const cJSON*    element  = NULL;
            const uint64_t  length   = cJSON_GetArraySize(item);
            uint32_t* const array    = (uint32_t*)calloc(length, sizeof(uint32_t));
            if (NULL == array) {
                return E_MEM_ALLOC;
            }

            cJSON_ArrayForEach(element, item)
            {
                rtn_code |= cvt_single_uint32_t(&array[index], element);
                index++;
            }

            *(uint32_t**)((void*)msg + field_desc->offset)         = array;
            *(size_t*)((void*)msg + field_desc->quantifier_offset) = length;
            return rtn_code;
        } else {
            return E_EMPTY_ARRAY;
        }
    } else {
        uint32_t* field_ptr = (uint32_t*)((void*)msg + field_desc->offset);
        return cvt_single_uint32_t(field_ptr, item);
    }
}

static uint32_t
__cvt_uint64_t__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && (field_desc->type == PROTOBUF_C_TYPE_UINT64 || field_desc->type == PROTOBUF_C_TYPE_FIXED64));

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);

    if (is_repeated) {
        if (!cJSON_IsArray(item)) {
            return E_UNACCEPTABLE_JSON_TYPE;
        }
        if (cJSON_GetArraySize(item) > 0) {
            uint32_t        rtn_code = E_SUCCESS;
            uint64_t        index    = 0;
            const cJSON*    element  = NULL;
            const uint64_t  length   = cJSON_GetArraySize(item);
            uint64_t* const array    = (uint64_t*)calloc(length, sizeof(uint64_t));
            if (NULL == array) {
                return E_MEM_ALLOC;
            }

            cJSON_ArrayForEach(element, item)
            {
                rtn_code |= cvt_single_uint64_t(&array[index], element);
                index++;
            }

            *(uint64_t**)((void*)msg + field_desc->offset)         = array;
            *(size_t*)((void*)msg + field_desc->quantifier_offset) = length;
            return rtn_code;
        } else {
            return E_EMPTY_ARRAY;
        }
    } else {
        uint64_t* field_ptr = (uint64_t*)((void*)msg + field_desc->offset);
        return cvt_single_uint64_t(field_ptr, item);
    }
}

static uint32_t
__cvt_float__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && (field_desc->type == PROTOBUF_C_TYPE_FLOAT));

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);

    if (is_repeated) {
        if (!cJSON_IsArray(item)) {
            return E_UNACCEPTABLE_JSON_TYPE;
        }
        if (cJSON_GetArraySize(item) > 0) {
            const uint64_t length  = cJSON_GetArraySize(item);
            float*         array   = (float*)calloc(length, sizeof(float));
            uint64_t       index   = 0;
            const cJSON*   element = NULL;
            cJSON_ArrayForEach(element, item)
            {
                cvt_single_float(&array[index], element);
                index++;
            }

            if (length == index) {
                *(float**)((void*)msg + field_desc->offset)            = array;
                *(size_t*)((void*)msg + field_desc->quantifier_offset) = length;
                return 0;
            } else {
                free(array);
                fprintf(stderr, "JSON field %s is not a valid array of numbers or number strings in decimal\n", item->string);
                return -1;
            }
        } else {
            fprintf(stderr, "JSON %s is not an valid or non-empty array of float type numbers or number strings in decimal\n", item->string);
            return -1;
        }
    } else {
        float* field_ptr = (float*)((void*)msg + field_desc->offset);
        return cvt_single_float(field_ptr, item);
    }
}

static uint32_t
__cvt_double__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && (field_desc->type == PROTOBUF_C_TYPE_DOUBLE));

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);

    if (is_repeated) {
        if (cJSON_IsArray(item) && cJSON_GetArraySize(item) > 0) {
            const uint64_t length  = cJSON_GetArraySize(item);
            double*        array   = (double*)calloc(length, sizeof(double));
            uint64_t       index   = 0;
            const cJSON*   element = NULL;
            cJSON_ArrayForEach(element, item)
            {
                cvt_single_double(&array[index], element);
                index++;
            }

            if (length == index) {
                *(double**)((void*)msg + field_desc->offset)           = array;
                *(size_t*)((void*)msg + field_desc->quantifier_offset) = length;
                return 0;
            } else {
                free(array);
                fprintf(stderr, "JSON field %s is not a valid array of numbers or number strings in decimal\n", item->string);
                return -1;
            }
        } else {
            fprintf(stderr, "JSON %s is not an valid or non-empty array of double type numbers or number strings in decimal\n", item->string);
            return -1;
        }
    } else {
        double* field_ptr = (double*)((void*)msg + field_desc->offset);
        return cvt_single_double(field_ptr, item);
    }
}

static uint32_t
__cvt_bool__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item, StringEnumCallback string_enum)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && field_desc->type == PROTOBUF_C_TYPE_BOOL);

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);

    if (is_repeated) {
        if (cJSON_IsArray(item) && cJSON_GetArraySize(item) > 0) {
            const uint64_t length  = cJSON_GetArraySize(item);
            bool*          array   = (bool*)calloc(length, sizeof(bool));
            uint64_t       index   = 0;
            const cJSON*   element = NULL;
            cJSON_ArrayForEach(element, item)
            {
                cvt_single_bool(&array[index], element, string_enum);
                index++;
            }

            if (length == index) {
                *(bool**)((void*)msg + field_desc->offset)             = array;
                *(size_t*)((void*)msg + field_desc->quantifier_offset) = length;
                return 0;
            } else {
                free(array);
                fprintf(stderr, "JSON field %s is not a valid array of numbers or number strings in decimal\n", item->string);
                return -1;
            }
        } else {
            fprintf(stderr, "JSON %s is not an valid or non-empty array of booleans\n", item->string);
            return -1;
        }
    } else {
        bool* field_ptr = (bool*)((void*)msg + field_desc->offset);
        return cvt_single_bool(field_ptr, item, string_enum);
    }
}

static uint32_t
__cvt_string__(ProtobufCMessage* const msg, const ProtobufCFieldDescriptor* const field_desc, const cJSON* const item)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && field_desc->type == PROTOBUF_C_TYPE_STRING);

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);

    if (is_repeated) {
        if (cJSON_IsArray(item) && cJSON_GetArraySize(item) > 0) {
            const uint64_t length  = cJSON_GetArraySize(item);
            char**         array   = (char**)calloc(length, sizeof(char*));
            uint64_t       index   = 0;
            const cJSON*   element = NULL;
            cJSON_ArrayForEach(element, item)
            {
                cvt_single_string(&array[index], element);
                index++;
            }

            if (length == index) {
                *(char***)((void*)msg + field_desc->offset)            = array;
                *(size_t*)((void*)msg + field_desc->quantifier_offset) = length;
                return 0;
            } else {
                free(array);
                fprintf(stderr, "JSON field %s is not a valid array of numbers or number strings in decimal\n", item->string);
                return -1;
            }
        } else {
            fprintf(stderr, "JSON %s is not an valid or non-empty array of strings\n", item->string);
            return -1;
        }
    } else {
        return cvt_single_string((void*)msg + field_desc->offset, item);
    }
}

static uint32_t
__cvt_bytes__(ProtobufCMessage* const msg, const ProtobufCFieldDescriptor* const field_desc, const cJSON* const item, const ByteMode mode)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && field_desc->type == PROTOBUF_C_TYPE_BYTES);

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);

    if (is_repeated) {
        if (cJSON_IsArray(item) && cJSON_GetArraySize(item) > 0) {
            const uint64_t       length  = cJSON_GetArraySize(item);
            ProtobufCBinaryData* array   = (ProtobufCBinaryData*)calloc(length, sizeof(ProtobufCBinaryData));
            uint64_t             index   = 0;
            const cJSON*         element = NULL;
            cJSON_ArrayForEach(element, item)
            {
                cvt_single_bytes(&array[index], element, mode);
                index++;
            }

            if (length == index) {
                *(ProtobufCBinaryData**)((void*)msg + field_desc->offset) = array;
                *(size_t*)((void*)msg + field_desc->quantifier_offset)    = length;
                return 0;
            } else {
                free(array);
                fprintf(stderr, "JSON field %s is not a valid array of numbers or number strings in decimal\n", item->string);
                return -1;
            }
        } else {
            fprintf(stderr, "JSON %s is not an valid or non-empty array of strings\n", item->string);
            return -1;
        }
    } else {
        return cvt_single_bytes((ProtobufCBinaryData*)((void*)msg + field_desc->offset), item, mode);
    }
}

static uint32_t
__cvt_enum__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item, StringEnumCallback string_enum)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && field_desc->type == PROTOBUF_C_TYPE_ENUM);

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);

    if (is_repeated) {
        if (cJSON_IsArray(item) && cJSON_GetArraySize(item) > 0) {
            const uint64_t                       length    = cJSON_GetArraySize(item);
            int*                                 array     = (int*)calloc(length, sizeof(int));
            uint64_t                             index     = 0;
            const cJSON*                         element   = NULL;
            const ProtobufCEnumDescriptor* const enum_desc = field_desc->descriptor;
            cJSON_ArrayForEach(element, item)
            {
                cvt_single_enum(&array[index], enum_desc, element, string_enum);
                index++;
            }

            if (length == index) {
                *(int**)((void*)msg + field_desc->offset)              = array;
                *(size_t*)((void*)msg + field_desc->quantifier_offset) = length;
                return 0;
            } else {
                free(array);
                fprintf(stderr, "JSON field %s is not a valid array of numbers or number strings in decimal\n", item->string);
                return -1;
            }
        } else {
            fprintf(stderr, "JSON %s is not an valid or non-empty array of enum value\n", item->string);
            return -1;
        }
    } else {
        const ProtobufCEnumDescriptor* const enum_desc = field_desc->descriptor;
        return cvt_single_enum((void*)msg + field_desc->offset, enum_desc, item, string_enum);
    }
}

static uint32_t
__cvt_pb_msg__(ProtobufCMessage* msg, const ProtobufCFieldDescriptor* field_desc, const cJSON* item, MsgConvertorCallback msg_convertor)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && field_desc->type == PROTOBUF_C_TYPE_MESSAGE);

    const bool is_repeated = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);

    if (is_repeated) {
        if (cJSON_IsArray(item) && cJSON_GetArraySize(item) > 0) {
            const uint64_t                    length   = cJSON_GetArraySize(item);
            ProtobufCMessage**                array    = (ProtobufCMessage**)calloc(length, sizeof(ProtobufCMessage*));
            uint64_t                          index    = 0;
            const cJSON*                      element  = NULL;
            const ProtobufCMessageDescriptor* msg_desc = field_desc->descriptor;

            cJSON_ArrayForEach(element, item)
            {
                array[index] = (ProtobufCMessage*)calloc(1, msg_desc->sizeof_message);
                msg_desc->message_init(array[index]);
                cvt_single_message(array[index], element, msg_convertor);
                index++;
            }

            if (length == index) {
                *(ProtobufCMessage***)((void*)msg + field_desc->offset) = array;
                *(size_t*)((void*)msg + field_desc->quantifier_offset)  = length;
                return 0;
            } else {
                for (size_t i = 0; i < index; i++) {
                    protobuf_c_message_free_unpacked(array[i], NULL);
                }
                free(array);
                fprintf(stderr, "JSON field %s is not a valid array of numbers or number strings in decimal\n", item->string);
                return -1;
            }
        } else {
            fprintf(stderr, "JSON %s is not an valid or non-empty array of numbers or number strings in decimal\n", item->string);
            return -1;
        }
    } else {
        const ProtobufCMessageDescriptor* msg_desc         = field_desc->descriptor;
        ProtobufCMessage*                 pb_msg_field_ptr = (ProtobufCMessage*)calloc(1, msg_desc->sizeof_message);
        msg_desc->message_init(pb_msg_field_ptr);
        if (cvt_single_message(pb_msg_field_ptr, item, msg_convertor) != 0) {
            fprintf(stderr, "msg_convertor is failed, free allocated memory\n");
            protobuf_c_message_free_unpacked(pb_msg_field_ptr, NULL);
            return -1;
        } else {
            *(ProtobufCMessage**)((void*)msg + field_desc->offset) = pb_msg_field_ptr;
            return 0;
        }
    }
}

/* External Functions */

uint32_t
cvt_cjson_2_proto_c_field(const cJSON* restrict root, ProtobufCMessage* restrict msg, const char* restrict field_name, StringEnumCallback const string_enum, MsgConvertorCallback const msg_convertor, const ByteMode mode)
{
    if (NULL == root || NULL == msg || NULL == field_name) {
        return E_INVALID_ARG;
    }

    if (NULL == msg->descriptor) {
        return E_UNINITIALIZED;
    }

    if (cJSON_IsNull(root)) {
        return E_NULL_VALUE;
    }

    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->descriptor, field_name);
    if (NULL == field_desc) {
        return E_INVALID_FIELD_NAME;
    }

    uint32_t rtn = E_SUCCESS;

    switch (field_desc->type) {
    case PROTOBUF_C_TYPE_INT32:
    case PROTOBUF_C_TYPE_SINT32:
    case PROTOBUF_C_TYPE_SFIXED32:
        rtn = __cvt_int32_t__(msg, field_desc, root);
        break;
    case PROTOBUF_C_TYPE_INT64:
    case PROTOBUF_C_TYPE_SINT64:
    case PROTOBUF_C_TYPE_SFIXED64:
        rtn = __cvt_int64_t__(msg, field_desc, root);
        break;
    case PROTOBUF_C_TYPE_UINT32:
    case PROTOBUF_C_TYPE_FIXED32:
        rtn = __cvt_uint32_t__(msg, field_desc, root);
        break;
    case PROTOBUF_C_TYPE_UINT64:
    case PROTOBUF_C_TYPE_FIXED64:
        rtn = __cvt_uint64_t__(msg, field_desc, root);
        break;
    case PROTOBUF_C_TYPE_FLOAT:
        rtn = __cvt_float__(msg, field_desc, root);
        break;
    case PROTOBUF_C_TYPE_DOUBLE:
        rtn = __cvt_double__(msg, field_desc, root);
        break;
    case PROTOBUF_C_TYPE_STRING:
        rtn = __cvt_string__(msg, field_desc, root);
        break;
    case PROTOBUF_C_TYPE_MESSAGE:
        rtn = __cvt_pb_msg__(msg, field_desc, root, msg_convertor);
        break;
    case PROTOBUF_C_TYPE_BOOL:
        rtn = __cvt_bool__(msg, field_desc, root, string_enum);
        break;
    case PROTOBUF_C_TYPE_BYTES:
        rtn = __cvt_bytes__(msg, field_desc, root, mode);
        break;
    case PROTOBUF_C_TYPE_ENUM:
        rtn = __cvt_enum__(msg, field_desc, root, string_enum);
        break;
    default:
        printf("field %s cannot processed in json for now\n", field_desc->name);
        break;
    }

    return rtn;
}

const char*
json_2_proto_c_version(void)
{
}

const char*
json_2_protoc_dependencies(void)
{
}

const int
parse_rtn_bit_map(const uint32_t map, char** const restrict desc_list)
{
    if (NULL == desc_list) {
        return -1;
    }

    char* parsed_data = (char*)calloc(1, 1024);
    if (NULL == parsed_data) {
        return -2;
    }

    const int err_count = __builtin_popcount(map);

    if (err_count == 0) {
        return err_count;
    }

    const int max_index = sizeof(rtn_code_enum_list) / sizeof(rtn_code_enum_list[0]);

    strcat(parsed_data, "Exception:\n");

    for (size_t index = 0; index < max_index; index++) {
        if ((map & rtn_code_enum_list[index]) != 0x00000000) {
            printf("index: %ld, value: %d, desc: %s\n", index, rtn_code_enum_list[index], rtn_code_desc_list[index]);
            strcat(parsed_data, rtn_code_desc_list[index]);
        }
    }

    asprintf(desc_list, "%s", parsed_data);

    free(parsed_data);

    return err_count;
}