/**
 * @file single_value_conversion.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @date 2024-07-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson2pb-c.h"
#include "trower-base64/base64.h"

static RtnCode util_cvt_hex_to_bytes(const char* const hex_str, uint8_t** const bytes, size_t* const bytes_len);
static RtnCode util_cvt_base64_to_bytes(const char* const base64_str, uint8_t** const bytes, size_t* const bytes_len);
static RtnCode util_cvt_file_to_bytes(const char* const file_path, uint8_t** const bytes, size_t* const bytes_len);

RtnCode
cvt_single_int32_t(int32_t* const field, const cJSON* const item)
{
    if (NULL == field || NULL == item) {
        return E_INVALID_ARG;
    }

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (num_value < INT32_MIN || num_value > INT32_MAX) {
            return E_TYPE_OVERFLOW;
        }
        *field = (int32_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                = 0;
        char*      endptr    = NULL;
        const long num_value = strtol(cJSON_GetStringValue(item), &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            return E_INVALID_NUMBER_STRING;
        } else if (num_value > INT32_MAX || num_value < INT32_MIN) {
            return E_TYPE_OVERFLOW;
        }
        *field = (int32_t)num_value;
    } else {
        return E_UNACCEPTABLE_JSON_TYPE;
    }

    return E_SUCCESS;
}

RtnCode
cvt_single_int64_t(int64_t* const field, const cJSON* const item)
{
    if (NULL == field || NULL == item) {
        return E_INVALID_ARG;
    }

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (num_value < INT64_MIN || num_value > INT64_MAX) {
            return E_TYPE_OVERFLOW;
        }
        *field = (int64_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                     = 0;
        char*           endptr    = NULL;
        const long long num_value = strtoll(cJSON_GetStringValue(item), &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            return E_INVALID_NUMBER_STRING;
        } else if (num_value > INT64_MAX || num_value < INT64_MIN) {
            return E_TYPE_OVERFLOW;
        }
        *field = (int64_t)num_value;
    } else {
        return E_UNACCEPTABLE_JSON_TYPE;
    }

    return E_SUCCESS;
}

RtnCode
cvt_single_uint32_t(uint32_t* field, const cJSON* item)
{
    if (NULL == field || NULL == item) {
        return E_INVALID_ARG;
    }

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (num_value < 0 || num_value > UINT32_MAX) {
            return E_TYPE_OVERFLOW;
        }
        *field = (uint32_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                         = 0;
        char*               endptr    = NULL;
        const unsigned long num_value = strtoul(cJSON_GetStringValue(item), &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            return E_INVALID_NUMBER_STRING;
        } else if (num_value < 0 || num_value > UINT32_MAX) {
            return E_TYPE_OVERFLOW;
        }
        *field = (uint32_t)num_value;
    } else {
        return E_UNACCEPTABLE_JSON_TYPE;
    }

    return E_SUCCESS;
}

RtnCode
cvt_single_uint64_t(uint64_t* field, const cJSON* item)
{
    if (NULL == field || NULL == item) {
        return E_INVALID_ARG;
    }

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (num_value < 0 || num_value > UINT64_MAX) {
            return E_TYPE_OVERFLOW;
        }
        *field = (uint64_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                              = 0;
        char*                    endptr    = NULL;
        const unsigned long long num_value = strtoull(cJSON_GetStringValue(item), &endptr, 10);
        if (errno != 0 || *endptr != '\0') {
            return E_INVALID_NUMBER_STRING;
        } else if (num_value < 0 || num_value > UINT64_MAX) {
            return E_TYPE_OVERFLOW;
        }
        *field = (uint64_t)num_value;
    } else {
        return E_UNACCEPTABLE_JSON_TYPE;
    }

    return E_SUCCESS;
}

RtnCode
cvt_single_float(float* field, const cJSON* item)
{
    assert(field != NULL && item != NULL);

    if (cJSON_IsNumber(item)) {
        *field = (float)item->valuedouble;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                 = 0;
        char*       endptr    = NULL;
        const float num_value = (float)strtof(cJSON_GetStringValue(item), &endptr);
        if (errno != 0 || *endptr != '\0') {
            return E_INVALID_NUMBER_STRING;
        }
        *field = num_value;
    } else {
        return E_UNACCEPTABLE_JSON_TYPE;
    }

    return E_SUCCESS;
}

RtnCode
cvt_single_double(double* field, const cJSON* item)
{
    assert(field != NULL && item != NULL);

    if (cJSON_IsNumber(item)) {
        *field = (double)item->valuedouble;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                  = 0;
        char*        endptr    = NULL;
        const double num_value = (double)strtod(cJSON_GetStringValue(item), &endptr);
        if (errno != 0 || *endptr != '\0') {
            return E_INVALID_NUMBER_STRING;
        }
        *field = num_value;
    } else {
        return E_UNACCEPTABLE_JSON_TYPE;
    }

    return E_SUCCESS;
}

RtnCode
cvt_single_bool(bool* const field, const cJSON* const item, StringEnumCallback string_enum)
{
    assert(field != NULL && item != NULL);

    if (cJSON_IsTrue(item)) {
        *field = true;
    } else if (cJSON_IsFalse(item)) {
        *field = false;
    } else if (cJSON_GetStringValue(item) != NULL && string_enum != NULL) {
        *field = string_enum(cJSON_GetStringValue(item)); /* TODO: unsafe, need to check return value of string_enum is valid */
    } else if (cJSON_IsNumber(item)) {
        *field = cJSON_GetNumberValue(item) == 0 ? false : true;
    } else {
        return E_UNACCEPTABLE_JSON_TYPE;
    }

    return E_SUCCESS;
}

