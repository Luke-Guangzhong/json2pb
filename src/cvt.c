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
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trower-base64/base64.h"

#include "internal.h"
#include "json2pb.h"

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
        printf("enum_val: %d, rv: %d\n", enum_val, rv);
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
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }
    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
cvt_single_int32_t_v2(const cJSON* const item, int32_t* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func)
{
    (void)field_desc;
    (void)add_cvt_func;
    return cvt_single_int32_t(item, field);
}

j2p_expt_t
cvt_single_enum_v2(const cJSON* const item, int* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func)
{
    return cvt_single_enum(item, (int*)field, add_cvt_func.enum_cvt, field_desc->descriptor);
}

static j2p_expt_t
util_cvt_hex_to_bytes(const char* const hex_str, uint8_t** const bytes, size_t* const bytes_len)
{

    assert(hex_str != NULL && bytes != NULL && bytes_len != NULL);

    char* buffer = strdup(hex_str);
    if (NULL == buffer) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    size_t hex_len = strlen(buffer);

    if (hex_len < 2) {
        free(buffer);
        return J2P_EXPT_INVALID_HEX_STRING;
    }

    size_t   b_len   = (hex_len % 3 == 0) ? (hex_len / 3) : (hex_len / 3 + 1);
    uint8_t* b_data  = (uint8_t*)calloc(b_len, sizeof(uint8_t));
    size_t   b_index = 0;

    char* token  = strtok(buffer, " ");
    char* endptr = NULL;
    while (token != NULL) {
        errno              = 0;
        unsigned long byte = strtoul(token, &endptr, 16);
        if (errno != 0 || *endptr != '\0' || byte > UINT8_MAX) {
            break;
        }
        b_data[b_index++] = byte;
        token             = strtok(NULL, " ");
    }

    if (b_index != b_len) {
        free(b_data);
        free(buffer);
        return J2P_EXPT_INVALID_HEX_STRING;
    }

    free(buffer);

    *bytes     = b_data;
    *bytes_len = b_len;

    return J2P_EXPT_SUCCESS;
}

static j2p_expt_t
util_cvt_base64_to_bytes(const char* const base64_str, uint8_t** const bytes, size_t* const bytes_len)
{
    assert(base64_str != NULL && bytes != NULL && bytes_len != NULL);

    size_t input_len = strlen(base64_str);
    *bytes           = b64_decode_with_alloc(base64_str, input_len, bytes_len);
    if (NULL == *bytes) {
        return J2P_EXPT_INVALID_BASE64_STRING;
    }

    return J2P_EXPT_SUCCESS;
}

static j2p_expt_t
util_cvt_file_to_bytes(const char* const file_path, uint8_t** const bytes, size_t* const bytes_len)
{
    assert(file_path != NULL && bytes != NULL && bytes_len != NULL);

    FILE* fp = fopen(file_path, "rb");
    if (NULL == fp) {
        return J2P_EXPT_INVALID_FILE_PATH;
    }

    fseek(fp, 0, SEEK_END);
    const size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t* b_data = (uint8_t*)calloc(size, sizeof(uint8_t));
    if (NULL == b_data) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    if (fread(b_data, sizeof(uint8_t), size, fp) != size) {
        if (ferror(fp) != 0) {
            perror("fread failed");
        }
        free(b_data);
        fclose(fp);
        return J2P_EXPT_INVALID_FILE_PATH;
    } else {
        *bytes     = b_data;
        *bytes_len = size;
        fclose(fp);
        return J2P_EXPT_SUCCESS;
    }
}

j2p_expt_t
cvt_single_bytes(const cJSON* const item, ProtobufCBinaryData* const field, j2p_file_t mode)
{
    assert(NULL != item);
    assert(NULL != field);

    j2p_expt_t rtn = J2P_EXPT_SUCCESS;

    ProtobufCBinaryData* b_data = (ProtobufCBinaryData*)calloc(1, sizeof(ProtobufCBinaryData));

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
        field->data = b_data->data;
        field->len  = b_data->len;
    } else {
        free(b_data->data);
        free(b_data);
    }

    return rtn;
}

j2p_expt_t
cvt_single_message(const cJSON* const root, const cJSON* const item, void** const field, obj_msg_convertor obj_cvt, const ProtobufCMessageDescriptor* const msg_desc)
{
    assert(NULL != root);
    assert(NULL != item);
    assert(NULL != field);
    assert(NULL != msg_desc);

    if (NULL == obj_cvt) {
        obj_cvt = default_obj_msg_convertor;
    }

    ProtobufCMessage* msg_buffer = (ProtobufCMessage*)calloc(1, msg_desc->sizeof_message);
    if (NULL == msg_buffer) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    msg_desc->message_init(msg_buffer);

    j2p_expt_t rtn = obj_cvt(root, item, msg_buffer);

    if (rtn == J2P_EXPT_SUCCESS) {
        *field = msg_buffer;
    } else {
        protobuf_c_message_free_unpacked(msg_buffer, NULL);
    }

    return rtn;
}