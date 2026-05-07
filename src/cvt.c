/**
 * @file cvt.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "internal.h"
#include "json2pb.h"
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const double MAX_EXACT = 0x1.0p53;
const double MIN_EXACT = -0x1.0p53;

j2p_expt_t
cvt_single_int32_t(const cJSON* const item, int32_t* const field)
{
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (isinf(num_value) || isnan(num_value) || num_value < INT32_MIN || num_value > INT32_MAX) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        *field = (int32_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                = 0;
        char*      endptr    = NULL;
        const long num_value = strtol(cJSON_GetStringValue(item), &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            if (errno == ERANGE) {
                return J2P_EXPT_VALUE_OVERFLOW;
            } else {
                return J2P_EXPT_INVALID_NUMBER_STRING;
            }
        }
        if (num_value < INT32_MIN || num_value > INT32_MAX) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        *field = (int32_t)num_value;
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }

    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
cvt_single_int64_t(const cJSON* const item, int64_t* const field)
{
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);

        if (isinf(num_value) || isnan(num_value)) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        if (num_value < MIN_EXACT || num_value > MAX_EXACT) {
            return J2P_EXPT_NOT_EXACT;
        }
        *field = (int64_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                     = 0;
        char*           endptr    = NULL;
        const long long num_value = strtoll(cJSON_GetStringValue(item), &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            if (errno == ERANGE) {
                return J2P_EXPT_VALUE_OVERFLOW;
            } else {
                return J2P_EXPT_INVALID_NUMBER_STRING;
            }
        }
        if (num_value < INT64_MIN || num_value > INT64_MAX) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        *field = (int64_t)num_value;
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }

    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
cvt_single_uint32_t(const cJSON* const item, uint32_t* const field)
{
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);

        if (isinf(num_value) || isnan(num_value) || num_value < 0 || num_value > UINT32_MAX) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        *field = (uint32_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        const char* num_str = cJSON_GetStringValue(item);
        if (num_str[0] == '-') {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        errno                         = 0;
        char*               endptr    = NULL;
        const unsigned long num_value = strtoul(num_str, &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            if (errno == ERANGE) {
                return J2P_EXPT_VALUE_OVERFLOW;
            } else {
                return J2P_EXPT_INVALID_NUMBER_STRING;
            }
        }
        if (num_value < 0 || num_value > UINT32_MAX) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        *field = (uint32_t)num_value;
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }

    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
cvt_single_uint64_t(const cJSON* const item, uint64_t* const field)
{
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);

        if (isinf(num_value) || isnan(num_value)) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        if (num_value < 0 || num_value > MAX_EXACT) {
            return J2P_EXPT_NOT_EXACT;
        }
        *field = (uint64_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        const char* num_str = cJSON_GetStringValue(item);
        if (num_str[0] == '-') {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        errno                              = 0;
        char*                    endptr    = NULL;
        const unsigned long long num_value = strtoull(num_str, &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            if (errno == ERANGE) {
                return J2P_EXPT_VALUE_OVERFLOW;
            } else {
                return J2P_EXPT_INVALID_NUMBER_STRING;
            }
        }
        if (num_value < 0 || num_value > UINT64_MAX) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        *field = (uint64_t)num_value;
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }

    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
cvt_single_float(const cJSON* const item, float* const field)
{
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        double num_value = cJSON_GetNumberValue(item);

        if (isnan(num_value) || isinf(num_value)) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }

        double abs_val = fabs(num_value);
        if (abs_val != 0.0 && (abs_val < FLT_TRUE_MIN || abs_val > FLT_MAX)) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }

        float  fval = (float)num_value;
        double back = (double)fval;
        if (back != num_value) {
            return J2P_EXPT_NOT_EXACT;
        }

        *field = fval;
    } else if (NULL != cJSON_GetStringValue(item)) {
        const char* str_val = cJSON_GetStringValue(item);
        errno               = 0;
        char* endptr        = NULL;
        float fval          = strtof(str_val, &endptr);

        if (errno == ERANGE) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }

        if (endptr == str_val || *endptr != '\0') {
            return J2P_EXPT_INVALID_NUMBER_STRING;
        }

        if (isnan(fval) || isinf(fval)) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }

        *field = fval;
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }

    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
cvt_single_double(const cJSON* const item, double* const field)
{
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        double num_value = cJSON_GetNumberValue(item);

        if (isnan(num_value) || isinf(num_value)) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }

        *field = num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        const char* str_val = cJSON_GetStringValue(item);
        errno               = 0;
        char*  endptr       = NULL;
        double num_value    = strtod(str_val, &endptr);

        if (errno == ERANGE) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }

        if (endptr == str_val || *endptr != '\0') {
            return J2P_EXPT_INVALID_NUMBER_STRING;
        }

        if (isnan(num_value) || isinf(num_value)) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }

        *field = num_value;
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }

    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
cvt_single_bool(const cJSON* const item, bool* const field, string_bool_convertor str_bool_cvt)
{
    assert(NULL != item);
    assert(NULL != field);

    if (NULL == str_bool_cvt) {
        str_bool_cvt = default_string_bool_convertor;
    }

    if (cJSON_IsBool(item)) {
        *field = cJSON_IsTrue(item);
    } else if (cJSON_IsNumber(item)) {
        *field = cJSON_GetNumberValue(item) != 0.0;
    } else if (NULL != cJSON_GetStringValue(item)) {
        const char* str_val = cJSON_GetStringValue(item);
        *field              = str_bool_cvt(str_val);
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }
    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
cvt_single_enum(const cJSON* const item, int* const field, string_enum_convertor str_enum_cvt, const ProtobufCEnumDescriptor* const enum_desc)
{
    assert(NULL != item);
    assert(NULL != field);

    if (NULL == str_enum_cvt) {
        str_enum_cvt = default_string_enum_convertor;
    }

    if (cJSON_IsNumber(item)) {
        int enum_val = (int)cJSON_GetNumberValue(item);
        int rv       = int_range_lookup(enum_desc->n_value_ranges, enum_desc->value_ranges, enum_val);
        if (rv < 0) {
            return J2P_EXPT_INVALID_ENUM_VALUE;
        }

        *field = rv;
    } else if (NULL != cJSON_GetStringValue(item)) {
        const char* enum_str = cJSON_GetStringValue(item);

        int enum_val = str_enum_cvt(enum_desc, enum_str);
        if (enum_val < 0) {
            return J2P_EXPT_INVALID_ENUM_VALUE;
        }

        *field = enum_val;
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }
    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
cvt_single_string(const cJSON* const item, char** const field)
{
    assert(NULL != item);
    assert(NULL != field);

    if (NULL != cJSON_GetStringValue(item)) {
        if (asprintf(field, "%s", cJSON_GetStringValue(item)) < 0) {
            return J2P_EXPT_MEM_ALLOC_FAILED;
        }
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }
    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
cvt_single_bytes(const cJSON* const item, ProtobufCBinaryData* const field, j2p_file_t mode)
{
    assert(NULL != item);
    assert(NULL != field);

    j2p_expt_t rtn = J2P_EXPT_SUCCESS;

    ProtobufCBinaryData* b_data = (ProtobufCBinaryData*)calloc(1, sizeof(ProtobufCBinaryData));
    if (NULL == b_data) {
        return J2P_EXPT_MEM_ALLOC_FAILED;
    }

    if (NULL != cJSON_GetStringValue(item)) {
        switch (mode) {
        case J2P_FILE_PATH_STR:
            rtn = util_cvt_file_to_bytes(cJSON_GetStringValue(item), &b_data->data, &b_data->len);
            break;
        case J2P_FILE_BASE64_STR:
            rtn = util_cvt_base64_to_bytes(cJSON_GetStringValue(item), &b_data->data, &b_data->len);
            break;
        case J2P_FILE_HEX_STR:
            rtn = util_cvt_hex_to_bytes(cJSON_GetStringValue(item), &b_data->data, &b_data->len);
            break;
        default:
            rtn = J2P_EXPT_INVALID_FILE_MODE;
            break;
        }
    } else {
        rtn = J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }

    if (rtn == J2P_EXPT_SUCCESS) {
        field->data  = b_data->data;
        b_data->data = NULL;
        field->len   = b_data->len;
        free(b_data);
    } else {
        free(b_data->data);
        free(b_data);
    }

    return rtn;
}

j2p_expt_t
cvt_single_message(const cJSON* const root, const cJSON* const item, ProtobufCMessage** const field, obj_msg_convertor obj_cvt, const ProtobufCMessageDescriptor* const msg_desc)
{
    assert(NULL != root);
    assert(NULL != item);
    assert(NULL != field);
    assert(NULL != msg_desc);

    if (NULL == obj_cvt) {
        obj_cvt = default_obj_msg_convertor;
    }

    if (!cJSON_IsObject(item)) {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }

    ProtobufCMessage* msg_buffer = (ProtobufCMessage*)calloc(1, msg_desc->sizeof_message);
    if (NULL == msg_buffer) {
        printf("Memory allocation failed\n");
        return J2P_EXPT_MEM_ALLOC_FAILED;
    }

    msg_desc->message_init(msg_buffer);

    j2p_expt_t rtn = obj_cvt(root, item, msg_buffer);

    *field = msg_buffer;

    return rtn;
}

j2p_expt_t
cvt_single_int32_t_v2(const cJSON* const root, const cJSON* const item, int32_t* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func)
{
    (void)field_desc;
    (void)add_cvt_func;
    (void)root;
    return cvt_single_int32_t(item, field);
}

j2p_expt_t
cvt_single_int64_t_v2(const cJSON* const root, const cJSON* const item, int64_t* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func)
{
    (void)field_desc;
    (void)add_cvt_func;
    (void)root;
    return cvt_single_int64_t(item, field);
}

j2p_expt_t
cvt_single_uint32_t_v2(const cJSON* const root, const cJSON* const item, uint32_t* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func)
{
    (void)field_desc;
    (void)add_cvt_func;
    (void)root;
    return cvt_single_uint32_t(item, field);
}

j2p_expt_t
cvt_single_uint64_t_v2(const cJSON* const root, const cJSON* const item, uint64_t* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func)
{
    (void)field_desc;
    (void)add_cvt_func;
    (void)root;
    return cvt_single_uint64_t(item, field);
}

j2p_expt_t
cvt_single_float_v2(const cJSON* const root, const cJSON* const item, float* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func)
{
    (void)field_desc;
    (void)add_cvt_func;
    (void)root;
    return cvt_single_float(item, field);
}

j2p_expt_t
cvt_single_double_v2(const cJSON* const root, const cJSON* const item, double* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func)
{
    (void)field_desc;
    (void)add_cvt_func;
    (void)root;
    return cvt_single_double(item, field);
}

j2p_expt_t
cvt_single_bool_v2(const cJSON* const root, const cJSON* const item, bool* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func)
{
    (void)field_desc;
    (void)root;
    return cvt_single_bool(item, field, add_cvt_func.bool_cvt);
}

j2p_expt_t
cvt_single_enum_v2(const cJSON* const root, const cJSON* const item, int* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func)
{
    (void)root;
    return cvt_single_enum(item, (int*)field, add_cvt_func.enum_cvt, field_desc->descriptor);
}

j2p_expt_t
cvt_single_string_v2(const cJSON* const root, const cJSON* const item, char** const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func)
{
    (void)field_desc;
    (void)add_cvt_func;
    (void)root;
    return cvt_single_string(item, field);
}

j2p_expt_t
cvt_single_bytes_v2(const cJSON* const                    root,
                    const cJSON* const                    item,
                    ProtobufCBinaryData* const            field,
                    const ProtobufCFieldDescriptor* const field_desc,
                    const j2p_add_cvt                     add_cvt_func)
{
    (void)field_desc;
    (void)root;
    return cvt_single_bytes(item, field, add_cvt_func.file_type);
}

j2p_expt_t
cvt_single_message_v2(const cJSON* const                    root,
                      const cJSON* const                    item,
                      ProtobufCMessage** const              field,
                      const ProtobufCFieldDescriptor* const field_desc,
                      const j2p_add_cvt                     add_cvt_func)
{

    return cvt_single_message(root, item, field, add_cvt_func.msg_cvt, field_desc->descriptor);
}