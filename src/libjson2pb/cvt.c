/**
 * @file cvt.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "internal.h"
#include "json2pb.h"

j2p_expt*
cvt_single_int32_t(const cJSON* const root, const cJSON* const item, int32_t* const field)
{
    assert(NULL != root);
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (isinf(num_value) || isnan(num_value) || num_value < INT32_MIN || num_value > INT32_MAX) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (int32_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                = 0;
        char*      endptr    = NULL;
        const long num_value = strtol(cJSON_GetStringValue(item), &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            JSON2PB_THROW_EXCEPTION(JSON2PB_INVALID_NUMBER_STRING);
        } else if (num_value < INT32_MIN || num_value > INT32_MAX) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (int32_t)num_value;
    } else if (cJSON_IsNull(item)) {
        JSON2PB_THROW_EXCEPTION(JSON2PB_NULL_VALUE);
    } else {
        JSON2PB_THROW_EXCEPTION(JSON2PB_UNACCEPTABLE_JSON_TYPE);
    }

    JSON2PB_THROW_EXCEPTION(JSON2PB_SUCCESS);
}

j2p_expt*
cvt_single_int64_t(const cJSON* const root, const cJSON* const item, int64_t* const field)
{
    assert(NULL != root);
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (isinf(num_value) || isnan(num_value) || num_value < INT64_MIN || num_value > INT64_MAX) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (int64_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                     = 0;
        char*           endptr    = NULL;
        const long long num_value = strtoll(cJSON_GetStringValue(item), &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            JSON2PB_THROW_EXCEPTION(JSON2PB_INVALID_NUMBER_STRING);
        } else if (num_value < INT64_MIN || num_value > INT64_MAX) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (int64_t)num_value;
    } else if (cJSON_IsNull(item)) {
        JSON2PB_THROW_EXCEPTION(JSON2PB_NULL_VALUE);
    } else {
        JSON2PB_THROW_EXCEPTION(JSON2PB_UNACCEPTABLE_JSON_TYPE);
    }

    JSON2PB_THROW_EXCEPTION(JSON2PB_SUCCESS);
}

j2p_expt*
cvt_single_uint32_t(const cJSON* const root, const cJSON* const item, uint32_t* const field)
{
    assert(NULL != root);
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (isinf(num_value) || isnan(num_value) || num_value < 0 || num_value > UINT32_MAX) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (uint32_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                         = 0;
        char*               endptr    = NULL;
        const unsigned long num_value = strtoul(cJSON_GetStringValue(item), &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            JSON2PB_THROW_EXCEPTION(JSON2PB_INVALID_NUMBER_STRING);
        } else if (num_value < 0 || num_value > UINT32_MAX) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (uint32_t)num_value;
    } else if (cJSON_IsNull(item)) {
        JSON2PB_THROW_EXCEPTION(JSON2PB_NULL_VALUE);
    } else {
        JSON2PB_THROW_EXCEPTION(JSON2PB_UNACCEPTABLE_JSON_TYPE);
    }

    JSON2PB_THROW_EXCEPTION(JSON2PB_SUCCESS);
}

j2p_expt*
cvt_single_uint64_t(const cJSON* const root, const cJSON* const item, uint64_t* const field)
{
    assert(NULL != root);
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (isinf(num_value) || isnan(num_value) || num_value < 0 || num_value > UINT64_MAX) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (uint64_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                              = 0;
        char*                    endptr    = NULL;
        const unsigned long long num_value = strtoull(cJSON_GetStringValue(item), &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            JSON2PB_THROW_EXCEPTION(JSON2PB_INVALID_NUMBER_STRING);
        } else if (num_value < 0 || num_value > UINT64_MAX) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (uint64_t)num_value;
    } else if (cJSON_IsNull(item)) {
        JSON2PB_THROW_EXCEPTION(JSON2PB_NULL_VALUE);
    } else {
        JSON2PB_THROW_EXCEPTION(JSON2PB_UNACCEPTABLE_JSON_TYPE);
    }

    JSON2PB_THROW_EXCEPTION(JSON2PB_SUCCESS);
}

j2p_expt*
cvt_single_float(const cJSON* const root, const cJSON* const item, float* const field)
{
    assert(NULL != root);
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (isinf(num_value) || isnan(num_value) || num_value < -__FLT_MAX__ || num_value > __FLT_MAX__) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (float)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                 = 0;
        char*       endptr    = NULL;
        const float num_value = strtof(cJSON_GetStringValue(item), &endptr);
        if (errno != 0 || *endptr != '\0') {
            JSON2PB_THROW_EXCEPTION(JSON2PB_INVALID_NUMBER_STRING);
        } else if (num_value < -__FLT_MAX__ || num_value > __FLT_MAX__) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (float)num_value;
    } else if (cJSON_IsNull(item)) {
        JSON2PB_THROW_EXCEPTION(JSON2PB_NULL_VALUE);
    } else {
        JSON2PB_THROW_EXCEPTION(JSON2PB_UNACCEPTABLE_JSON_TYPE);
    }

    JSON2PB_THROW_EXCEPTION(JSON2PB_SUCCESS);
}

j2p_expt*
cvt_single_double(const cJSON* const root, const cJSON* const item, double* const field)
{
    assert(NULL != root);
    assert(NULL != item);
    assert(NULL != field);

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (isinf(num_value) || isnan(num_value)) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (double)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                  = 0;
        char*        endptr    = NULL;
        const double num_value = strtod(cJSON_GetStringValue(item), &endptr);
        if (errno != 0 || *endptr != '\0') {
            JSON2PB_THROW_EXCEPTION(JSON2PB_INVALID_NUMBER_STRING);
        }
        *field = (double)num_value;
    } else if (cJSON_IsNull(item)) {
        JSON2PB_THROW_EXCEPTION(JSON2PB_NULL_VALUE);
    } else {
        JSON2PB_THROW_EXCEPTION(JSON2PB_UNACCEPTABLE_JSON_TYPE);
    }

    JSON2PB_THROW_EXCEPTION(JSON2PB_SUCCESS);
}