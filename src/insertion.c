#include "app.h"
#include <assert.h>



// bool async_insert_item_single(struct InsertData *i) {
// 	APP = i->ptr;
//    	Result res = i->res; 
// 	// g_print("Adding %s\n", res.label);
//
//     // NOTE: Creation of item box is here
//     AUTO item = gtk_list_box_row_new();
//     g_object_set_data(item, "__resptr", &res);
//     g_object_set_data(item, "__label", g_ascii_strdown(res.label, -1));
//
//     AUTO item_layout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
//     ADD(item, item_layout);
//
//     AUTO label = gtk_label_new(res.label);
//     gtk_label_set_justify(label, GTK_JUSTIFY_LEFT);
//     gtk_label_set_xalign(label, 0);
//
//     AUTO icon = gtk_image_new_from_gicon(res.icon, GTK_ICON_SIZE_BUTTON);
//     gtk_image_set_pixel_size(icon, 48);
//     gtk_box_pack_start(item_layout, icon, 0, 0, 8);
//     gtk_box_pack_start(item_layout, label, true, true, 8);
//
// 	gtk_widget_show_all(item);
//     ADD(app->ui.display, item);
// 	free(i);
// 	return false;
// }

// TODO: Something async-y
void async_insert_item(GList *list, APP) {
  AUTO display = app->ui.display;
  Result *res = list->data;
  assert(res != NULL);
    // g_print("Adding %s\n", res->label);

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
  while (list != NULL && list->next != NULL) {
    res = list->data;
    // g_print("Adding %s\n", res->label);

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
