#include "app.h"

// WARN: Trigger happy
void async_update_progress(float v, void *ptr)
{
	APP_MUT = ptr;
	// static GtkWidget *progress;
	// NOTE: v is a fraction not a percentage
	if (v == 0) {
		AUTO scroll = app->ui.scroll;
		app->ui.display = gtk_progress_bar_new();
		CLASS(app->ui.display, "display");
		gtk_widget_set_valign(app->ui.display, GTK_ALIGN_START);
		ADD(scroll, app->ui.display);
		gtk_widget_show_all(scroll);
	}
	gtk_progress_bar_set_fraction(app->ui.display, v);
	if (v == 1) {
		if (C_GET("BEHAVIOUR", "exitOnExecute", boolean, true))
			QUIT();
	}
}

void execute(APP)
{
	AUTO type = app->currentMode.metadata.type;
	bool quit = C_GET("OPERATION", "exitOnExecute", boolean, true);

	if (app->currentMode.execute == NULL) {
		g_error("Invalid execute provided by %s\n",
			app->currentMode.label);
	}

	if (type & ITEMS) {
		AUTO selected = gtk_list_box_get_selected_row(app->ui.display);
		Result *result = g_object_get_data(selected, "__resptr");
		app->currentMode.execute(g_object_get_data(selected, "__label"),
					 *result);
	} else if (type & COMMAND) {
		if (type & PROGRESS) {
			app->currentMode.
			    execute_command(gtk_entry_get_text(app->ui.input),
					    &app->api);
		} else {
			app->currentMode.
			    execute_command(gtk_entry_get_text(app->ui.input),
					    &app->api);
		}
	}

	if (quit) {
		// This is to allow the clipboard to pick up any requested contents
		/* gtk_widget_hide(app->ui.window); */
		/* g_timeout_add(1000, gtk_main_quit, NULL); */
	}
}
