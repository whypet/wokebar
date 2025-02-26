#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xproto.h>

#include "xwindow.h"

#define REQUEST_ATOM(c, name)                                                  \
	xcb_intern_atom_reply(c, xcb_intern_atom(c, 0, strlen(#name), #name), NULL)

enum {
	_NET_WM_DESKTOP,
	_NET_WM_STATE,
	_NET_WM_STATE_ABOVE,
	_NET_WM_STATE_STICKY,
	_NET_WM_STRUT,
	_NET_WM_STRUT_PARTIAL,
	_NET_WM_WINDOW_TYPE,
	_NET_WM_WINDOW_TYPE_DOCK
};

typedef struct {
	uint32_t left;
	uint32_t right;
	uint32_t top;
	uint32_t bottom;
	uint32_t left_start_y;
	uint32_t left_end_y;
	uint32_t right_start_y;
	uint32_t right_end_y;
	uint32_t top_start_x;
	uint32_t top_end_x;
	uint32_t bottom_start_x;
	uint32_t bottom_end_x;
} wm_strut_partial;

static const char title[] = "wokebar";

void xwindow_init_display(char *name, xserver_t *server) {
	xcb_screen_iterator_t iter;
	int32_t               screen_index;
	int32_t               i;
	/* todo */
	server->c = xcb_connect(name, &screen_index);
	assert(server->c != NULL);
	iter            = xcb_setup_roots_iterator(xcb_get_setup(server->c));
	server->screens = malloc(sizeof(xcb_screen_t) * iter.rem);

	for (i = 0; iter.rem; screen_index--, i++, xcb_screen_next(&iter)) {
		memcpy(&server->screens[i], iter.data, sizeof(xcb_screen_t));

		printf("\n");
		printf("Informations of display->screen %ld:\n",
		       server->screens[i].root);
		printf("  width.........: %d\n", server->screens[i].width_in_pixels);
		printf("  height........: %d\n", server->screens[i].height_in_pixels);
		printf("  white pixel...: %ld\n", server->screens[i].white_pixel);
		printf("  black pixel...: %ld\n", server->screens[i].black_pixel);
		printf("\n");
	}
}

static void set_properties(xcb_window_t win, xcb_connection_t *c,
                           xcb_screen_t *screen, int32_t height) {
	uint32_t                 i;
	uint32_t                 desktop[1];
	wm_strut_partial         strut;
	xcb_intern_atom_reply_t *replies[] = {
		REQUEST_ATOM(c, _NET_WM_DESKTOP),
		REQUEST_ATOM(c, _NET_WM_STATE),
		REQUEST_ATOM(c, _NET_WM_STATE_ABOVE),
		REQUEST_ATOM(c, _NET_WM_STATE_STICKY),
		REQUEST_ATOM(c, _NET_WM_STRUT),
		REQUEST_ATOM(c, _NET_WM_STRUT_PARTIAL),
		REQUEST_ATOM(c, _NET_WM_WINDOW_TYPE),
		REQUEST_ATOM(c, _NET_WM_WINDOW_TYPE_DOCK),
	};
	xcb_atom_t atoms[sizeof(replies) / sizeof(xcb_intern_atom_reply_t *)];

	for (i = 0; i < sizeof(atoms) / sizeof(xcb_atom_t); i++) {
		if (replies[i] == NULL) {
			atoms[i] = 0;
			fprintf(stderr, "atom reply %d is null!", i);
		} else {
			atoms[i] = replies[i]->atom;
			free(replies[i]);
		}
	}

	desktop[0] = 0xffffffff;

	memset(&strut, 0, sizeof(strut));
	strut.top       = height;
	strut.top_end_x = screen->width_in_pixels - 1;

	xcb_change_property(c, XCB_PROP_MODE_REPLACE, win, XCB_ATOM_WM_NAME,
	                    XCB_ATOM_STRING, 8, strlen(title), title);
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, win, atoms[_NET_WM_DESKTOP],
	                    XCB_ATOM_CARDINAL, 32, 1, desktop);
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, win, atoms[_NET_WM_STATE],
	                    XCB_ATOM_ATOM, 32, 2,
	                    &atoms[_NET_WM_STATE_ABOVE]); // above and sticky
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, win, atoms[_NET_WM_STRUT],
	                    XCB_ATOM_CARDINAL, 32, 4, &strut);
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, win,
	                    atoms[_NET_WM_STRUT_PARTIAL], XCB_ATOM_CARDINAL, 32, 12,
	                    &strut);
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, win,
	                    atoms[_NET_WM_WINDOW_TYPE], XCB_ATOM_ATOM, 32, 1,
	                    &atoms[_NET_WM_WINDOW_TYPE_DOCK]);
}

void xwindow_create(xcb_connection_t *c, xcb_screen_t *screen, int32_t height,
                    xwindow_t *w) {
	xcb_window_t   win;
	xcb_gcontext_t gc;
	uint32_t       override_redirect;
	uint32_t       values[2];
	int32_t        width;

	assert(w != NULL && c != NULL && screen != NULL);

	win               = xcb_generate_id(c);
	override_redirect = 1;
	values[0]         = screen->black_pixel;
	values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY;
	width     = screen->width_in_pixels;

	xcb_create_window(c, XCB_COPY_FROM_PARENT, win, screen->root, 0, 0, width,
	                  height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
	                  screen->root_visual,
	                  XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, values);
	xcb_change_window_attributes(c, win, XCB_CW_OVERRIDE_REDIRECT,
	                             &override_redirect);
	set_properties(win, c, screen, height);
	xcb_map_window(c, win);

	gc = xcb_generate_id(c);
	xcb_create_gc(c, gc, screen->root, XCB_GC_FOREGROUND, values);

	xcb_flush(c);

	printf("window created\n");

	w->c      = c;
	w->screen = screen;
	w->win    = win;
	w->gc     = gc;
	w->height = height;
}

bool xwindow_update(xwindow_t *w) {
	xcb_generic_event_t *e;

	e = xcb_poll_for_event(w->c);
	if (e == NULL)
		return true;

	switch (e->response_type & ~0x80) {
	case XCB_EXPOSE: {
		xcb_expose_event_t *ev = (xcb_expose_event_t *)e;

		xcb_rectangle_t rects[] = {{ev->x, ev->y, ev->width, ev->height}};
		xcb_poly_rectangle(w->c, w->win, w->gc, 1, rects);
		xcb_flush(w->c);

		break;
	}
	}

	free(e);
	return true;
}
