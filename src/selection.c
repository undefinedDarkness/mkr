#include "app.h"

void update_selected(int offset, APP)
{
	if (app->currentMode.metadata.type & ONLY_PREVIEW) {
		return;
	}

	AUTO display = app->ui.display;
	if (!GTK_IS_LIST_BOX(display)) {
		g_error
		    ("Unexpected widget set for display, expected ListBox got %s",
		     gtk_widget_get_name(display));
	}

	AUTO selected = gtk_list_box_get_selected_row(display);
	const bool isAnySelected = selected != NULL;
	AUTO index = !isAnySelected ? 0 : gtk_list_box_row_get_index(selected);

	int nindex = index + offset;
	if (offset == 0xAA || index + offset < 0) // What the fuck am I reading?
		nindex = 0;
	AUTO new = gtk_list_box_get_row_at_index(display, nindex);
	gtk_widget_grab_focus(new);
	if (isAnySelected)
		gtk_list_box_unselect_row(display, selected);
	gtk_list_box_select_row(display, new);

	// TODO: Let mode do this async if preview takes a while eg: pdf
	if (app->currentMode.metadata.type & HAS_PREVIEW)
		display_preview(NULL, new, app);
}

void display_preview(GtkWidget * listbox, GtkWidget * row, APP)
{
	Result *res = g_object_get_data(row, "__resptr");
	GtkWidget *child = gtk_bin_get_child(app->ui.preview);
	if (child != NULL)
		gtk_widget_destroy(child);
	gtk_container_add(app->ui.preview, app->currentMode.preview(res));
	gtk_widget_show_all(app->ui.preview);
}
