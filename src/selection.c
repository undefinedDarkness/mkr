#include "app.h"

void update_selected(int offset, APP)
{
	if (app->currentMode.metadata.type & ONLY_PREVIEW) {
		return;
	}

	AUTO display = app->ui.display;
	if (!GTK_IS_FLOW_BOX(display)) {
		g_error
		    ("Unexpected widget set for display, expected FlowBox got %s",
		     gtk_widget_get_name(display));
	}

	GtkWidget* selected = NULL; // = gtk_flow_box_get_selected_children(display)->data;
	bool isAnySelected;// = selected != NULL;
	
	AUTO selectedChild = g_list_first(gtk_flow_box_get_selected_children(display));
	if (selectedChild != NULL) {
		selected = selectedChild->data;
		isAnySelected = true;
	} else {
		isAnySelected = false;
	}

	AUTO head = gtk_container_get_children(display);
	AUTO children = head;
	AUTO index = !isAnySelected ? 0 : g_list_index(children, selected); // (long)g_object_get_data(selected, "__index_in_list");// gtk_flow_box_get_index(selected);

	int nindex = index + offset;
	if (offset == 0xAA || index + offset < 0) // What the fuck am I reading?
		nindex = 0;
	AUTO new = (GtkWidget*)g_list_nth_data(children, nindex); 
	gtk_widget_grab_focus(new);
	if (isAnySelected)
		gtk_flow_box_unselect_child(display, selected);
	gtk_flow_box_select_child(display, new);

	// TODO: Let mode do this async if preview takes a while eg: pdf
	if (app->currentMode.metadata.type & HAS_PREVIEW)
		display_preview(NULL, new, app);

	g_list_free(head);
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
