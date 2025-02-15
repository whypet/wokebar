#include <assert.h>

#include "xwindow.h"

void xwindow_create(xwindow_t *w, xcb_connection_t *c, xcb_screen_t *screen, int x, int y, int _w, int _h) {
	xcb_window_t win;

	assert(c != NULL && w != NULL);

	win = xcb_generate_id(c);
	xcb_create_window(
		c, XCB_COPY_FROM_PARENT, win, screen->root,
		x, y, _w, _h, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
		0, NULL
		);

	xcb_map_window(c, win);

	w->win = win;
}

