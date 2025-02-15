#ifndef _XWINDOW_H_
#define _XWINDOW_H_

#include <xcb/xcb.h>

typedef struct {
	xcb_window_t win;
} xwindow_t;

void xwindow_create(xwindow_t *w, xcb_connection_t *c, xcb_screen_t *screen, int x, int y, int _w, int _h);

#endif