RtnCode
cvt_single_string(char** field, const cJSON* item)
{
    assert(field != NULL && item != NULL);

    if (cJSON_GetStringValue(item) != NULL) {
        asprintf(field, "%s", cJSON_GetStringValue(item));
    } else {
        return E_UNACCEPTABLE_JSON_TYPE;
    }

    return E_SUCCESS;
}

RtnCode
cvt_single_bytes(ProtobufCBinaryData* const field, const cJSON* const item, const ByteMode mode)
{
    assert(field != NULL && item != NULL);

    if (cJSON_GetStringValue(item) != NULL) {
        uint8_t* b_data = NULL;
        size_t   b_len  = 0;
        RtnCode  ret    = E_SUCCESS;

        switch (mode) {
        case BYTE_MODE_FILE_PATH:
            ret = util_cvt_file_to_bytes(cJSON_GetStringValue(item), &b_data, &b_len);
            break;

        case BYTE_MODE_BASE64:
            ret = util_cvt_base64_to_bytes(cJSON_GetStringValue(item), &b_data, &b_len);
            break;

        case BYTE_MODE_HEX:
            ret = util_cvt_hex_to_bytes(cJSON_GetStringValue(item), &b_data, &b_len);
            break;

        default:
            return E_UNKNOWN;
        }

        if (E_SUCCESS == ret) {
            field->data = b_data;
            field->len  = b_len;
        } else {
            free(b_data);
        }

        return ret;
    } else {
        return E_UNACCEPTABLE_JSON_TYPE;
    }
}

RtnCode
cvt_single_enum(int* const field, const ProtobufCEnumDescriptor* const enum_desc, const cJSON* const item, StringEnumCallback string_enum)
{
    assert(field != NULL && item != NULL && enum_desc != NULL);

    if (cJSON_IsNumber(item)) {
        *field = cJSON_GetNumberValue(item); /* TODO: check if the value is valid */
    } else if (cJSON_GetStringValue(item) != NULL) {
        if (NULL == string_enum) {
            *field = default_string_enum_conversion(enum_desc, cJSON_GetStringValue(item));
        } else {
            *field = string_enum(cJSON_GetStringValue(item)); /* TODO: unsafe, need to check return value of string_enum is valid */
        }
    } else {
        return E_UNACCEPTABLE_JSON_TYPE;
    }

    return E_SUCCESS;
}

RtnCode
cvt_single_message(ProtobufCMessage* const msg, const cJSON* const item, MsgConvertorCallback msg_convertor)
{
    assert(msg != NULL && item != NULL && msg->descriptor != NULL);
    if (NULL == msg_convertor) {
        return default_msg_convertor(msg, item);
    } else {
        return msg_convertor(msg, item);
    }
}

static RtnCode
util_cvt_hex_to_bytes(const char* const hex_str, uint8_t** const bytes, size_t* const bytes_len)
{

    assert(hex_str != NULL && bytes != NULL && bytes_len != NULL);

    char* buffer = strdup(hex_str);
    if (NULL == buffer) {
        return E_MEM_ALLOC;
    }

    size_t hex_len = strlen(buffer);

    if (hex_len < 2) {
        free(buffer);
        return E_INVALID_HEX_STRING;
    }

    size_t   b_len   = (hex_len % 3 == 0) ? (hex_len / 3) : (hex_len / 3 + 1);
    uint8_t* b_data  = (uint8_t*)calloc(b_len, sizeof(uint8_t));
    size_t   b_index = 0;

    char* token  = strtok(buffer, " ");
    char* endptr = NULL;
    while (token != NULL) {
        errno        = 0;
        uint8_t byte = (uint8_t)strtoul(token, &endptr, 16);
        if (errno != 0 || *endptr != '\0') {
            break;
        }
        b_data[b_index++] = byte;
        token             = strtok(NULL, " ");
    }

    if (b_index != b_len) {
        free(b_data);
        free(buffer);
        return E_INVALID_HEX_STRING;
    }

    free(buffer);

    *bytes     = b_data;
    *bytes_len = b_len;

    return E_SUCCESS;
}

static RtnCode
util_cvt_base64_to_bytes(const char* const base64_str, uint8_t** const bytes, size_t* const bytes_len)
{
    return E_UNKNOWN;
}

static RtnCode
util_cvt_file_to_bytes(const char* const file_path, uint8_t** const bytes, size_t* const bytes_len)
{
    assert(file_path != NULL && bytes != NULL && bytes_len != NULL);

    FILE* fp = fopen(file_path, "rb");
    if (NULL == fp) {
        return E_INVALID_FILEPATH;
    }

    fseek(fp, 0, SEEK_END);
    const size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t* b_data = (uint8_t*)calloc(size, sizeof(uint8_t));
    if (fread(b_data, sizeof(uint8_t), size, fp) != size) {
        if (ferror(fp) != 0) {
            perror("fread failed");
        }
        free(b_data);
        fclose(fp);
        return E_FILE_OPEN;
    } else {
        *bytes     = b_data;
        *bytes_len = size;
        fclose(fp);
        return E_SUCCESS;
    }
}