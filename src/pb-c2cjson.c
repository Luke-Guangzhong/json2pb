/**
 * @file pb2cjson.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2024-11-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "pb-c2cjson.h"
#include "trower-base64/base64.h"

cJSON*
gen_cjson_via_pb_field(const ProtobufCMessage* pb_msg, const char* field_name, const bool show_null_value, const bool enum_string, ByteMode byte_mode)
{
    if (NULL == pb_msg || NULL == pb_msg->descriptor || NULL == field_name) {
        return NULL;
    }

    if (BYTE_MODE_FILE_PATH == byte_mode) { /* File Path Mode is not support when pb2cjson */
        byte_mode = BYTE_MODE_BASE64;
    }

    const ProtobufCMessageDescriptor* msg_desc   = pb_msg->descriptor;
    const ProtobufCFieldDescriptor*   field_desc = protobuf_c_message_descriptor_get_field_by_name(msg_desc, field_name);
    if (NULL == field_desc) {
        return NULL;
    }

    switch (field_desc->type) {
    case PROTOBUF_C_TYPE_INT32:
    case PROTOBUF_C_TYPE_SINT32:
    case PROTOBUF_C_TYPE_SFIXED32:
    case PROTOBUF_C_TYPE_INT64:
    case PROTOBUF_C_TYPE_SINT64:
    case PROTOBUF_C_TYPE_SFIXED64:
    case PROTOBUF_C_TYPE_UINT32:
    case PROTOBUF_C_TYPE_FIXED32:
    case PROTOBUF_C_TYPE_UINT64:
    case PROTOBUF_C_TYPE_FIXED64:
    case PROTOBUF_C_TYPE_FLOAT:
    case PROTOBUF_C_TYPE_DOUBLE:
        return gen_cjson_via_pb_number(pb_msg, field_desc, show_null_value);
    case PROTOBUF_C_TYPE_BOOL:
        return gen_cjson_via_pb_bool(pb_msg, field_desc, show_null_value);
    case PROTOBUF_C_TYPE_ENUM:
        return gen_cjson_via_pb_enum(pb_msg, field_desc, show_null_value, enum_string);
    case PROTOBUF_C_TYPE_STRING:
        return gen_cjson_via_pb_string(pb_msg, field_desc, show_null_value);
    case PROTOBUF_C_TYPE_BYTES:
        return gen_cjson_via_pb_bytes(pb_msg, field_desc, show_null_value, byte_mode);
    case PROTOBUF_C_TYPE_MESSAGE:
        return gen_cjson_via_pb_msg(pb_msg, field_desc, show_null_value, enum_string, byte_mode);
    default:
        break;
    }

    return NULL;
}

