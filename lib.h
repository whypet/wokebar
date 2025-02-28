#ifndef WOKELIB_H
#define WOKELIB_H

#include <stddef.h>

#if _POSIX_C_SOURCE < 200809L && !defined(_GNU_SOURCE)
size_t strnlen(const char *s, size_t n);
#endif

#endif
