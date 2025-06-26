/**
 * @file test_public_interface.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-06-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>

#include "json2pb.h"
#include "test.pb-c.h"

/******************************************************************************/
/*                                Declarations                                */
/******************************************************************************/

static void sanitize_name(const char* in, char* out, size_t out_sz);
void        setup_successful_conversion(void);
void        teardown_successful_conversion(void);

void test_cvt_json_to_deprecated_field(void);
void test_cvt_json_to_oneof_field(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

TestMessage* msg  = NULL;
cJSON*       root = NULL;

static int  saved_stdout_fd = -1;
static char log_path[PATH_MAX];

CU_SuiteInfo suites[] = {CU_SUITE_INFO_NULL};

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
    return;
}

void
teardown_successful_conversion(void)
{
    cJSON_Delete(root);
    test_message__free_unpacked(msg, NULL);
    msg  = NULL;
    root = NULL;
    fflush(stdout);
    if (saved_stdout_fd >= 0) {
        dup2(saved_stdout_fd, fileno(stdout));
        close(saved_stdout_fd);
        saved_stdout_fd = -1;
    }
    return;
}

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

/******************************************************************************/
/*                                 Main Code                                  */
/******************************************************************************/

int
main(int argc, char const* argv[])
{
    unsigned  rv    = 1;
    CU_pSuite suite = NULL;

    if (CUE_SUCCESS == CU_initialize_registry() && CUE_SUCCESS == CU_register_suites(suites)) {
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_basic_show_failures(CU_get_failure_list());
        rv = CU_get_number_of_tests_failed();
        CU_cleanup_registry();
    }

    if (0 != rv) {
        exit(EXIT_FAILURE);
    }
    return 0;
}
