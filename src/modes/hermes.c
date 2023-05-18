// vim:fdm=marker:
#include "hermes.h"



GtkWidget *hermes_preview(void *_) {
  AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  CLASS(layout, "hermes-view");

	

  return layout;
}
