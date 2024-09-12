/**
 * @file utilities.h
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @date 2024-07-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdint.h>

int write2File(const char* filepath, const char* msg);

int write2File_s(const char* filepath, const char* msg, const int len);

int readFromFile(const char* file_name, char** data);

void print_uint8_in_hex(const uint8_t* data, const int len);

#endif /* UTILITIES_H */