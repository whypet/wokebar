#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
	char   *server;

	xdisplay_t display;
	xwindow_t  w;

	server = NULL;

	while ((opt = getopt(argc, argv, "hs:")) != -1) {
		switch (opt) {
		case 'h':
			print_help(stdout);
			return EXIT_SUCCESS;
		case 's':
			server = optarg;
			break;
		default:
			print_help(stderr);
			return EXIT_FAILURE;
		}
	}

	xwindow_init_display(&display, server);

	memset(&w, 0, sizeof(w));
	xwindow_create(&w, &display, 25);

	xwindow_loop(&w);
	pause();

	return EXIT_SUCCESS;
}
