/**
 * @file main.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @date 2024-07-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "cjson/cJSON.h"
#include "cjson2pb-c.h"
#include "common.h"
#include "example_pkg.h"
#include "utilities.h"

#ifdef __GLIBC__
#include <gnu/libc-version.h>
#endif

#include <stdio.h>

int
main(int argc, char const* argv[])
{
    printf("Hello, world!\n");
#ifdef __GLIBC__
    printf("GNU C Library version %s %s\n", gnu_get_libc_release(), gnu_get_libc_version());
#endif

    char* data = NULL;
    readFromFile("../example/resource/example.json", &data);
    cJSON*   root       = cJSON_Parse(data);
    char*    json_str   = cJSON_PrintUnformatted(root);
    uint8_t* output     = NULL;
    size_t   output_len = 0;
    pack_example_msg(&output, root, &output_len);
    print_uint8_in_hex(output, output_len);
    cJSON_Delete(root);
    free(data);
    free(json_str);
    free(output);
    return 0;
}
