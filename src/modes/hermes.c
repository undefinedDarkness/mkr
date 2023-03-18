#include "hermes.h"
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

static void getRawIcon(Display* display, Window window) {
	unsigned int *width, *height;
	Atom ret_type;
	int ret_format;
	unsigned long ret_offset;
	unsigned long nitems;
	Atom _NET_WM_ICON = XInternAtom(display, "_NET_WM_ICON", true);

	XGetWindowProperty(display, window, _NET_WM_ICON, 0, 1, false, XA_CARDINAL,
			&ret_type, &ret_format, &nitems, &ret_offset, (unsigned char**)&width);

	if (ret_type == None)
		return;

	XGetWindowProperty(display, window, _NET_WM_ICON, 1, 1, false, XA_CARDINAL,
			&ret_type, &ret_format, &nitems, &ret_offset, (unsigned char**)&height);

	size_t iconSize = *width**height;
	unsigned char *ret;

	XGetWindowProperty(display, window, _NET_WM_ICON, 2, iconSize, false, XA_CARDINAL,
			&ret_type, &ret_format, &nitems, &ret_offset, (unsigned char**)&ret);

	// ARGB Data..??? 
}

void getWindows(GtkWidget*box) {
	AUTO display = XOpenDisplay(NULL);
	
	AUTO _NET_CLIENT_LIST = XInternAtom(display, "_NET_CLIENT_LIST", true);
	AUTO _NET_WM_NAME = XInternAtom(display, "_NET_WM_NAME", true);

	Atom ret_type;
	int ret_format;
	unsigned long nitems;
	unsigned long ret_offset;
	unsigned char *items;
	XGetWindowProperty(display, DefaultRootWindow(display), _NET_CLIENT_LIST, 0, 1024, false, XA_WINDOW,
			&ret_type, &ret_format, &nitems, &ret_offset, &items);

	// skip 1st window since its our own...
	for (int i = 1; i < nitems; i++) {
		Window win = ((Window*)items)[i];
		XTextProperty title;
		XGetTextProperty(display, win, &title, _NET_WM_NAME);

		XClassHint class;
		XGetClassHint(display, win, &class);
		// printf("%s %s\n", class.res_class, class.res_name);
	
		AUTO btn = gtk_button_new_from_icon_name(g_ascii_strdown(class.res_class, -1), GTK_ICON_SIZE_DND);//gtk_button_new_with_label(title.value);
		ADD(box, btn);
	}

}

GtkWidget* hermes_preview(void*) {
	AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	CLASS(layout, "hermes-view");

	GtkWidget *frame = gtk_frame_new("");
	gtk_frame_set_shadow_type(frame, GTK_SHADOW_NONE);
	AUTO openWindows = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
	getWindows(openWindows);
	CLASS(frame, "hermes-open-windows");
	ADD(frame, openWindows);
	ADD(layout, frame);

	return layout;
}
