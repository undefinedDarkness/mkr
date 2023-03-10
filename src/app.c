#include "modes/mode.h"app
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

// gboolean supports_alpha = FALSE;
static void window_on_screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer userdata)
{
    /* To check if the display supports alpha channels, get the visual */
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

    if (!visual)
    {
        printf("Your screen does not support alpha channels!\n");
        visual = gdk_screen_get_system_visual(screen);
        // supports_alpha = FALSE;
    }
    else
    {
        printf("Your screen supports alpha channels!\n");
        // supports_alpha = TRUE;
    }

    gtk_widget_set_visual(widget, visual);
}

int main(int argc, char** argv) {
	gtk_init(&argc, &argv);
    
	State* app = malloc(sizeof(State));
	app->selectedItem = -1;
	app->config = g_key_file_new();
    g_key_file_load_from_file(app->config, "config.ini", G_KEY_FILE_NONE, NULL);

	// run_tests();

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated(window, FALSE);
    gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_DESKTOP);
	gtk_window_set_position(window, GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_keep_above(window, TRUE);
	
	g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	

	AUTO screen = gtk_widget_get_screen(window);

	// Setup for transparentcy???
	gtk_widget_set_app_paintable(window, true);
	AUTO vis = gdk_screen_get_rgba_visual(screen);
	gtk_widget_set_visual(window, vis);

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
	printf("Initial Mode: %s\n", app->currentMode.label);

	//== INIT UI ==
	scaffold(window, app); // app->ui is set here


	// Rest of mode init
	modeInit(app);

	gtk_widget_show_all(window);

	// window_on_screen_changed(window, NULL, NULL);

	gtk_window_present(window);

	gtk_widget_grab_focus(app->ui.input);

	// fill_display(app);
	gtk_main();
	free(app);
}
