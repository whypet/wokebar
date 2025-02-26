#include <string.h>

#include <toml.h>

#include "conf.h"

#define CONF_string s
#define CONF_bool b
#define CONF_int i
#define CONF_double d
#define CONF_timestamp ts

#define CONF_SET_VALUE(config_field, table, type, prop, formal_name)           \
	{                                                                          \
		toml_value_t value;                                                    \
                                                                               \
		value = toml_table_##type(table, prop);                                \
		if (value.ok) {                                                        \
			(config_field) = value.u.CONF_##type;                              \
		} else {                                                               \
			fprintf(stderr, "warning: couldn't read config property '%s'\n",   \
			        formal_name);                                              \
		}                                                                      \
	}

#define CONF_IN_TABLE(table, var, key, unit)                                   \
	{                                                                          \
		toml_table_t *var;                                                     \
		var = toml_table_table(table, key);                                    \
		if (var == NULL) {                                                     \
			fprintf(stderr, "warning: couldn't read config table '%s'\n",      \
			        key);                                                      \
		} else                                                                 \
			unit;                                                              \
	}

bool conf_read(FILE *file, config_t *config) {
	char          errbuf[256];
	toml_table_t *table;

	memset(errbuf, 0, sizeof(errbuf));
	table = toml_parse_file(file, errbuf, sizeof(errbuf));

	if (table == NULL) {
		fprintf(stderr, "couldn't parse toml file: %s\n", errbuf);
		return false;
	}

	CONF_SET_VALUE(config->title, table, string, "title", "title");

	CONF_IN_TABLE(table, table_text, "text", {
		CONF_SET_VALUE(config->text.font, table_text, string, "font",
		               "text.font");
		CONF_SET_VALUE(config->text.font_style, table_text, string,
		               "font_style", "text.font_style");
		CONF_SET_VALUE(config->text.font_size, table_text, double, "font_size",
		               "text.font_size");
	});

	CONF_IN_TABLE(table, table_color, "color", {
		CONF_SET_VALUE(config->color.back, table_color, int, "back",
		               "color.back");
		CONF_SET_VALUE(config->color.fore, table_color, int, "fore",
		               "color.fore");
	});

	toml_free(table);
	return true;
}
