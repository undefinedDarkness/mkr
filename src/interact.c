#include "app.h"
#include "modes/mode.h"

// Expecting following structure:
// ScrolledWindow
// 	-> Viewport
// 		-> Container (ANY)
// 			-> Child
static bool is_visible_in_parent(GtkWidget *child) {
  AUTO scrolled = gtk_widget_get_parent(
      gtk_widget_get_parent(gtk_widget_get_parent(child)));
  int x, y;
  GtkAllocation child_alloc, scroll_alloc;
  gtk_widget_translate_coordinates(child, scrolled, 0, 0, &x, &y);
  gtk_widget_get_allocation(child, &child_alloc);
  gtk_widget_get_allocation(scrolled, &scroll_alloc);

  return (x >= 0 && y >= 0) && x + child_alloc.width <= scroll_alloc.width &&
         y + child_alloc.height <= scroll_alloc.height;
}

static void scroll_to_widget(GtkWidget *focused) {
  AUTO container = gtk_widget_get_parent(focused);
  AUTO scrolled = gtk_widget_get_parent(gtk_widget_get_parent(container));

  GtkAdjustment *hadj =
      gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(scrolled));
  gint x, y;
  gtk_widget_translate_coordinates(focused, container, 0, 0, &x, &y);
  gtk_adjustment_set_value(hadj, MIN(x, gtk_adjustment_get_upper(hadj)));
}

static void ensureVisible(GtkWidget *child) {
  if (!is_visible_in_parent(child))
    scroll_to_widget(child);
}

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

    ADD(display, item);
    // WARN: Memory leak, list->data should be freed.
    list = list->next;
  }

  // for 1st label
  CLASS(gtk_container_get_children(display)->data, "selected");

  gtk_widget_show_all(display);
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
      AUTO scroll = gtk_widget_get_parent(gtk_widget_get_parent(display));
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
