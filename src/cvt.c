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
            return J2P_EXPT_INVALID_NUMBER_STRING;
        } else if (num_value < INT32_MIN || num_value > INT32_MAX) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        *field = (int32_t)num_value;
    } else if (cJSON_IsNull(item)) {
        return J2P_EXPT_NULL_VALUE;
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
        if (isinf(num_value) || isnan(num_value) || num_value < INT64_MIN || num_value > INT64_MAX) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        *field = (int64_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                     = 0;
        char*           endptr    = NULL;
        const long long num_value = strtoll(cJSON_GetStringValue(item), &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            return J2P_EXPT_INVALID_NUMBER_STRING;
        } else if (num_value < INT64_MIN || num_value > INT64_MAX) {
            return J2P_EXPT_VALUE_OVERFLOW;
        }
        *field = (int64_t)num_value;
    } else if (cJSON_IsNull(item)) {
        return J2P_EXPT_NULL_VALUE;
    } else {
        return J2P_EXPT_UNACCEPTABLE_JSON_TYPE;
    }

    return J2P_EXPT_SUCCESS;
}
