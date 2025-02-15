#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xwindow.h"

void print_help(FILE *stream) {
	fprintf(stream,
		"wokebar\n"
		"\n"
		"options\n"
		"-h   shows this screen\n"
		"-s   uses specified X server display name instead of\n"
		"     DISPLAY environment variable\n"
		);
}

int main(int argc, char *argv[]) {
	int   opt;
	char *server;

	xcb_connection_t *c;
	xcb_screen_t     *screen;
	xwindow_t         w;

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

	c = xcb_connect(server, NULL);
	screen = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

	memset(&w, 0, sizeof(w));
	xwindow_create(&w, c, screen, 0, 0, 300, 300);

	xcb_flush(c);

	pause();

	return EXIT_SUCCESS;
}

