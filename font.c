
#include <fontconfig/fontconfig.h>

#include "font.h"

void    font_init(void) { FcInit(); }
void    font_free(void) { FcFini(); }
int32_t font_search(const char *name, const char *font_style, double font_size,
                    char *path, size_t path_size) {}
