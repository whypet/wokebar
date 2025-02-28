#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <toml.h>

#include "conf.h"
#include "lib.h"

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

static const char default_config[] =
	"title = \"wokebar\""
	"\n"
	"\n[text]"
	"\nfamily = \"Fixed\""
	"\nfont_style = \"Regular\""
	"\n# Set font_size (pt) to 0.0 to use pixel_size (px) instead."
	"\nfont_size = 0.0"
	"\npixel_size = 13.0"
	"\n"
	"\n[color]"
	"\nback = 0x000000"
	"\nfore = 0xffffff"
	"\n";

static size_t get_xdg_config_home(char *restrict s, size_t n) {
	const char *env_path, *home;
	size_t      len;

	env_path = getenv("XDG_CONFIG_HOME");
	if (env_path != NULL && env_path[0] != '\0') {
		len = strnlen(env_path, n);
		if (len >= n)
			return len + 1;

		assert(s != NULL);
		*s = '\0';
		strncat(s, env_path, n);

		return len;
	}

	home = getenv("HOME");
	if (home == NULL || home[0] == '\0') {
		fprintf(stderr, "environment variable HOME is unset\n");
		return 0;
	} else {
		static const char rel_xdg_config[] = ".config";

		len = snprintf(NULL, 0, "%s/%s", home, rel_xdg_config);
		if (len >= n)
			return len + 1;

		assert(s != NULL);
		len = snprintf(s, n, "%s/%s", home, rel_xdg_config);

		return len;
	}
}

FILE *conf_open(void) {
	static char rel_config_path[] = "wokebar.toml";

	char *config_path;
	FILE *file;

	{
		char  *config_home;
		size_t res;
		size_t n;

		config_home = NULL;
		res         = 0;

		do {
			n = res;
			if (config_home != NULL)
				free(config_home);
			if (n > 0)
				config_home = malloc(n);
			res = get_xdg_config_home(config_home, n);
		} while (res > n && res != 0);

		if (res == 0)
			return NULL;

		n           = snprintf(NULL, 0, "%s/%s", config_home, rel_config_path);
		config_path = malloc(n + 1);
		snprintf(config_path, n + 1, "%s/%s", config_home, rel_config_path);

		free(config_home);
	}

start:
	file = fopen(config_path, "r");
	if (file != NULL)
		goto end;

	switch (errno) {
	case EACCES:
		fprintf(stderr,
		        "couldn't read config file at '%s': permission denied\n",
		        config_path);
		break;
	case ENOENT:
		file = fopen(config_path, "w");

		if (file == NULL) {
			fprintf(stderr, "couldn't create config file at '%s'\n",
			        config_path);
		} else if (fputs(default_config, file) < 0) {
			fprintf(stderr, "couldn't write default config file\n");
			fclose(file);
			file = NULL;
		} else {
			fclose(file);
			goto start;
		}
		break;
	}
end:
	free(config_path);
	return file;
}

bool conf_read(FILE *file, config_t *config) {
	static char   errbuf[256];
	toml_table_t *table;

	memset(errbuf, 0, sizeof(errbuf));
	table = toml_parse_file(file, errbuf, sizeof(errbuf));

	if (table == NULL) {
		fprintf(stderr, "couldn't read toml config: %s\n", errbuf);
		return false;
	}

	CONF_SET_VALUE(config->title, table, string, "title", "title");

	CONF_IN_TABLE(table, table_text, "text", {
		CONF_SET_VALUE(config->text.family, table_text, string, "family",
		               "text.family");
		CONF_SET_VALUE(config->text.font_style, table_text, string,
		               "font_style", "text.font_style");
		CONF_SET_VALUE(config->text.font_size, table_text, double, "font_size",
		               "text.font_size");
		CONF_SET_VALUE(config->text.pixel_size, table_text, double,
		               "pixel_size", "text.pixel_size");
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
