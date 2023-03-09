#ifndef APP_H
#define APP_H

#include "basic.h"
#include "modes/mode.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *input;
    GtkWidget *display;
	GtkWidget *mode;
	GtkWidget *scroll;
	GtkWidget *favourites;
} UI;

typedef struct {
	UI ui;
	GKeyFile *config;
	Mode currentMode;
	int selectedItem;
} State;

#define APP const State *const app
#define APP_MUT State *const app
void scaffold(GtkWidget*, APP_MUT);
void fill_display(APP_MUT); 

#define C_GET(g, k, type, default) g_key_file_has_key(app->config, g, k, NULL) ? g_key_file_get_##type(app->config, g, k, NULL) : default

#define CLASS(w, c) gtk_style_context_add_class(gtk_widget_get_style_context(w), c);
#define ADD(p, c) gtk_container_add(GTK_CONTAINER(p), c);
#define QUIT g_print("%s:%d is quitting\n", __FILE__, __LINE__);gtk_main_quit

#endif
