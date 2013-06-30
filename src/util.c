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
