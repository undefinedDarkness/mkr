#include "hermes.h"
#include "../../lib/ewmh.c"

// {{{
static void getRawIcon(Display *display, Window window) {
  unsigned int *width, *height;
  Atom ret_type;
  int ret_format;
  unsigned long ret_offset;
  unsigned long nitems;
  Atom _NET_WM_ICON = XInternAtom(display, "_NET_WM_ICON", true);

  XGetWindowProperty(display, window, _NET_WM_ICON, 0, 1, false, XA_CARDINAL,
                     &ret_type, &ret_format, &nitems, &ret_offset,
                     (unsigned char **)&width);

  if (ret_type == None)
    return;

  XGetWindowProperty(display, window, _NET_WM_ICON, 1, 1, false, XA_CARDINAL,
                     &ret_type, &ret_format, &nitems, &ret_offset,
                     (unsigned char **)&height);

  size_t iconSize = *width * *height;
  unsigned char *ret;

  XGetWindowProperty(display, window, _NET_WM_ICON, 2, iconSize, false,
                     XA_CARDINAL, &ret_type, &ret_format, &nitems, &ret_offset,
                     (unsigned char **)&ret);

  // ARGB Data..???
}
// }}}

void activateWindowOnClick(GtkWidget *g, Window win) {
  printf("%lu %p\n", win, gtk_widget_get_parent(g));
  Display *disp = g_object_get_data(gtk_widget_get_parent(g), "__xdisplay");

  ewmhActivateWindow(disp, win);
}

void getWindows(GtkWidget *box) {
  Display *display = XOpenDisplay(NULL);

  /* AUTO _NET_CLIENT_LIST = XInternAtom(display, "_NET_CLIENT_LIST", true); */
  AUTO _NET_WM_NAME = XInternAtom(display, "_NET_WM_NAME", true);

  unsigned long nitems;
  Window *items;
  ewmhGetProperty(display, DefaultRootWindow(display), "_NET_CLIENT_LIST", 0,
                  1024, XA_WINDOW, &nitems, (unsigned char **)&items);

  // skip 1st window since its our own...
  for (int i = 1; i < nitems; i++) {
    Window win = ((Window *)items)[i];
    XTextProperty title;
    XGetTextProperty(display, win, &title, _NET_WM_NAME);

    XClassHint class;
    XGetClassHint(display, win, &class);

    AUTO btn = gtk_button_new_from_icon_name(
        g_ascii_strdown(class.res_class, -1),
        GTK_ICON_SIZE_DND); // gtk_button_new_with_label(title.value);
    g_signal_connect(btn, "clicked", activateWindowOnClick, (void *)win);
    ADD(box, btn);
    printf("%lu - %s\n", win, title.value);
  }
  g_object_set_data(box, "__xdisplay", display);
  /* XCloseDisplay(display); */
}

GtkWidget *hermes_preview(void *_) {
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
