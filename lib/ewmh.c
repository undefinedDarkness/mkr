#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

__always_inline static void ewmhGetProperty(Display *display, Window win,
                                        const char *atom, int index32,
                                        size_t size, Atom ret_type,
                                        unsigned long *ret_size,
                                        unsigned char **ret) {
  unsigned long ret_bytes_remaining;
  Atom ret_type_a;
  int ret_format_a;
  XGetWindowProperty(display, win, XInternAtom(display, atom, true), index32,
                     size, false, ret_type, &ret_type_a, &ret_format_a,
                     ret_size, &ret_bytes_remaining, ret);
}



static int ewmhClientMessage(Display *disp, Window win, char *msg, 
        unsigned long data0, unsigned long data1, 
        unsigned long data2, unsigned long data3,
        unsigned long data4) {
    XEvent event;
    long mask = SubstructureRedirectMask | SubstructureNotifyMask;

    event.xclient.type = ClientMessage;
    event.xclient.serial = 0;
    event.xclient.send_event = True;
    event.xclient.message_type = XInternAtom(disp, msg, False);
    event.xclient.window = win;
    event.xclient.format = 32;
    event.xclient.data.l[0] = data0;
    event.xclient.data.l[1] = data1;
    event.xclient.data.l[2] = data2;
    event.xclient.data.l[3] = data3;
    event.xclient.data.l[4] = data4;
    
    if (XSendEvent(disp, DefaultRootWindow(disp), False, mask, &event)) {
        return EXIT_SUCCESS;
    }
    else {
        fprintf(stderr, "Cannot send %s event.\n", msg);
        return EXIT_FAILURE;
    }
}

static void ewmhActivateWindow(Display*disp,Window win) {
  unsigned long nitems;
  unsigned char *value;
  ewmhGetProperty(disp, win, "_NET_WM_DESKTOP", 0, 1, XA_CARDINAL, &nitems, &value);
  unsigned int desktop = *value;

  // move to that desktop
  ewmhClientMessage(disp, DefaultRootWindow(disp), "_NET_CURRENT_DESKTOP", desktop, 0,
             0, 0, 0);

  ewmhClientMessage(disp, win, "_NET_CURRENT_WINDOW", 1, CurrentTime, 0, 0, 0);
  XFlush(disp);

}
