#include "app.h"
#include "gtk/gtkcssprovider.h"
#include "modes/modes-s.h"

GResource *mkr_get_resource();
static void init_resource() {
	GResource *res = mkr_get_resource();
	g_resources_register(res);
	// GtkIconTheme *theme = gtk_icon_theme_get_default();
	// gtk_icon_theme_add_resource_path (theme, "/undefinedDarkness/colr/icons");
}

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);
	init_resource();

	State *app = malloc(sizeof(State));

	app->api = (Api_t) {
		.update_progress = async_update_progress,.insert_item = async_insert_item,.data = app,.insert_custom_item = async_insert_custom_item	//, const State *const app) 
	};
	app->search = "";
	app->selectedItem = -1;
	app->config = g_key_file_new();
	g_key_file_load_from_file(app->config, "config.ini", G_KEY_FILE_NONE,
				  NULL);

	// run_tests();

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
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
	AUTO cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_resource(cssProvider, "/undefinedDarkness/mkr/style.css");
	// gtk_css_provider_load_from_file(cssProvider,
					// g_file_new_for_path("style.css"), NULL);
	gtk_style_context_add_provider_for_screen(screen, cssProvider,
						  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	//== SETUP MODES ==
	AUTO modeLabel = C_GET("MODE", "default", string, "APP");
	app->currentMode = modes[0];
	/* for (int i = 0; i < sizeof(modes) / sizeof(modes[0]); i++)
		if (g_ascii_strcasecmp(modeLabel, modes[i].label) == 0)
			app->currentMode = modes[i]; */
	printf("Initial Mode: %s\n", app->currentMode.label);

	//== INIT UI ==
	scaffold(window, app);	// app->ui is set here

	// Rest of mode init
	modeInit(modes[0], app);

	gtk_widget_show_all(window);
	gtk_widget_hide(app->ui.preview);

	// window_on_screen_changed(window, NULL, NULL);

	gtk_window_present(window);

	gtk_widget_grab_focus(app->ui.input);

	// fill_display(app);
	gtk_main();
	free(app);
}
