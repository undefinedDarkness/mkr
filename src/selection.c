#include "app.h"

void update_selected(int offset, APP) {
	AUTO display = app->ui.display;
	if (!GTK_IS_LIST_BOX(display)) {
		g_error("Unexpected widget set for display, expected ListBox got %s", gtk_widget_get_name(display));
	}

	AUTO selected = gtk_list_box_get_selected_row(display);
	AUTO index = gtk_list_box_row_get_index(selected);


	int nindex = index+offset;
	if (offset == 0xAA || index+offset < 0)
		nindex = 0;	
	AUTO new = gtk_list_box_get_row_at_index(display, nindex);
	gtk_widget_grab_focus(new);
	gtk_list_box_unselect_row(display, selected);
	gtk_list_box_select_row(display, new);

	// gtk_widget_grab_focus(app->ui.input);

	// g_object_unref(selected);
	// g_object_unref(new);
}
