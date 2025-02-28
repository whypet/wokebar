#include <string.h>

#include "lib.h"

#if _POSIX_C_SOURCE < 200809L && !defined(_GNU_SOURCE)
// https://git.musl-libc.org/cgit/musl/commit/src/string/strnlen.c?id=0b44a0315b47dd8eced9f3b7f31580cf14bbfc01
size_t strnlen(const char *s, size_t n) {
	const char *p = memchr(s, 0, n);
	return p ? p - s : n;
}
#endif
