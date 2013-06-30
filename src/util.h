#ifndef __UTIL_H__
#define __UTIL_H__

#define READ_LINE_BUFFER 1024

size_t fcontent_length(FILE* f);
char* randstr(char* dest, size_t length);
int copy(FILE *dst, FILE *src);
int copyn(FILE *dst, FILE *src, size_t n);

/* mcef's readline function from https://gist.github.com/mcef/5896303 */
char* read_line(FILE* f);

#endif /* __UTIL_H__ */
