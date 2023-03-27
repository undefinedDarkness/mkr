#include "upload.h"

const static GtkTargetEntry targets[] = {
	{"text/uri-list",0,0}
};

static void on_drag_data_received (GtkWidget *wgt, GdkDragContext *context, gint x, gint y, GtkSelectionData *seldata, guint info, guint time, gpointer data) {
    gchar **filenames = NULL;
    filenames = g_uri_list_extract_uris((const gchar *) gtk_selection_data_get_data (seldata));
    if (filenames == NULL) // If unable to retrieve filenames:
    {
        g_printerr("FAILURE!");
        gtk_drag_finish(context, FALSE, FALSE, time); // Drag and drop was a failure.
        return;
    }
    int iter = 0;
    while(filenames[iter] != NULL) // The last URI list element is NULL.
    {
        char *filename = g_filename_from_uri (filenames[iter], NULL, NULL); 
        
		// TODO: Use curl over here...	

		iter++;
    }
    gtk_drag_finish(context, TRUE, FALSE, time); // Drag and drop was successful!
}

GtkWidget* uploadPreview(void*) {
	AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous(layout, true);
	gtk_drag_dest_set(layout, GTK_DEST_DEFAULT_DROP, targets, sizeof(targets)/sizeof(targets[0]), GDK_ACTION_COPY);
	AUTO text = gtk_label_new("Drag and Drop to Upload\n");
	g_signal_connect(layout, "drag-data-recieved", on_drag_data_received, NULL);
	ADD(layout,text);
	return layout;
}
