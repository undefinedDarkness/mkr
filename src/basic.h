#ifndef BASIC_H
#define BASIC_H

#include <gtk/gtk.h>
//#include <stdbool.h>
typedef gboolean bool;
#define true 1
#define false 0
#define AUTO __auto_type
#define C_GET(g, k, type, default) g_key_file_has_key(app->config, g, k, NULL) ? g_key_file_get_##type(app->config, g, k, NULL) : default
#define CLASS(w, c) gtk_style_context_add_class(gtk_widget_get_style_context(w), c);
#define ADD(p, c) gtk_container_add(GTK_CONTAINER(p), c);
#define QUIT g_print("%s:%d is quitting\n", __FILE__, __LINE__);gtk_main_quit



#endif
