#include "app.h"
#include "modes/mode.h"
#include <stdio.h>
#include <unistd.h>

// TODO: Something async-y
void fill_display(APP_MUT) {
  GList *list = app->currentMode.generate();
  // app->selectedItem = 0;
  AUTO display = app->ui.display;
// unrolled 1st element
  Result *res = list->data;

    // NOTE: Creation of item box is here
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
	gtk_list_box_select_row(display, item);
	//
  while (list->next != NULL) {
    Result *res = list->data;

    // NOTE: Creation of item box is here
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
  }

  // for 1st label
  gtk_widget_show_all(display);
  g_list_free(list);
}


