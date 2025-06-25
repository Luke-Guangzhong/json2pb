/**
 * @file easy_interface.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-06-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "json2pb.h"

j2p_expt_t
cvt_json_2_pb_number(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name)
{
    if (NULL == root || NULL == msg || NULL == field_name) {
        return J2P_EXPT_INVALID_ARG;
    }

    return cvt_json_2_pb_field(root, item, msg, field_name, NULL, NULL, J2P_FILE_PATH_STR);
}