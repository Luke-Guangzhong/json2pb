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
#include <stdlib.h>

#include "internal.h"
#include "json2pb.h"

j2p_expt*
cvt_single_int32_t(const cJSON* const root, const cJSON* const item, int32_t* const field)
{
    assert(NULL != root);
    assert(NULL != item);
    assert(NULL != field);

    {
        char* root_str = cJSON_Print(root);
        if (NULL == root_str) {
            printf("cvt_single_int32_t: cJSON_Print failed\n");
        } else {
            printf("cvt_single_int32_t: root_str = %s\n", root_str);
        }

        char* item_str = cJSON_Print(item);
        if (NULL == item_str) {
            printf("cvt_single_int32_t: cJSON_Print failed\n");
        } else {
            printf("cvt_single_int32_t: item_str = %s\n", item_str);
        }
    }

    if (cJSON_IsNumber(item)) {
        const double num_value = cJSON_GetNumberValue(item);
        if (isnan(num_value) || num_value < INT32_MIN || num_value > INT32_MAX) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (int32_t)num_value;
    } else if (NULL != cJSON_GetStringValue(item)) {
        errno                = 0;
        char*      endptr    = NULL;
        const long num_value = strtol(cJSON_GetStringValue(item), &endptr, 0);
        if (errno != 0 || *endptr != '\0') {
            JSON2PB_THROW_EXCEPTION(JSON2PB_INVALID_NUMBER_STRING);
        } else if (num_value > INT32_MAX || num_value < INT32_MIN) {
            JSON2PB_THROW_EXCEPTION(JSON2PB_VALUE_OVERFLOW);
        }
        *field = (int32_t)num_value;
    } else {
        JSON2PB_THROW_EXCEPTION(JSON2PB_UNACCEPTABLE_JSON_TYPE);
    }

    return NULL;
}