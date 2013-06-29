#include <stdio.h>

#include "util.h"

size_t fcontent_length(FILE* f)
{
    size_t size;
    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return size;
}
