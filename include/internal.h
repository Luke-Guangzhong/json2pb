/**
 * @file internal.h
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <stdint.h>

#include "json2pb.h"

typedef j2p_expt_t (*single_field_cvt_func)(const cJSON* const item, void* const field);

j2p_expt_t cvt_single_int32_t(const cJSON* const item, int32_t* const field);

j2p_expt_t cvt_single_int64_t(const cJSON* const item, int64_t* const field);

j2p_expt_t cvt_single_uint32_t(const cJSON* const item, uint32_t* const field);

j2p_expt_t cvt_single_uint64_t(const cJSON* const item, uint64_t* const field);

j2p_expt_t cvt_single_float(const cJSON* const item, float* const field);

bool default_string_bool_convertor(const char* const str);