#include <stdio.h>
#include <stdlib.h>

#include "util.h"

size_t fcontent_length(FILE* f)
{
    size_t size;
    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return size;
}

char* randstr(char* dest, size_t length)
{
    int index = 0;
    int r;

    while (index < length) {
        r = rand() % 122;
        if ((r >= 'a' && r <= 'z') || 
            (r >= 'A' && r <= 'Z') || 
            (r >= '0' && r <= '9')) {
            dest[index] = r;
            index++;
        }
    }
    dest[length] = 0;
    return dest;
}
