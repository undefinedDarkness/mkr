#include "app.h"
#include <assert.h>

// List of custom widgets
void async_insert_custom_item(GList * list, APP)
{
	AUTO display = app->ui.display;
	const AUTO head = list;
	while (list != NULL) {
		assert(GTK_IS_WIDGET(list->data));
		AUTO row = gtk_list_box_row_new();
		Result *res = g_object_get_data(list->data, "__resptr");
		assert(res != NULL);
		/* printf("Result from custom element: %s %p %p %d\n", res->label, res->metadata, res->icon, res->id); */
		g_object_set_data(row, "__resptr", res);
		g_object_set_data(row, "__label", res->label);
		ADD(row, list->data);
		ADD(display, row);
		list = list->next;
	}
	gtk_widget_show_all(display);
	g_list_free(head);
	printf("Finished adding custom elements\n");
}

// List of results..
void async_insert_item(GList * list, APP)
{
	const AUTO lh = list;
	assert(list != NULL);
	
	AUTO display = app->ui.display;
	gtk_list_box_set_sort_func(display, NULL, NULL, NULL);
	
	Result *res = list->data;
	assert(res != NULL); assert(res->label != NULL);

	AUTO item = gtk_list_box_row_new();
	g_object_set_data(item, "__resptr", res);
	g_object_set_data(item, "__label", g_ascii_strdown(res->label, -1));

	AUTO item_layout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	ADD(item, item_layout);

	AUTO label = gtk_label_new(res->label);
	gtk_label_set_justify(label, GTK_JUSTIFY_LEFT);
	gtk_label_set_xalign(label, 0);

	AUTO icon = gtk_image_new_from_gicon(res->icon, GTK_ICON_SIZE_BUTTON);
	gtk_image_set_pixel_size(icon, 48);
	gtk_box_pack_start(item_layout, icon, 0, 0, 8);
	gtk_box_pack_start(item_layout, label, true, true, 8);

	list = list->next;

	ADD(display, item);
	while (list != NULL) {
		res = list->data;
		// g_print("Adding %s\n", res->label);

		AUTO item = gtk_list_box_row_new();
		g_object_set_data(item, "__resptr", res);
		g_object_set_data(item, "__label",
				  g_ascii_strdown(res->label, -1));

		AUTO item_layout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
		ADD(item, item_layout);

		AUTO label = gtk_label_new(res->label);
		gtk_label_set_justify(label, GTK_JUSTIFY_LEFT);
		gtk_label_set_xalign(label, 0);

		AUTO icon =
		    gtk_image_new_from_gicon(res->icon, GTK_ICON_SIZE_BUTTON);
		gtk_image_set_pixel_size(icon, 48);
		gtk_box_pack_start(item_layout, icon, 0, 0, 8);
		gtk_box_pack_start(item_layout, label, true, true, 8);

		list = list->next;

		ADD(display, item);
	}

	// for 1st label
	gtk_widget_show_all(display);
	gtk_list_box_set_sort_func(display, sort, app, NULL);
	g_list_free(lh);
}
