#include "upload.h"

const static GtkTargetEntry targets[] = {
	{"text/uri-list",0,0}
};

static char buffer[1000];
static GtkWidget* results;

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
		char *url = g_slice_alloc(100);
		int re;
        char *filename = g_filename_from_uri (filenames[iter], NULL, NULL);
		printf("Uploading %s\n", filename);
		sprintf(buffer, "curl -s -F'file=@%s' https://0x0.st", filename);
		g_spawn_command_line_sync(buffer, &url, NULL, &re, NULL);
		printf("%s\n", url);
		AUTO l = gtk_label_new(url);
		gtk_label_set_selectable(l, true);
		ADD(results,l);
		iter++;
    }
	gtk_widget_show_all(results);
    gtk_drag_finish(context, TRUE, FALSE, time); // Drag and drop was successful!
}

GtkWidget* uploadPreview(void* _) {
	AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous(layout, true);

	results = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); CLASS(results,"upload-results-view");
	AUTO sc = gtk_scrolled_window_new(NULL, NULL);
	AUTO text = gtk_label_new("Drag and Drop to Upload\n"); CLASS(results, "upload-drop-view");
	ADD(layout,text);
	ADD(sc,results);

	ADD(layout, sc);
	gtk_drag_dest_set(layout, GTK_DEST_DEFAULT_ALL, NULL, 0, GDK_ACTION_COPY);
	gtk_drag_dest_add_uri_targets(layout);
	g_signal_connect(layout, "drag-data-received", on_drag_data_received, NULL);
	return layout;
}
