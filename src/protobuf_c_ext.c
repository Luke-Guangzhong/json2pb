#include <string.h>

#include "cjson2pb-c.h"

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
protobuf_c_enum_descriptor_get_value_by_name_case_insensitive(const ProtobufCEnumDescriptor* desc, const char* name)
{
    if (desc == NULL || desc->values_by_name == NULL)
        return NULL;

    unsigned start = 0;
    unsigned count = desc->n_value_names;

    while (count > 1) {
        unsigned mid = start + count / 2;
        int      rv  = strcasecmp(desc->values_by_name[mid].name, name);
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
    if (strcasecmp(desc->values_by_name[start].name, name) == 0)
        return desc->values + desc->values_by_name[start].index;
    return NULL;
}