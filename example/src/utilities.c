#include "utilities.h"
#include "common.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>

int res_max_length = 4 * 1024; // 4KB

int
write2File(const char* filepath, const char* msg)
{
    int rtn = -2;
    if (filepath == NULL || strlen(filepath) <= 0) {
        DEBUG("[ERROR] filepath empty!(%s)\n", __FUNCTION__);
        return rtn;
    }
    if (msg == NULL || strlen(msg) <= 0) {
        DEBUG("[ERROR] message empty!(%s)\n", __FUNCTION__);
        return rtn;
    }

    FILE* fp = fopen(filepath, "w");
    if (fp == NULL) {
        DEBUG("[ERROR] file open err(filepath=%s)\n", filepath);
        return rtn;
    }

    rtn = fputs(msg, fp);
    if (rtn < 0) {
        DEBUG("[ERROR] fputs err (%s)\n", __FUNCTION__);
    }

    fclose(fp);
    return rtn;
}

int
write2File_s(const char* filepath, const char* msg, const int len)
{
    int   rtn = -2;
    FILE* fp  = NULL;
    if (filepath == NULL || strlen(filepath) <= 0) {
        DEBUG("[ERROR] filepath empty!(%s)\n", __FUNCTION__);
        return rtn;
    }
    if (msg == NULL || strlen(msg) <= 0) {
        DEBUG("[ERROR] message empty!(%s)\n", __FUNCTION__);
        return rtn;
    }

    fp = fopen(filepath, "w");
    if (fp == NULL) {
        DEBUG("[ERROR] file open err(filepath=%s)\n", filepath);
        return rtn;
    }

    rtn = fwrite(msg, len, 1, fp);
    if (rtn < 0) {
        DEBUG("[ERROR] fputs err (%s)\n", __FUNCTION__);
    }

    fclose(fp);
    return rtn;
}

int
readFromFile(const char* file_name, char** data)
{
    int   ch_count = 0;
    FILE* fp       = fopen(file_name, "r+");

    if (fp == NULL) {
        DEBUG("Failed to open file %s (errno %d)\n", file_name, errno);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    ch_count = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    *data = (char*)malloc(sizeof(char) * (ch_count + 1));

    fread(*data, 1, ch_count, fp);
    (*data)[ch_count] = '\0';

    FCLOSE(fp);
    return 0;
}

void
print_uint8_in_hex(const uint8_t* data, const int len)
{
    for (int i = 1; i <= len; i++) {
        printf("%02x", data[i - 1]);
        if ((i % 64) == 0)
            printf("\n");
    }
    printf("\n");
}