#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 
/* https://gist.github.com/mcef/5896303 */
char *read_line(FILE *f)
{
    char *line, *l;
    size_t linelen;
    char buf[READ_LINE_BUFFER];
    size_t len;
     
    line = NULL;
    linelen = 0;
       
    while (!feof(f) && !ferror(f)) {
        if (!fgets(buf, (READ_LINE_BUFFER), f)) break;
        len = strlen(buf);
        l = realloc(line, linelen + len + 1);
        if (!l) {
            if (line) free(line);
            return NULL;
        }
        line = l;
        strcpy(line+linelen, buf);
        linelen += len;
        if (line[linelen-1] == '\n') {
            break;
        }
    }
    return line;
 }

int copy(FILE *dst, FILE *src)
{
    char buf[1024*4];
    int r, w;
    int c = 0;

    while ((r = fread(buf, 1, 1024*4, src)) > 0) {
        w = fwrite(buf, 1, r, dst);
        c += w;
        if (r != w) break;
    }

    return c;
}

int copyn(FILE *dst, FILE *src, size_t n)
{
    char buf[1024*4];
    int r, w;
    int to_read = n<1024*4 ? n : 1024*4;
    int c = 0;

    while (n > 0 && (r = fread(buf, 1, to_read, src)) > 0) {
        w = fwrite(buf, 1, r, dst);
        c += w;
        n -= w;
        to_read = n<1024*4 ? n : 1024*4;
        if (r != w) break;
    }

    return c;
}

uint32_t djb2(const char* str)
{
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
