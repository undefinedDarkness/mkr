#include "mode.h"

static bool updateTime(GtkWidget * clock)
{
	AUTO time = g_date_time_new_now_local();
	gtk_label_set_text(clock, g_date_time_format(time, "%H:%M"));
	g_date_time_unref(time);
}

static bool launcherOnClick(GtkWidget * btn, char *execute)
{
	g_spawn_command_line_async(execute, NULL);
}

static GtkWidget *launcherBtn(char *iconName, char *execute)
{
	if (execute == NULL)
		execute = iconName;

	AUTO btn =
	    gtk_button_new_from_icon_name(iconName,
					  GTK_ICON_SIZE_LARGE_TOOLBAR);
	g_signal_connect(btn, "clicked", launcherOnClick, execute);
	return btn;
}

static void dashboard_insert_todo_item(GList * list, GtkWidget * listbox)
{
	while (list != NULL) {
		CLASS(list->data, "dashboard-todo-item");
		ADD(listbox, list->data);
		list = list->next;
	}
	g_list_free(list);
}

GtkWidget *dashboard(void *)
{

	AUTO grid = gtk_grid_new();
	gtk_grid_set_row_homogeneous(grid, true);
	gtk_grid_set_column_homogeneous(grid, true);

	AUTO clock = gtk_label_new("");
	CLASS(clock, "dashboard-clock");
	AUTO time = g_date_time_new_now_local();
	gtk_label_set_text(clock, g_date_time_format(time, "%H:%M"));
	g_timeout_add_seconds(60, updateTime, clock);
	g_date_time_unref(time);
	gtk_grid_attach(grid, clock, 0, 0, 3, 1);

	AUTO launcher = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	CLASS(launcher, "dashboard-launcher");
	ADD(launcher, launcherBtn("firefox", "firefox-esr"));
	ADD(launcher, launcherBtn("utilities-terminal", "xterm"));
	ADD(launcher, launcherBtn("transmission", NULL));
	gtk_box_set_homogeneous(launcher, true);
	gtk_grid_attach_next_to(grid, launcher, clock, GTK_POS_RIGHT, 1, 2);

	AUTO quote =
	    gtk_label_new
	    ("<span font=\"Liberation Serif\"><i>Nothing is impossible,The word \nitself says 'I'm possible'</i></span>");
	CLASS(quote, "dashboard-quote");
	gtk_label_set_use_markup(quote, true);
	gtk_grid_attach_next_to(grid, quote, clock, GTK_POS_BOTTOM, 3, 1);

	// TODO: Add a playerctl widget at some point
	// TODO: Weather widget

	// This is a pretty dumb way of doing it but im also lazy so deal with it.
	AUTO todoView = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
	CLASS(todoView, "dashboard-todo-view");
	gtk_grid_attach_next_to(grid, todoView, quote, GTK_POS_BOTTOM, 4, 2);
	Api_t api = {
		.insert_custom_item = dashboard_insert_todo_item,	//, GtkWidget *listbox) 
		.data = todoView
	};
	todo_generate(&api);

	return grid;
}

// TODO: This is very incomplete, please customize it if you like..
