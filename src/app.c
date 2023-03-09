#include "modes/mode.h"
#include "app.h"

void run_tests() {
	GList* r = app_mode_generate();
	while(r->next != NULL) {
		Result* res = r->data;
		printf("NAME: %s\nEXE: %s\n\n", res->label, res->metadata);
		r = r->next;
	}
	exit(0);
}

int main(int argc, char** argv) {
	gtk_init(&argc, &argv);
    
	State* app = malloc(sizeof(State));
	app->selectedItem = 0;
	app->config = g_key_file_new();
    g_key_file_load_from_file(app->config, "config.ini", G_KEY_FILE_NONE, NULL);

	// run_tests();

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated(window, FALSE);
    gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_DIALOG);
	g_signal_connect(window, "destroy", gtk_main_quit, NULL);

	gtk_window_set_keep_above(window, TRUE);

    /*== Window Position And Size ==*/
    GdkScreen* screen = gtk_window_get_screen(window);
    GdkRectangle workarea;
    gdk_monitor_get_workarea(
        gdk_display_get_primary_monitor(gdk_screen_get_display(screen)), 
        &workarea);
    
    int height = C_GET("WINDOW", "height", integer, 24);
    if (C_GET("WINDOW", "top", boolean, TRUE))
        gtk_window_move(window, workarea.x, workarea.y);
    else
        gtk_window_move(window, workarea.x, workarea.y + workarea.height - height);
    gtk_window_resize(window, workarea.width, height);

    /*== Window Styling ==*/
    AUTO cssProvider = gtk_css_provider_new( );
	gtk_css_provider_load_from_file(cssProvider, g_file_new_for_path("style.css"), NULL);
	gtk_style_context_add_provider_for_screen(
		screen,
        cssProvider, 
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	//== SETUP MODES ==
	AUTO modeLabel = C_GET("MODE", "default", string, "APP");
	app->currentMode = modes[0];
	for (int i = 0; i < sizeof(modes)/sizeof(modes[0]); i++)
		if (g_ascii_strcasecmp(modeLabel, modes[i].label) == 0)
			app->currentMode = modes[i];
	printf("%s\n", app->currentMode.label);

	//== INIT UI ==
	scaffold(window, app); // app->ui is set here
	gtk_widget_show_all(window);
	gtk_window_present(window);

	gtk_widget_grab_focus(app->ui.input);

	fill_display(app);
	gtk_main();
	free(app);
}
