#ifndef _XWINDOW_H_
#define _XWINDOW_H_

#include <xcb/xcb.h>

typedef struct {
	xcb_connection_t *c;
	xcb_screen_t     *screen;
} xdisplay_t;

typedef struct {
	xcb_window_t   win;
	xcb_gcontext_t gc;
	xdisplay_t    *display;
	int32_t        height;
} xwindow_t;

void xwindow_init_display(xdisplay_t *display, char *name);
void xwindow_create(xwindow_t *w, xdisplay_t *display, int32_t height);
void xwindow_loop(xwindow_t *w);

#endif
