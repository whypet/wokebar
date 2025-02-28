
#include <fontconfig/fontconfig.h>

#include "font.h"

void font_init(void) { FcInit(); }
void font_free(void) { FcFini(); }

int32_t font_search(const char *family, const char *font_style,
                    double font_size, char *path, size_t path_size) {
	FcPattern *pattern;

	pattern = FcPatternCreate();
	FcPatternAddString(pattern, FC_FAMILY, (const FcChar8 *)family);
	FcPatternAddString(pattern, FC_STYLE, (const FcChar8 *)font_style);
	FcPatternAddDouble(pattern, FC_SIZE, font_size);

	return 0;
}
