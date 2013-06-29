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

char* rstrstr(const char* haystack, const char* needle)
{
    char* pt1, pt2;
    
    pt1 = strstr(haystack, needle);
    do {
        pt2 = pt1;
        pt1 = strstr(pt1, needle);
    } while (pt1 != NULL);
    return pt2;
}
