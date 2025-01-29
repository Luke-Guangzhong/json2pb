/**
 * @file main.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdbool.h>

#include "common.h"
#include "device.pb-c.h"
#include "json2pb.h"

static const char* json_str = "{\"signal_strength\":[1,4,5,{\"test\":12}]}";

int
main()
{
    DEBUG("HELLO WORLD\n");
    cJSON* root = cJSON_Parse(json_str);
    assert(root != NULL);

    Device* device = (Device*)calloc(1, sizeof(Device));
    assert(device != NULL);

    device__init(device);

    j2p_expt* e = cvt_cjson_2_proto_c_field(root, (ProtobufCMessage*)device, cJSON_GetObjectItem(root, "signal_strength"), "signal_strength");
    if (e != NULL) {
        if (e->msg->type != JSON2PB_SUCCESS) {
            ERROR("convert failed (%s)\n\tconverting json path \"%s\"\n", e->msg->message, e->where);
        }
        FREE_JSON2PB_EXCEPTION(e);
    } else {
        exit(EXIT_FAILURE);
    }

    size_t sizeOne = device__get_packed_size(device);
    DEBUG("sizeOne: %ld\n", sizeOne);
    byte* pb_data = (byte*)calloc(1, sizeOne);
    assert(pb_data != NULL);

    size_t sizeTwo = device__pack(device, pb_data);
    assert(sizeTwo == sizeOne);

    INFO("pb_data: \"");
    for (size_t i = 0; i < sizeOne; i++) {
        printf("%02x", pb_data[i]);
        if (i % 16 == 15) {
            printf("\n");
        }
    }
    printf("\"\n");

    cJSON_Delete(root);
    device__free_unpacked(device, NULL);
    FREE(pb_data);

    return 0;
}