cJSON*
gen_cjson_via_pb_number(const ProtobufCMessage* const restrict pb_msg, const ProtobufCFieldDescriptor* const restrict field_desc, const bool show_null_value)
{
    if (NULL == pb_msg || NULL == field_desc) {
        return NULL;
    }

    cJSON* root = NULL;
    if (field_desc->label == PROTOBUF_C_LABEL_REPEATED) {
        unsigned number_of_elements = *(unsigned*)((void*)pb_msg + field_desc->quantifier_offset);
        switch (field_desc->type) {
        case PROTOBUF_C_TYPE_INT32:
        case PROTOBUF_C_TYPE_SINT32:
        case PROTOBUF_C_TYPE_SFIXED32: {
            const int32_t* array_ptr = (*(int32_t**)((void*)pb_msg + field_desc->offset));

            if (NULL == array_ptr) {
                if (show_null_value) {
                    root = cJSON_CreateNull();
                }
            } else {
                root = cJSON_CreateArray();
                for (unsigned i = 0; i < number_of_elements; i++) {
                    cJSON_AddItemToArray(root, cJSON_CreateNumber((int32_t)array_ptr[i]));
                }
            }
            break;
        }
        case PROTOBUF_C_TYPE_INT64:
        case PROTOBUF_C_TYPE_SINT64:
        case PROTOBUF_C_TYPE_SFIXED64: {
            const int64_t* array_ptr = (*(int64_t**)((void*)pb_msg + field_desc->offset));

            if (NULL == array_ptr) {
                if (show_null_value) {
                    root = cJSON_CreateNull();
                }
            } else {
                root = cJSON_CreateArray();
                for (unsigned i = 0; i < number_of_elements; i++) {
                    cJSON_AddItemToArray(root, cJSON_CreateNumber((int64_t)array_ptr[i]));
                }
            }
            break;
        }
        case PROTOBUF_C_TYPE_UINT32:
        case PROTOBUF_C_TYPE_FIXED32: {
            const uint32_t* array_ptr = (*(uint32_t**)((void*)pb_msg + field_desc->offset));

            if (NULL == array_ptr) {
                if (show_null_value) {
                    root = cJSON_CreateNull();
                }
            } else {
                root = cJSON_CreateArray();
                for (unsigned i = 0; i < number_of_elements; i++) {
                    cJSON_AddItemToArray(root, cJSON_CreateNumber((uint32_t)array_ptr[i]));
                }
            }
            break;
        }
        case PROTOBUF_C_TYPE_UINT64:
        case PROTOBUF_C_TYPE_FIXED64: {
            const uint64_t* array_ptr = (*(uint64_t**)((void*)pb_msg + field_desc->offset));

            if (NULL == array_ptr) {
                if (show_null_value) {
                    root = cJSON_CreateNull();
                }
            } else {
                root = cJSON_CreateArray();
                for (unsigned i = 0; i < number_of_elements; i++) {
                    cJSON_AddItemToArray(root, cJSON_CreateNumber((uint64_t)array_ptr[i]));
                }
            }
            break;
        }
        case PROTOBUF_C_TYPE_FLOAT: {
            const float* array_ptr = (*(float**)((void*)pb_msg + field_desc->offset));

            if (NULL == array_ptr) {
                if (show_null_value) {
                    root = cJSON_CreateNull();
                }
            } else {
                root = cJSON_CreateArray();
                for (unsigned i = 0; i < number_of_elements; i++) {
                    cJSON_AddItemToArray(root, cJSON_CreateNumber((float)array_ptr[i]));
                }
            }
            break;
        }
        case PROTOBUF_C_TYPE_DOUBLE: {
            const double* array_ptr = (*(double**)((void*)pb_msg + field_desc->offset));

            if (NULL == array_ptr) {
                if (show_null_value) {
                    root = cJSON_CreateNull();
                }
            } else {
                root = cJSON_CreateArray();
                for (unsigned i = 0; i < number_of_elements; i++) {
                    cJSON_AddItemToArray(root, cJSON_CreateNumber((double)array_ptr[i]));
                }
            }
            break;
        }
        default:
            break;
        }

    } else {
        switch (field_desc->type) {
        case PROTOBUF_C_TYPE_INT32:
        case PROTOBUF_C_TYPE_SINT32:
        case PROTOBUF_C_TYPE_SFIXED32:
            root = cJSON_CreateNumber(*(int32_t*)((void*)pb_msg + field_desc->offset));
            break;
        case PROTOBUF_C_TYPE_INT64:
        case PROTOBUF_C_TYPE_SINT64:
        case PROTOBUF_C_TYPE_SFIXED64:
            root = cJSON_CreateNumber(*(int64_t*)((void*)pb_msg + field_desc->offset));
            break;
        case PROTOBUF_C_TYPE_UINT32:
        case PROTOBUF_C_TYPE_FIXED32:
            root = cJSON_CreateNumber(*(uint32_t*)((void*)pb_msg + field_desc->offset));
            break;
        case PROTOBUF_C_TYPE_UINT64:
        case PROTOBUF_C_TYPE_FIXED64:
            root = cJSON_CreateNumber(*(uint64_t*)((void*)pb_msg + field_desc->offset));
            break;
        case PROTOBUF_C_TYPE_FLOAT:
            root = cJSON_CreateNumber(*(float*)((void*)pb_msg + field_desc->offset));
            break;
        case PROTOBUF_C_TYPE_DOUBLE:
            root = cJSON_CreateNumber(*(double*)((void*)pb_msg + field_desc->offset));
            break;
        default:
            cJSON_Delete(root);
            root = NULL;
            break;
        }
    }

    return root;
}

cJSON*
gen_cjson_via_pb_bool(const ProtobufCMessage* const restrict pb_msg, const ProtobufCFieldDescriptor* const restrict field_desc, const bool show_null_value)
{
    if (NULL == pb_msg || NULL == field_desc || field_desc->type != PROTOBUF_C_TYPE_BOOL) {
        return NULL;
    }

    cJSON* root = NULL;
    if (field_desc->label == PROTOBUF_C_LABEL_REPEATED) {
        unsigned                  number_of_elements = *(unsigned*)((void*)pb_msg + field_desc->quantifier_offset);
        const protobuf_c_boolean* array_ptr          = (*(protobuf_c_boolean**)((void*)pb_msg + field_desc->offset));

        if (NULL == array_ptr) {
            if (show_null_value) {
                root = cJSON_CreateNull();
            }
        } else {
            root = cJSON_CreateArray();
            for (unsigned i = 0; i < number_of_elements; i++) {
                cJSON_AddItemToArray(root, (bool)array_ptr[i] ? cJSON_CreateTrue() : cJSON_CreateFalse());
            }
        }
    } else {
        root = (*(protobuf_c_boolean*)((void*)pb_msg + field_desc->offset)) ? cJSON_CreateTrue() : cJSON_CreateFalse();
    }

    return root;
}

