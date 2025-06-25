/**
 * @file test_cvt_int32_t.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-06-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdlib.h>

#include <CUnit/Basic.h>

#include "json2pb.h"
#include "test.pb-c.h"

void
test_cvt_single_int32_t(void)
{
    TestMessage* msg = (TestMessage*)calloc(1, sizeof(TestMessage));

    test_message__init(msg);
}

void
test_incorrect_operation(void)
{
    TestMessage* msg = (TestMessage*)calloc(1, sizeof(TestMessage));
    j2p_expt_t   e   = cvt_json_2_pb_field(NULL, NULL, NULL, NULL, NULL, NULL, J2P_FILE_HEX_STR);
    CU_ASSERT_TRUE(e == J2P_EXPT_INVALID_ARG);
}

int
maxi(int i1, int i2)
{
    return (i1 > i2) ? i1 : i2;
}

void
test_maxi(void)
{
    CU_ASSERT(maxi(0, 2) == 2);
    CU_ASSERT(maxi(0, -2) == -1);
    CU_ASSERT(maxi(2, 2) == 2);
}

int
main(int argc, char const* argv[])
{
    unsigned  rv    = 1;
    CU_pSuite suite = NULL;

    if (CUE_SUCCESS == CU_initialize_registry()) {

        suite = CU_add_suite("test number convertion", NULL, NULL);
        CU_add_test(suite, "test single int32 convertion", test_cvt_single_int32_t);

        suite = CU_add_suite("test wrong operation", NULL, NULL);
        CU_add_test(suite, "test incorrect operation", test_incorrect_operation);

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
