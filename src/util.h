#ifndef __UTIL_H__
#define __UTIL_H__

size_t fcontent_length(FILE* f);
char* rstrstr(const char* haystack, const char* needle);
char* randstr(char* dest, size_t length);

#endif /* __UTIL_H__ */
