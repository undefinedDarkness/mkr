#include "app.h"
#include <glib-unix.h>
#include "glib.h"
#include "gtk/gtk.h"
#include "gtk/gtkcssprovider.h"
#include "modes/modes-s.h"
#include "src/basic.h"

State app_val;

GResource *mkr_get_resource();
static void init_resource() {
	GResource *res = mkr_get_resource();
	g_resources_register(res);
}

struct { int x; int y;  } window_state;
bool toggle_display(void *_ptr) {
	if (gtk_widget_is_visible(app_val.ui.window)) {
		gtk_window_get_position(app_val.ui.window, &window_state.x, &window_state.y);
		gtk_widget_hide(app_val.ui.window);
	} else {
		gtk_window_present(app_val.ui.window);
		gtk_window_move(app_val.ui.window, window_state.x, window_state.y);
	}
	return true;
}

static bool createOrCheckForPidFile() {
	AUTO fp = fopen("/tmp/mkr-pidfile", "r");
	char buffer[100];
	if ((fp)) {
		fgets(buffer, sizeof(buffer), fp);
		uint32_t pid = atoi(buffer);
		fclose(fp);
		if (pid != 0 && kill(pid, SIGUSR1) == 0) {
			printf("=== SYS === Found existing process");
			return true;
		}
	}

	fp = fopen("/tmp/mkr-pidfile", "w");
	fprintf(fp, "%d", getpid());
	fclose(fp);

	return false;
}

int main(int argc, char **argv)
{
	// TODO: Deal with config & style files not existing (Copy)
	GError *error = NULL;
	GOptionContext *context;
	gboolean show_help = FALSE;
	gboolean show_version = FALSE;
	bool open_inspector = false;

	GOptionEntry entries[] = {
		{ "help", 'h', 0, G_OPTION_ARG_NONE, &show_help, "Show help message", NULL },
		{ "version", 'v', 0, G_OPTION_ARG_NONE, &show_version, "Show application version", NULL },
		{ "inspect", 0, 0, G_OPTION_ARG_NONE, &open_inspector, "Open inspector", NULL },
		{ NULL }
	};

	context = g_option_context_new("- application options");
	g_option_context_add_main_entries(context, entries, NULL);
	g_option_context_parse(context, &argc, &argv, &error);

	if (error != NULL) {
		g_printerr("Option parsing failed: %s\n", error->message);
		g_error_free(error);
		exit(1);
	}

	if (show_help) {
		g_print("%s\n", g_option_context_get_help(context, TRUE, NULL));
		exit(0);
	}

	if (show_version) {
		g_print("Application version: 1.0.0\n");
		exit(0);
	}

	if (open_inspector) {
		g_setenv("GTK_DEBUG", "interactive", true);
	}

	g_option_context_free(context);

	if (createOrCheckForPidFile())
		return 0;
	gtk_init(&argc, &argv);
	init_resource();

	g_unix_signal_add(SIGUSR1, toggle_display, NULL);

	State *app = &app_val;	
	/* State *app = malloc(sizeof(State)); */

	app->api = (Api_t) {
		.update_progress = async_update_progress,.insert_item = async_insert_item,.data = app,.insert_custom_item = async_insert_custom_item	//, const State *const app) 
	};
	app->search = "";
	app->selectedItem = -1;
	app->config = g_key_file_new();

	AUTO userConfigPath = g_get_user_config_dir();
	AUTO userConfigFile = g_build_path("/", userConfigPath, "mkr", "config.ini", NULL);

	printf("Loading configuration from %s\n", userConfigFile);
	g_key_file_load_from_file(app->config, userConfigFile, G_KEY_FILE_NONE,
						   NULL);

	// run_tests();

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated(window, FALSE);
	gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_DESKTOP);

	AUTO windowPosition = GTK_WIN_POS_NONE;
	AUTO userWindowPosition = C_GET("WINDOW", "position", string, "NONE");
	if (g_ascii_strcasecmp(userWindowPosition, "CENTER") == 0) {
		printf("Centering window\n");
		windowPosition = GTK_WIN_POS_CENTER_ALWAYS;
	} else if (g_ascii_strcasecmp(userWindowPosition, "MOUSE") == 0) {
		printf ("Positioning window at mouse\n");
		windowPosition = GTK_WIN_POS_MOUSE;
	}

	gtk_window_set_position(window, windowPosition);
	gtk_window_set_keep_above(window, TRUE);

	g_signal_connect(window, "destroy", gtk_main_quit, NULL);

	AUTO screen = gtk_widget_get_screen(window);

	// Setup for transparentcy???
	gtk_widget_set_app_paintable(window, true);
	AUTO vis = gdk_screen_get_rgba_visual(screen);
	gtk_widget_set_visual(window, vis);

	/*== Window Styling ==*/
	AUTO cssProvider = gtk_css_provider_new();
	AUTO cssFilePath = g_build_path("/", userConfigPath, "mkr", "user.css", NULL);

	gtk_css_provider_load_from_resource(cssProvider, "/undefinedDarkness/mkr/style.css");
	gtk_style_context_add_provider_for_screen(screen, cssProvider,
										   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_file(cssProvider,
								 g_file_new_for_path(cssFilePath), NULL);
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
	update_selected(0, app);

	gtk_widget_show_all(window);
	gtk_widget_hide(app->ui.preview);

	// window_on_screen_changed(window, NULL, NULL);

	gtk_window_present(window);
	gtk_window_resize(window, 1, 1);
	on_input_changed(app->ui.input, app);

	gtk_widget_grab_focus(app->ui.input);

	// fill_display(app);
	gtk_main();
}
