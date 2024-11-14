/**
 * @file global.h
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2024-11-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdbool.h>
#include <stdlib.h>

#include "cjson/cJSON.h"
#include "protobuf-c/protobuf-c.h"

typedef enum byte_mode {
    BYTE_MODE_FILE_PATH,
    BYTE_MODE_HEX,
    BYTE_MODE_BASE64
} ByteMode;

#define JSON_EXCEPTION_LIST(METHOD)                                                                                                                                                                             \
    METHOD(E_NULL_VALUE, 0x00000001, "json object is null"), METHOD(E_EMPTY_ARRAY, 0x00000002, "convert empty array to a repeated protobuf field"),                                                             \
        METHOD(E_UNACCEPTABLE_JSON_TYPE, 0x00000004, "convert unacceptable json type to a protobuf field"), METHOD(E_INVALID_FILEPATH, 0x00000008, "filepath for bytes field does not exist or is not a file"), \
        METHOD(E_INVALID_BASE64, 0x00000010, "error in base64 decoding"), METHOD(E_INVALID_HEX_STRING, 0x00000020, "error in hex decoding, possibly unsupported format"),                                       \
        METHOD(E_INVALID_NUMBER_STRING, 0x00000040, "cannot convert string to number with specified type"), METHOD(E_TYPE_OVERFLOW, 0x00000080, "number in string format is out of range for specified type"),  \
        METHOD(E_INVALID_ENUM, 0x00000100, "enum value (number provided by json) is out of range")

#define PROTOBUF_EXCEPTION_LIST(METHOD) METHOD(E_INVALID_FIELD_NAME, 0x00010000, "field name is not found in protobuf message"), METHOD(E_UNINITIALIZED, 0x00020000, "protobuf message has not been initialized")

#define USER_EXCEPTION_LIST(METHOD) METHOD(E_INVALID_ARG, 0x00100000, "invalid argument, possibly NULL value"), METHOD(E_INVALID_STRING_ENUM_CALLBACK, 0x00200000, "enum value (number provided by callback function) is out of range")

#define SYSTEM_EXCEPTION_LIST(METHOD) METHOD(E_FILE_OPEN, 0x01000000, "file open error"), METHOD(E_MEM_ALLOC, 0x02000000, "memory allocation error")

#define ENUM_K_V(KEY, VAL, DESC)  KEY = VAL
#define ONLY_ENUM(KEY, VAL, DESC) KEY
#define ONLY_DESC(KEY, VAL, DESC) DESC

#define JSON_EXCEPTION(METHOD) JSON_EXCEPTION_LIST(METHOD), E_JSON_EXCEPTION_MAX /* max value of json exception */

#define PROTOBUF_EXCEPTION(METHOD) PROTOBUF_EXCEPTION_LIST(METHOD), E_PROTOBUF_EXCEPTION_MAX /* max value of protobuf exception */

#define USER_EXCEPTION(METHOD) USER_EXCEPTION_LIST(METHOD), E_USER_EXCEPTION_MAX /* max value of user exception */

#define SYSTEM_EXCEPTION(METHOD) SYSTEM_EXCEPTION_LIST(METHOD), E_UNKNOWN /* unknown error, max value of system exception */

typedef enum rtn_code {
    E_SUCCESS = 0x00000000,

    /* JSON Exception */

    JSON_EXCEPTION(ENUM_K_V),

    /* Protobuf Exception */

    PROTOBUF_EXCEPTION(ENUM_K_V),

    /* Other Exception */

    USER_EXCEPTION(ENUM_K_V),

    /* System */

    SYSTEM_EXCEPTION(ENUM_K_V),

} RtnCode;

#define E_JSON_EXCEPTION_MASK     0x0000ffff /* json exception mask */
#define E_PROTOBUF_EXCEPTION_MASK 0x000f0000 /* protobuf exception mask */
#define E_USER_EXCEPTION_MASK     0x00f00000 /* user exception mask */
#define E_SYSTEM_EXCEPTION_MASK   0x0f000000 /* system exception mask */

static RtnCode rtn_code_enum_list[] = {JSON_EXCEPTION_LIST(ONLY_ENUM), PROTOBUF_EXCEPTION_LIST(ONLY_ENUM), USER_EXCEPTION_LIST(ONLY_ENUM), SYSTEM_EXCEPTION_LIST(ONLY_ENUM)};
static char*   rtn_code_desc_list[] = {JSON_EXCEPTION_LIST(ONLY_DESC), PROTOBUF_EXCEPTION_LIST(ONLY_DESC), USER_EXCEPTION_LIST(ONLY_DESC), SYSTEM_EXCEPTION_LIST(ONLY_DESC)};

#undef JSON_EXCEPTION_LIST
#undef PROTOBUF_EXCEPTION_LIST
#undef USER_EXCEPTION_LIST
#undef SYSTEM_EXCEPTION_LIST
#undef ENUM_K_V
#undef ONLY_ENUM
#undef JSON_EXCEPTION
#undef PROTOBUF_EXCEPTION
#undef USER_EXCEPTION
#undef SYSTEM_EXCEPTION

#endif /* _GLOBAL_H_ */