#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "font.h"
#include "xwindow.h"

void print_help(FILE *stream) {
	fprintf(stream, "wokebar\n"
	                "\n"
	                "options\n"
	                "-h   shows this screen\n"
	                "-s   uses specified X server display name instead of\n"
	                "     DISPLAY environment variable\n");
}

int main(int argc, char *argv[]) {
	int32_t opt;
	char   *servername;

	xserver_t server;
	xwindow_t w;

	servername = NULL;

	while ((opt = getopt(argc, argv, "hs:")) != -1) {
		switch (opt) {
		case 'h':
			print_help(stdout);
			return EXIT_SUCCESS;
		case 's':
			servername = optarg;
			break;
		default:
			print_help(stderr);
			return EXIT_FAILURE;
		}
	}

	xwindow_init_display(servername, &server);
	font_init();

	memset(&w, 0, sizeof(w));
	xwindow_create(server.c, &server.screens[0], 25, &w);

	while (xwindow_update(&w))
		;

	font_free();

	return EXIT_SUCCESS;
}
