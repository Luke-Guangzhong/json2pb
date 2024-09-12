/**
 * @file example_pkg.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-07-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef EXAMPLE_PKG_H
#define EXAMPLE_PKG_H

#include "cjson/cJSON.h"
#include <stdint.h>

typedef enum {
    TRIGGER_PERIODIC,
    TRIGGER_API,
} Trigger_t;

int pack_example_msg(uint8_t** const output, const cJSON* const json_obj, size_t* const output_len);

int pack_device_msg(uint8_t** const output, const cJSON* const root, size_t* const output_len);

#endif /* EXAMPLE_PKG_H */