cJSON*
gen_cjson_via_pb_enum(const ProtobufCMessage* const restrict pb_msg, const ProtobufCFieldDescriptor* const restrict field_desc, const bool show_null_value, const bool enum_string)
{
    if (NULL == pb_msg || NULL == field_desc || field_desc->type != PROTOBUF_C_TYPE_ENUM) {
        return NULL;
    }

    cJSON*                               root      = NULL;
    const ProtobufCEnumDescriptor* const enum_desc = field_desc->descriptor;
    if (field_desc->label == PROTOBUF_C_LABEL_REPEATED) {
        unsigned       number_of_elements = *(unsigned*)((void*)pb_msg + field_desc->quantifier_offset);
        const int32_t* array_ptr          = (*(int32_t**)((void*)pb_msg + field_desc->offset));

        if (NULL == array_ptr) {
            if (show_null_value) {
                root = cJSON_CreateNull();
            }
        } else {
            root = cJSON_CreateArray();
            for (unsigned i = 0; i < number_of_elements; i++) {
                cJSON_AddItemToArray(root, gen_cjson_via_pb_enum_single((int32_t)array_ptr[i], enum_desc, enum_string));
            }
        }
    } else {
        root = gen_cjson_via_pb_enum_single(*(int32_t*)((void*)pb_msg + field_desc->offset), enum_desc, enum_string);
    }

    return root;
}

cJSON*
gen_cjson_via_pb_string(const ProtobufCMessage* const restrict pb_msg, const ProtobufCFieldDescriptor* const restrict field_desc, const bool show_null_value)
{
    if (NULL == pb_msg || NULL == field_desc || field_desc->type != PROTOBUF_C_TYPE_STRING) {
        return NULL;
    }

    cJSON* root = NULL;
    if (field_desc->label == PROTOBUF_C_LABEL_REPEATED) {
        unsigned     number_of_elements = *(unsigned*)((void*)pb_msg + field_desc->quantifier_offset);
        const char** array_ptr          = (*(const char***)((void*)pb_msg + field_desc->offset));

        if (NULL == array_ptr) {
            if (show_null_value) {
                root = cJSON_CreateNull();
            }
        } else {
            root = cJSON_CreateStringArray(array_ptr, number_of_elements);
        }
    } else {
        root = cJSON_CreateString(*(char**)((void*)pb_msg + field_desc->offset));
    }

    return root;
}

cJSON*
gen_cjson_via_pb_bytes(const ProtobufCMessage* const restrict pb_msg, const ProtobufCFieldDescriptor* const restrict field_desc, const bool show_null_value, const ByteMode byte_mode)
{
    if (NULL == pb_msg || NULL == field_desc || field_desc->type != PROTOBUF_C_TYPE_BYTES || NULL == pb_msg->descriptor) {
        return NULL;
    }

    cJSON*                            root     = NULL;
    const ProtobufCMessageDescriptor* msg_desc = pb_msg->descriptor;
    if (field_desc->label == PROTOBUF_C_LABEL_REPEATED) {
        unsigned                   number_of_elements = *(unsigned*)((void*)pb_msg + field_desc->quantifier_offset);
        const ProtobufCBinaryData* array_ptr          = (*(const ProtobufCBinaryData**)((void*)pb_msg + field_desc->offset));

        if (NULL == array_ptr) {
            if (show_null_value) {
                root = cJSON_CreateNull();
            }
        } else {
            root = cJSON_CreateArray();
            for (size_t i = 0; i < number_of_elements; i++) {
                cJSON_AddItemToArray(root, gen_cjson_via_pb_bytes_single(array_ptr[i], field_desc, show_null_value, byte_mode));
            }
        }
    } else {
        root = gen_cjson_via_pb_bytes_single(*(const ProtobufCBinaryData*)((void*)pb_msg + field_desc->offset), field_desc, show_null_value, byte_mode);
    }

    return root;
}

