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

j2p_expt* cvt_single_int32_t(const cJSON* const root, const cJSON* const item, int32_t* const field);