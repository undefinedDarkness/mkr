#include "upload.h"

GtkWidget* uploadPreview(void*) {
	AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous(layout, true);
	AUTO text = gtk_label_new("Drag and Drop to Upload\n");
	ADD(layout,text);
	return layout;
}