cJSON*
gen_cjson_via_pb_msg(const ProtobufCMessage* const restrict pb_msg, const ProtobufCFieldDescriptor* const restrict field_desc, const bool show_null_value, const bool enum_string, const ByteMode byte_mode)
{
    if (NULL == pb_msg || NULL == field_desc || field_desc->type != PROTOBUF_C_TYPE_MESSAGE) {
        return NULL;
    }

    cJSON*                                  root     = NULL;
    const ProtobufCMessageDescriptor* const msg_desc = field_desc->descriptor;

    if (field_desc->label == PROTOBUF_C_LABEL_REPEATED) {
        unsigned                 number_of_elements = *(unsigned*)((void*)pb_msg + field_desc->quantifier_offset);
        const ProtobufCMessage** array_ptr          = (*(const ProtobufCMessage***)((void*)pb_msg + field_desc->offset));

        if (array_ptr == NULL) {
            if (show_null_value) {
                root = cJSON_CreateNull();
            }
        } else {
            root = cJSON_CreateArray();
            for (unsigned i = 0; i < number_of_elements; i++) {
                const ProtobufCMessage* sub_msg = array_ptr[i];
                if (NULL == sub_msg) {
                    if (show_null_value) {
                        cJSON_AddItemToArray(root, cJSON_CreateNull());
                    }
                } else {
                    cJSON_AddItemToArray(root, gen_cjson_via_pb_msg_single(sub_msg, show_null_value, enum_string, byte_mode));
                }
            }
        }
    } else {
        const ProtobufCMessage* sub_msg = (*(ProtobufCMessage**)((void*)pb_msg + field_desc->offset));
        if (NULL == sub_msg) {
            if (show_null_value) {
                root = cJSON_CreateNull();
            }
        } else {
            root = gen_cjson_via_pb_msg_single(sub_msg, show_null_value, enum_string, byte_mode);
        }
    }

    return root;
}

cJSON*
gen_cjson_via_pb_msg_single(const ProtobufCMessage* const restrict pb_msg, const bool show_null_value, const bool enum_string, const ByteMode byte_mode)
{
    if (NULL == pb_msg || pb_msg->descriptor == NULL) {
        return NULL;
    }

    const ProtobufCMessageDescriptor* const restrict msg_desc = pb_msg->descriptor;
    const unsigned n_fields                                   = pb_msg->descriptor->n_fields;

    cJSON* root = cJSON_CreateObject();

    for (size_t i = 0; i < n_fields; i++) {
        cJSON_AddItemToObject(root, (msg_desc->fields + i)->name, gen_cjson_via_pb_field(pb_msg, (msg_desc->fields + i)->name, show_null_value, enum_string, byte_mode));
    }

    return root;
}

cJSON*
gen_cjson_via_pb_enum_single(const int32_t enum_value, const ProtobufCEnumDescriptor* const restrict enum_desc, const bool enum_string)
{
    if (NULL == enum_desc) {
        return NULL;
    }

    if (!enum_string) {
        return cJSON_CreateNumber(enum_value);
    }

    const ProtobufCEnumValue* enum_value_desc = protobuf_c_enum_descriptor_get_value(enum_desc, enum_value);
    if (NULL == enum_value_desc) {
        return cJSON_CreateNumber(enum_value);
    } else {
        return cJSON_CreateString(enum_value_desc->name);
    }
}

cJSON*
gen_cjson_via_pb_bytes_single(const ProtobufCBinaryData bin_data, const ProtobufCFieldDescriptor* field_desc, const bool show_null_value, const ByteMode byte_mode)
{
    if (NULL == bin_data.data) {
        if (show_null_value) {
            return cJSON_CreateNull();
        } else {
            return NULL;
        }
    }

    cJSON* root = NULL;
    switch (byte_mode) {
    case BYTE_MODE_BASE64: {
        size_t out_len         = 0;
        char*  b64_encode_buff = b64_encode_with_alloc(bin_data.data, bin_data.len, &out_len);
        if (NULL == b64_encode_buff) {
            if (show_null_value) {
                root = cJSON_CreateNull();
            }
        } else {
            root = cJSON_CreateString(b64_encode_buff);
            free(b64_encode_buff);
            b64_encode_buff = NULL;
        }

        break;
    }
    case BYTE_MODE_FILE_PATH: {
        break;
    }
    case BYTE_MODE_HEX: {
        size_t out_len         = bin_data.len * 3;
        char*  hex_encode_buff = (char*)calloc(out_len + 1, sizeof(char));
        char*  p               = hex_encode_buff;

        for (size_t i = 0; i < bin_data.len; i++) {
            snprintf(p, 4, "%02x ", bin_data.data[i]);
            p += 3;
        }
        hex_encode_buff[out_len - 1] = '\0';

        root = cJSON_CreateString(hex_encode_buff);
        free(hex_encode_buff);
        hex_encode_buff = NULL;

        break;
    }

    default:
        break;
    }
    return root;
}