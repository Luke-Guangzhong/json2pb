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

#include "cjson/cJSON.h"
#include "cjson/cJSON_Utils.h"
#include "json2pb.h"

const j2p_expt_msg j2p_expt_msg_list[] = {
    {JSON2PB_SUCCESS,                  "success"                                               },
    {JSON2PB_NULL_VALUE,               "null value in json"                                    },
    {JSON2PB_JSON_GENERAL,             "general error in json"                                 },
    {JSON2PB_UNINITIALIZED,            "protobuf message not initialized"                      },
    {JSON2PB_FIELD_NOT_FOUND,          "specified field not found in protobuf message"         },
    {JSON2PB_FIELD_IS_DEPRECATED,      "specified field already deprecated in protobuf message"},
    {JSON2PB_PB_GENERAL,               "general error in protobuf message"                     },
    {JSON2PB_INVALID_ARG,              "pass invalid argument to function"                     },
    {JSON2PB_INCORRECT_EXCEPTION_TYPE, "throw an incorrect exception type"                     },
    {JSON2PB_CODE_GENERAL,             "general error in coding"                               },
    {JSON2PB_OS_GENERAL,               "general error in operating system"                     },
};

static j2p_expt* __cvt_int32_t__(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const ProtobufCFieldDescriptor* restrict field_desc);

j2p_expt*
cvt_cjson_2_proto_c_field(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const char* restrict field_name)
{
#if 0
    assert(root != NULL);
    assert(msg != NULL);
    assert(item != NULL);
    assert(field_name != NULL);
#endif

    if (NULL == root || NULL == msg || NULL == item || NULL == field_name) {
        JSON2PB_THROW_EXCEPTION(JSON2PB_INVALID_ARG);
    }

    if (NULL == msg->descriptor) {
        JSON2PB_THROW_EXCEPTION(JSON2PB_UNINITIALIZED);
    }

    if (cJSON_IsNull(root)) {
        JSON2PB_THROW_EXCEPTION(JSON2PB_NULL_VALUE);
    }

    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->descriptor, field_name);
    if (NULL == field_desc) {
        JSON2PB_THROW_EXCEPTION(JSON2PB_FIELD_NOT_FOUND);
    }

    j2p_expt* rtn = NULL;

    switch (field_desc->type) {
    case PROTOBUF_C_TYPE_INT32:
    case PROTOBUF_C_TYPE_SINT32:
    case PROTOBUF_C_TYPE_SFIXED32:
#if 1
        rtn = __cvt_int32_t__(root, msg, item, field_desc);
#endif
        break;
    case PROTOBUF_C_TYPE_INT64:
    case PROTOBUF_C_TYPE_SINT64:
    case PROTOBUF_C_TYPE_SFIXED64:
#if 0
        rtn = __cvt_int64_t__(msg, field_desc, root);
#endif
        break;
    case PROTOBUF_C_TYPE_UINT32:
    case PROTOBUF_C_TYPE_FIXED32:
#if 0
        rtn = __cvt_uint32_t__(msg, field_desc, root);
#endif
        break;
    case PROTOBUF_C_TYPE_UINT64:
    case PROTOBUF_C_TYPE_FIXED64:
#if 0
        rtn = __cvt_uint64_t__(msg, field_desc, root);
#endif
        break;
    case PROTOBUF_C_TYPE_FLOAT:
#if 0
        rtn = __cvt_float__(msg, field_desc, root);
#endif
        break;
    case PROTOBUF_C_TYPE_DOUBLE:
#if 0
        rtn = __cvt_double__(msg, field_desc, root);
#endif
        break;
    case PROTOBUF_C_TYPE_STRING:
#if 0
        rtn = __cvt_string__(msg, field_desc, root);
#endif
        break;
    case PROTOBUF_C_TYPE_MESSAGE:
#if 0    
    rtn = __cvt_pb_msg__(msg, field_desc, root, msg_convertor);
#endif
        break;
    case PROTOBUF_C_TYPE_BOOL:
#if 0    
    rtn = __cvt_bool__(msg, field_desc, root, string_bool);
#endif
        break;
    case PROTOBUF_C_TYPE_BYTES:
#if 0    
    rtn = __cvt_bytes__(msg, field_desc, root, mode);
#endif
        break;
    case PROTOBUF_C_TYPE_ENUM:
#if 0    
    rtn = __cvt_enum__(msg, field_desc, root, string_enum);
#endif
        break;
    default:
        printf("field %s cannot processed in json for now\n", field_desc->name);
        break;
    }

    return rtn;
}

j2p_expt*
gen_json2pb_exception(const cJSON* root, const cJSON* item, const j2p_expt_t type)
{
    j2p_expt* e = (j2p_expt*)calloc(1, sizeof(j2p_expt));
    e->where    = cJSONUtils_FindPointerFromObjectTo(root, item);

    if (type <= JSON2PB_OS_GENERAL) {
        e->msg = &j2p_expt_msg_list[type];
    } else {
        e->msg = &j2p_expt_msg_list[JSON2PB_INCORRECT_EXCEPTION_TYPE];
    }
    return e;
}

void
free_json2pb_exception(j2p_expt* e)
{
    free(e->where);
    free(e);
}

static j2p_expt*
__cvt_int32_t__(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const ProtobufCFieldDescriptor* restrict field_desc)
{
    assert(msg != NULL && field_desc != NULL && item != NULL && msg->descriptor != NULL && (field_desc->type == PROTOBUF_C_TYPE_INT32 || field_desc->type == PROTOBUF_C_TYPE_SINT32 || field_desc->type == PROTOBUF_C_TYPE_SFIXED32));

    const bool is_repeated   = (field_desc->label == PROTOBUF_C_LABEL_REPEATED);
    const bool is_oneof      = (field_desc->flags == PROTOBUF_C_FIELD_FLAG_ONEOF);
    const bool is_deprecated = (field_desc->flags == PROTOBUF_C_FIELD_FLAG_DEPRECATED);

    if (is_deprecated) {
        JSON2PB_THROW_EXCEPTION(JSON2PB_FIELD_IS_DEPRECATED);
    }

    // if (is_repeated) {
    //     if (!cJSON_IsArray(item)) {
    //         return E_UNACCEPTABLE_JSON_TYPE;
    //     }
    //     if (cJSON_GetArraySize(item) > 0) {
    //         uint32_t       rtn_code = E_SUCCESS;
    //         uint64_t       index    = 0;
    //         const cJSON*   element  = NULL;
    //         const uint64_t length   = cJSON_GetArraySize(item);
    //         int32_t* const array    = (int32_t*)calloc(length, sizeof(int32_t));
    //         if (NULL == array) {
    //             return E_MEM_ALLOC;
    //         }

    //         cJSON_ArrayForEach(element, item)
    //         {
    //             rtn_code |= cvt_single_int32_t(&array[index], element);
    //             index++;
    //         }

    //         /* TODO: check if array is valid, if not, free it and retrun an error code here */

    //         *(int32_t**)((void*)msg + field_desc->offset)          = array;
    //         *(size_t*)((void*)msg + field_desc->quantifier_offset) = length;
    //         return rtn_code;
    //     } else {
    //         return E_EMPTY_ARRAY;
    //     }
    // } else {
    //     int32_t* field_ptr = (int32_t*)((void*)msg + field_desc->offset);
    //     return cvt_single_int32_t(field_ptr, item);
    // }
}