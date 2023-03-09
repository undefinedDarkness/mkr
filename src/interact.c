#include "app.h"
#include "modes/mode.h"
#include <stdio.h>

void ensureVisible(void*);

// can be called quite often, would be good, if it is quick.
void update_selected(int new, APP_MUT) {
  // barriers
  if (new < 0)
    new = 0;
  if (new == app->selectedItem)
    return;

  AUTO list = gtk_container_get_children(app->ui.display);
  // no children
  if (list == NULL)
    return;
  if (new > g_list_length(list))
    new = g_list_length(list) - 1;

  // update old
  AUTO item = g_list_nth_data(list, app->selectedItem);
  gtk_style_context_remove_class(gtk_widget_get_style_context(item),
                                 "selected");

  // update new
  item = g_list_nth_data(list, new);
  CLASS(item, "selected");
  app->selectedItem = new;

  g_idle_add(ensureVisible, item);
  g_list_free(list);
}

static void free_1st(GtkWidget *A, void *B) { gtk_widget_destroy(A); }

// TODO: Something async-y
void fill_display(APP_MUT) {
  GList *list = app->currentMode.generate();
  app->selectedItem = 0;
  AUTO display = app->ui.display;
  GList *children = gtk_container_get_children(display);
  g_list_foreach(children, gtk_widget_destroy, NULL);
  g_list_free(children);

	AUTO favouritesDisplay = app->ui.favourites;
	size_t favouritesLen;
	GError *err = NULL;
	AUTO favouritesLabels = g_key_file_get_string_list(app->config, "FAVOURITES", "APP", &favouritesLen, &err);

	// printf("FAVOURITES %s:\n", err == NULL ? "" : err->message);
	for (int i = 0; i < favouritesLen; i++) {
		printf("%s\n",favouritesLabels[i]);
	}

  while (list->next != NULL) {
    Result *res = list->data;

    // NOTE: Creation of item box is here
    AUTO item = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    g_object_set_data(item, "__resptr", res);

    AUTO label = gtk_label_new(res->label);
    gtk_box_pack_start(item, label, 0, 0, 0);

    AUTO icon = gtk_image_new_from_gicon(res->icon, GTK_ICON_SIZE_BUTTON);
    gtk_image_set_pixel_size(icon, 24);
    gtk_box_pack_start(item, icon, 0, 0, 0);

    list = list->next;
	
	// TODO: Rework this code
	for (int i = 0; i < favouritesLen; i++) {
		if (g_strcmp0(res->label, favouritesLabels[i]) == 0) {
			ADD(favouritesDisplay, item);
			printf("Adding %p to favourites list\n", item);
			break;
		}
	}

	if (gtk_widget_get_parent(item) == NULL)
		ADD(display, item);
    // WARN: Memory leak, list->data should be freed.
  }

  // for 1st label
  CLASS(gtk_container_get_children(display)->data, "selected");
	g_strfreev(favouritesLabels);
  gtk_widget_show_all(display);
  gtk_widget_show_all(favouritesDisplay);
  g_list_free(list);
}

// MUTATES APP!!!
void update_mode(char p, APP_MUT) {
  g_print("MODE: %c\n", p);
  for (int i = 0; i < sizeof(modes) / sizeof(modes[0]); i++) {
    char *label = modes[i].label;
    if (label[0] == p) {
      app->currentMode = modes[i];
      app->selectedItem = 0;
      enum ModeType type = modes[i].type;

      AUTO display = app->ui.display;
      AUTO layout = gtk_widget_get_parent(app->ui.mode);
      AUTO scroll = app->ui.scroll;
      // Clear all children
      AUTO children = gtk_container_get_children(display);
      g_list_foreach(children, gtk_widget_destroy, NULL);
      g_list_free(children);

      gtk_label_set_text(app->ui.mode, label);
      
	  if (type & COMMAND) {
        gtk_widget_hide(scroll);
        gtk_container_remove(layout, app->ui.input);
        gtk_box_pack_end(layout, app->ui.input, true, true, 0);
      } else if (type & ITEMS) {
        // rearrange
		g_object_ref(scroll); // will die without this
        gtk_container_remove(layout, scroll);
        gtk_container_remove(layout, app->ui.input);
        gtk_container_add(layout, app->ui.input);
        gtk_box_pack_start(layout, scroll, true, true, 0);
		g_object_unref(scroll);
       
		gtk_widget_show_all(layout);
		fill_display(app);
      }

	  gtk_widget_grab_focus(app->ui.input);
    }
  }
}
