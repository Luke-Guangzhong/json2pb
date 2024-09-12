/**
 * @file common.h
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @date 2024-07-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROCNAME "LEAKTEST"

#define LOG_COLOR_YELLOW "\033[33m"
#define LOG_COLOR_BLUE   "\033[34m"
#define LOG_COLOR_RED    "\033[31m"
#define LOG_COLOR_NONE   "\033[0m"

#if 0
#define DEBUG
#else
#define DEBUG(format, args...)                                                    \
    do {                                                                          \
        printf("[%s][%s][%d]:" format, PROCNAME, __FUNCTION__, __LINE__, ##args); \
        fflush(stdout);                                                           \
    } while (0)

#endif

#define INFO(format, args...)                    \
    do {                                         \
        printf("[%s]" format, PROCNAME, ##args); \
        fflush(stdout);                          \
    } while (0)

#define ERROR(format, args...)                                                                                                        \
    do {                                                                                                                              \
        fprintf(stderr, LOG_COLOR_NONE LOG_COLOR_RED "[%s][%s][%d]" LOG_COLOR_NONE format, PROCNAME, __FUNCTION__, __LINE__, ##args); \
        fflush(stderr);                                                                                                               \
    } while (0)

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

#endif