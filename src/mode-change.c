#include "app.h"

static void modeItems(APP_MUT) {
	AUTO display = gtk_list_box_new();
	gtk_list_box_set_sort_func(display, sort, app, NULL);
	gtk_list_box_set_selection_mode(display, GTK_SELECTION_SINGLE);
	CLASS(display, "display");
	// g_print("Adding child to scroll!!\n");
	ADD(app->ui.scroll, display);
	CLASS(app->ui.scroll, "items");
	app->ui.display = display;
	fill_display(app);
	gtk_widget_show(app->ui.scroll);
}

static void modeCommand(APP_MUT) {
	gtk_style_context_remove_class(gtk_widget_get_style_context(app->ui.scroll), "items");
	gtk_widget_hide(app->ui.scroll);
	gtk_window_resize(app->ui.window, 1, 1);

	if (app->currentMode.metadata.type & PROGRESS) {
		
	}
}

static void updateModeLabel(APP) {
	gtk_label_set_text(app->ui.mode, app->currentMode.metadata.symbol);
	AUTO modeStyleCtx = gtk_widget_get_style_context(app->ui.mode);
	for (int i = 0; i < sizeof(modes)/sizeof(modes[0]); i++)
		gtk_style_context_remove_class(modeStyleCtx, modes[i].label);
	gtk_style_context_add_class(modeStyleCtx, app->currentMode.label);
}

void modeInit(APP_MUT) {
	// g_print("Changing mode to %s\n", app->currentMode.label);
	if (GTK_IS_WIDGET(app->ui.display)) {
		gtk_container_remove(app->ui.scroll, app->ui.display);
		// No references to app->ui.display anymore so itll die 
		app->ui.display = NULL;
	}

	enum ModeType type = app->currentMode.metadata.type;

	if (type & ITEMS) {
		// g_print("Mode is of type items\n");
		modeItems(app);
		
	} else if (type & COMMAND) {
		// g_print("Mode is of type command\n");
		modeCommand(app);
	}
	// g_print("Finished init code\n");
	g_idle_add(updateModeLabel, app);
}
