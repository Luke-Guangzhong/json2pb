/**
 * @file utils.h
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-06-27
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>

#include "json2pb.h"
#include "test.pb-c.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/******************************************************************************/
/*                                Declarations                                */
/******************************************************************************/

static void setup_successful_conversion(void);
static void teardown_successful_conversion(void);

static void sanitize_name(const char* in, char* out, size_t out_sz);
static void setup_redirect_stdout(void);
static void teardown_redirect_stdout(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

static TestMessage* msg  = NULL;
static cJSON*       root = NULL;

static int  saved_stdout_fd = -1;
static char log_path[PATH_MAX];
static char log_path[PATH_MAX];

/******************************************************************************/
/*                                 Utilities                                  */
/******************************************************************************/

/* 把测试用例名转换成文件名安全的格式，比如把空格和特殊字符换成下划线 */
static void
sanitize_name(const char* in, char* out, size_t out_sz)
{
    size_t j = 0;
    for (size_t i = 0; in[i] != '\0' && j + 1 < out_sz; ++i) {
        char c = in[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            out[j++] = c;
        } else {
            out[j++] = '_';
        }
    }
    out[j] = '\0';
}

void
setup_successful_conversion(void)
{
    msg = (TestMessage*)calloc(1, sizeof(TestMessage));
    if (NULL == msg) {
        exit(EXIT_FAILURE);
    }
    test_message__init(msg);
    root = cJSON_CreateObject();
    if (NULL == root) {
        exit(EXIT_FAILURE);
    }

    setup_redirect_stdout();

    return;
}

void
setup_redirect_stdout(void)
{
    CU_pTest current = CU_get_current_test();
    if (!current || !current->pName) {
        exit(EXIT_FAILURE);
    }

    /* 确保 build/log 目录存在 */
    if (mkdir("log", 0777) != 0 && errno != EEXIST) {
        perror("mkdir log");
        exit(EXIT_FAILURE);
    }

    /* 备份 stdout */
    fflush(stdout);
    saved_stdout_fd = dup(fileno(stdout));
    if (saved_stdout_fd < 0) {
        perror("dup stdout");
        exit(EXIT_FAILURE);
    }

    /* 生成日志文件路径 */
    {
        char safe_name[PATH_MAX - 11] = {};
        sanitize_name(current->pName, safe_name, sizeof(safe_name));
        snprintf(log_path, sizeof(log_path), "log/%s.log", safe_name);
    }

    /* 打开日志文件并重定向 stdout */
    FILE* f = fopen(log_path, "w");
    if (!f) {
        perror("fopen log file");
        /* 恢复 stdout */
        dup2(saved_stdout_fd, fileno(stdout));
        close(saved_stdout_fd);
        exit(EXIT_FAILURE);
    }
    if (dup2(fileno(f), fileno(stdout)) < 0) {
        perror("dup2 to stdout");
        fclose(f);
        dup2(saved_stdout_fd, fileno(stdout));
        close(saved_stdout_fd);
        exit(EXIT_FAILURE);
    }
    fclose(f);
}

void
teardown_successful_conversion(void)
{
    cJSON_Delete(root);
    test_message__free_unpacked(msg, NULL);
    msg  = NULL;
    root = NULL;

    teardown_redirect_stdout();

    return;
}

void
teardown_redirect_stdout(void)
{
    fflush(stdout);
    if (saved_stdout_fd >= 0) {
        dup2(saved_stdout_fd, fileno(stdout));
        close(saved_stdout_fd);
        saved_stdout_fd = -1;
    }
}