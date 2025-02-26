#ifndef WOKEXWINDOW_H
#define WOKEXWINDOW_H

#include <stdbool.h>
#include <stdint.h>

#include <xcb/xcb.h>

typedef struct {
	xcb_connection_t *c;
	size_t            screen_count;
	xcb_screen_t     *screens;
} xserver_t;

typedef struct {
	xcb_connection_t *c;
	xcb_screen_t     *screen;
	xcb_window_t      win;
	xcb_gcontext_t    gc;
	int32_t           height;
} xwindow_t;

void xwindow_init_display(char *name, xserver_t *server);
void xwindow_create(xcb_connection_t *c, xcb_screen_t *screen, int32_t height,
                    xwindow_t *w);
bool xwindow_update(xwindow_t *w);

#endif
