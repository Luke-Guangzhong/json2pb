/**
 * @file common.h
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cimplog/cimplog.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t byte;

typedef struct binary_data b_data_t;

/******************************************************************************/
/*                                Custom Types                                */
/******************************************************************************/

struct binary_data {
    byte*  data;
    size_t size;
};

/******************************************************************************/
/*                                 LOG OUTPUT                                 */
/******************************************************************************/

#define LOG_COLOR_YELLOW "\033[33m"
#define LOG_COLOR_BLUE   "\033[34m"
#define LOG_COLOR_RED    "\033[31m"
#define LOG_COLOR_NONE   "\033[0m"

#define MODULE_NAME "BLANK"

#ifdef NDEBUG /* set 1 before release */
#define DEBUG
#else
#define DEBUG(format, args...)                                                           \
    do {                                                                                 \
        cimplog_debug(MODULE_NAME, "[%s][%d]: " format, __FUNCTION__, __LINE__, ##args); \
    } while (0)

#endif

#define INFO(format, args...)                      \
    do {                                           \
        cimplog_info(MODULE_NAME, format, ##args); \
    } while (0)

#define ERROR(format, args...)                                                                                                       \
    do {                                                                                                                             \
        cimplog_error(MODULE_NAME, LOG_COLOR_NONE LOG_COLOR_RED "[%s][%d]: " LOG_COLOR_NONE format, __FUNCTION__, __LINE__, ##args); \
    } while (0)

/******************************************************************************/
/*                            RESOURCE FREE MACRO                             */
/******************************************************************************/

#ifndef FREE
#define FREE(pointer)   \
    do {                \
        free(pointer);  \
        pointer = NULL; \
    } while (0)
#endif

#ifndef FCLOSE
#define FCLOSE(fp)  \
    do {            \
        fclose(fp); \
        fp = NULL;  \
    } while (0)
#endif

#ifndef CLOSE
#define CLOSE(fd)  \
    do {           \
        close(fd); \
        fd = 0;    \
    } while (0)
#endif

#ifndef PCLOSE
#define PCLOSE(fp)  \
    do {            \
        pclose(fp); \
        fp = NULL;  \
    } while (0)
#endif

/******************************************************************************/
/*                                JUDGE MACRO                                 */
/******************************************************************************/

#ifndef IS_NULL
#define IS_NULL(ptr) (NULL == (ptr))
#endif /* IS_NULL */

#ifndef IS_NOT_NULL
#define IS_NOT_NULL(ptr) (NULL != (ptr))
#endif /* IS_NOT_NULL */