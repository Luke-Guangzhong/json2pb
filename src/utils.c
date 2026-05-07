/**
 * @file utils.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-07-01
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "json2pb.h"
#include "trower-base64/base64.h"
#include <errno.h>
#include <protobuf-c/protobuf-c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
int_range_lookup(unsigned n_ranges, const ProtobufCIntRange* ranges, int value)
{
    if (n_ranges == 0)
        return -1;

    unsigned start = 0;
    unsigned n     = n_ranges;
    while (n > 1) {
        unsigned mid = start + n / 2;

        if (value < ranges[mid].start_value) {
            n = mid - start;
        } else if (value >= ranges[mid].start_value + (int)(ranges[mid + 1].orig_index - ranges[mid].orig_index)) {
            unsigned new_start = mid + 1;
            n                  = start + n - new_start;
            start              = new_start;
        } else
            return (value - ranges[mid].start_value) + ranges[mid].orig_index;
    }
    if (n > 0) {
        unsigned start_orig_index = ranges[start].orig_index;
        unsigned range_size       = ranges[start + 1].orig_index - start_orig_index;

        if (ranges[start].start_value <= value && value < (int)(ranges[start].start_value + range_size)) {
            return (value - ranges[start].start_value) + start_orig_index;
        }
    }
    return -1;
}

const ProtobufCEnumValue*
protobuf_c_enum_descriptor_get_value_by_name(const ProtobufCEnumDescriptor* desc, const char* name)
{
    if (desc == NULL || desc->values_by_name == NULL)
        return NULL;

    unsigned start = 0;
    unsigned count = desc->n_value_names;

    while (count > 1) {
        unsigned mid = start + count / 2;
        int      rv  = strcmp(desc->values_by_name[mid].name, name);
        if (rv == 0)
            return desc->values + desc->values_by_name[mid].index;
        else if (rv < 0) {
            count = start + count - (mid + 1);
            start = mid + 1;
        } else
            count = mid - start;
    }
    if (count == 0)
        return NULL;
    if (strcmp(desc->values_by_name[start].name, name) == 0)
        return desc->values + desc->values_by_name[start].index;
    return NULL;
}

j2p_expt_t
util_cvt_hex_to_bytes(const char* const hex_str, uint8_t** const bytes, size_t* const bytes_len)
{

    assert(hex_str != NULL && bytes != NULL && bytes_len != NULL);

    char* buffer = strdup(hex_str);
    if (NULL == buffer) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    size_t hex_len = strlen(buffer);

    if (hex_len < 2) {
        free(buffer);
        return J2P_EXPT_INVALID_HEX_STRING;
    }

    size_t   b_len   = (hex_len % 3 == 0) ? (hex_len / 3) : (hex_len / 3 + 1);
    uint8_t* b_data  = (uint8_t*)calloc(b_len, sizeof(uint8_t));
    size_t   b_index = 0;

    char* token  = strtok(buffer, " ");
    char* endptr = NULL;
    while (token != NULL) {
        errno              = 0;
        unsigned long byte = strtoul(token, &endptr, 16);
        if (errno != 0 || *endptr != '\0' || byte > UINT8_MAX) {
            break;
        }
        b_data[b_index++] = byte;
        token             = strtok(NULL, " ");
    }

    if (b_index != b_len) {
        free(b_data);
        free(buffer);
        return J2P_EXPT_INVALID_HEX_STRING;
    }

    free(buffer);

    *bytes     = b_data;
    *bytes_len = b_len;

    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
util_cvt_base64_to_bytes(const char* const base64_str, uint8_t** const bytes, size_t* const bytes_len)
{
    assert(base64_str != NULL && bytes != NULL && bytes_len != NULL);

    size_t input_len = strlen(base64_str);
    *bytes           = b64_decode_with_alloc((const uint8_t*)base64_str, input_len, bytes_len);
    if (NULL == *bytes) {
        return J2P_EXPT_INVALID_BASE64_STRING;
    }

    return J2P_EXPT_SUCCESS;
}

j2p_expt_t
util_cvt_file_to_bytes(const char* const file_path, uint8_t** const bytes, size_t* const bytes_len)
{
    assert(file_path != NULL && bytes != NULL && bytes_len != NULL);

    FILE* fp = fopen(file_path, "rb");
    if (NULL == fp) {
        return J2P_EXPT_INVALID_FILE_PATH;
    }

    fseek(fp, 0, SEEK_END);
    const size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t* b_data = (uint8_t*)calloc(size, sizeof(uint8_t));
    if (NULL == b_data) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    if (fread(b_data, sizeof(uint8_t), size, fp) != size) {
        if (ferror(fp) != 0) {
            perror("fread failed");
        }
        free(b_data);
        fclose(fp);
        return J2P_EXPT_INVALID_FILE_PATH;
    } else {
        *bytes     = b_data;
        *bytes_len = size;
        fclose(fp);
        return J2P_EXPT_SUCCESS;
    }
}
