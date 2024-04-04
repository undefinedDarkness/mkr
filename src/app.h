#ifndef APP_H
#define APP_H

#include "basic.h"
#include "modes/mode.h"

typedef struct {
    const GtkWidget *restrict window;
    const GtkWidget *restrict input;
    const GtkWidget *restrict display;
	const GtkWidget *restrict mode;
	const GtkWidget *restrict preview;
	const GtkWidget *restrict scroll;
	// GtkWidget *favourites;
} UI;

typedef struct {
	UI ui;
	const GKeyFile *restrict config;
	Mode currentMode;
	int selectedItem;
	 Api_t api;
	char*restrict search;
} State;

#define APP const State *const app
#define APP_MUT State *const app

void async_update_progress(float v, void *ptr);
void modeInit(Mode, APP_MUT);
void execute(APP);
int sort(GtkWidget*a,GtkWidget*b,APP);
void scaffold(GtkWidget*, APP_MUT);
void display_preview(GtkWidget *listbox, GtkWidget *row, APP);

bool toggle_display(void* __x);

// void async_insert_item_single(struct { Result res; APP;  }*i);
void async_insert_item(GList* list, APP);
void async_insert_custom_item(GList *list, APP);
// void sort_display(APP_MUT);
void update_selected(int new, APP);

#define C_GET(g, k, type, default) (g_key_file_has_key(app->config, g, k, NULL) ? g_key_file_get_##type(app->config, g, k, NULL) : default)

#define CLASS(w, c) gtk_style_context_add_class(gtk_widget_get_style_context(w), c);
#define QUIT g_print("%s:%d is quitting\n", __FILE__, __LINE__);gtk_main_quit


#endif
