#include "app.h"

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

 void ensureVisible(GtkWidget *child) {
  if (!is_visible_in_parent(child))
    scroll_to_widget(child);
}


