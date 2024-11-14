/**
 * @file example_pkg.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @date 2024-07-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson2pb-c.h"
#include "example.pb-c.h"
#include "example_pkg.h"
#include "pb-c2cjson.h"

uint32_t
convert_cjson_to_example_msg(Example* const example, const cJSON* const root)
{
    assert(example != NULL && root != NULL && example->base.descriptor == &example__descriptor);

    cvt_cjson_2_pb_string(cJSON_GetObjectItem(root, "mac"), (ProtobufCMessage*)example, "mac");
    cvt_cjson_2_pb_number(cJSON_GetObjectItem(root, "timestamp"), (ProtobufCMessage*)example, "timestamp");
    cvt_cjson_2_pb_byte(cJSON_GetObjectItem(root, "exec_file"), (ProtobufCMessage*)example, "exec_file", BYTE_MODE_HEX);
    cvt_cjson_2_pb_byte(cJSON_GetObjectItem(root, "lib_file"), (ProtobufCMessage*)example, "lib_file", BYTE_MODE_BASE64);
    cvt_cjson_2_pb_enum(cJSON_GetObjectItem(root, "algorithm"), (ProtobufCMessage*)example, "algorithm", NULL);
    cvt_cjson_2_pb_enum(cJSON_GetObjectItem(root, "supported_algorithm"), (ProtobufCMessage*)example, "supported_algorithm", NULL);
    cvt_cjson_2_pb_message(cJSON_GetObjectItem(root, "publisher"), (ProtobufCMessage*)example, "publisher", NULL);
    cvt_cjson_2_pb_message(cJSON_GetObjectItem(root, "contributer"), (ProtobufCMessage*)example, "contributer", NULL);
    cvt_cjson_2_pb_message(cJSON_GetObjectItem(root, "device"), (ProtobufCMessage*)example, "device", NULL);
    cvt_cjson_2_pb_number(cJSON_GetObjectItem(root, "sea"), (ProtobufCMessage*)example, "sea");

    return 0;
}

int
pack_example_msg(uint8_t** const output, const cJSON* const json_obj, size_t* const output_len)
{
    Example* example = (Example*)calloc(1, sizeof(Example));
    if (example == NULL) {
        return -1;
    }
    example__init(example);

    convert_cjson_to_example_msg(example, json_obj);

    {
        cJSON* example_json = cJSON_CreateObject();
        for (size_t i = 0; i < example->base.descriptor->n_fields; i++) {
            cJSON_AddItemToObject(example_json, example->base.descriptor->fields[i].name, gen_cjson_via_pb_field((ProtobufCMessage*)example, example->base.descriptor->fields[i].name, true, true, BYTE_MODE_HEX));
        }
        char* example_json_str = cJSON_Print(example_json);
        printf("%s\n", example_json_str);
        free(example_json_str);
        cJSON_Delete(example_json);
    }

    size_t   size1 = example__get_packed_size(example);
    uint8_t* data  = (uint8_t*)calloc(size1, sizeof(uint8_t));
    size_t   size2 = example__pack(example, data);
    if (size1 == size2) {
        *output     = data;
        *output_len = size2;
    } else {
        free(data);
    }

    example__free_unpacked(example, NULL);

    return 0;
}

int
convert_cjson_to_device_msg(Device* const device, const cJSON* const root)
{
    assert(device != NULL && root != NULL && device->base.descriptor == &device__descriptor);

    cvt_cjson_2_proto_c_field(cJSON_GetObjectItem(root, "rssi"), (ProtobufCMessage*)device, "rssi", NULL, NULL, NULL, BYTE_MODE_FILE_PATH);
    cvt_cjson_2_proto_c_field(cJSON_GetObjectItem(root, "signal_strength"), (ProtobufCMessage*)device, "signal_strength", NULL, NULL, NULL, BYTE_MODE_FILE_PATH);

    return 0;
}

int
pack_device_msg(uint8_t** const output, const cJSON* const json_obj, size_t* const output_len)
{
    Device* device = (Device*)calloc(1, sizeof(Device));
    if (device == NULL) {
        return -1;
    }
    device__init(device);

    int stat = 0;

    convert_cjson_to_device_msg(device, json_obj);

    size_t   size1 = device__get_packed_size(device);
    uint8_t* data  = (uint8_t*)calloc(1, size1);
    size_t   size2 = device__pack(device, data);
    if (size1 == size2) {
        *output     = data;
        *output_len = size2;
    } else {
        free(data);
    }
    device__free_unpacked(device, NULL);
    return stat;
}