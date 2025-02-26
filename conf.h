#ifndef WOKECONF_H
#define WOKECONF_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
	char *title;
	struct {
		char  *font;
		char  *font_style;
		double font_size;
	} text;
	struct {
		uint32_t back;
		uint32_t fore;
	} color;
} config_t;

bool conf_read(FILE *file, config_t *config);

#endif
