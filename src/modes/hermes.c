// vim:fdm=marker:
#include "hermes.h"

GtkWidget *hermes_preview(void *_) {
  AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  CLASS(layout, "hermes-view");

  /*   GtkWidget *frame = gtk_frame_new(""); */
  /*   gtk_frame_set_shadow_type(frame, GTK_SHADOW_NONE); */
  /* 	AUTO openWindows = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16); */
  /*   getWindows(openWindows); */
  /*   CLASS(frame, "hermes-open-windows"); */
  /*   ADD(frame, openWindows); */
  /* ADD(layout, frame); */

  return layout;
}
