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
            exit(EXIT_FAILURE);
        }
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }
    return J2P_EXPT_SUCCESS;
}