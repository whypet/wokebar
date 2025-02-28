#ifndef WOKEFONT_H
#define WOKEFONT_H

#include <stddef.h>
#include <stdint.h>

void    font_init(void);
void    font_free(void);
int32_t font_search(const char *family, const char *font_style,
                    double font_size, char *path, size_t path_size);

#endif
