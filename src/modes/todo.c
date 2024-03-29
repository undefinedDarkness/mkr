#include "mode.h"

// lets assume, your todo list isnt some 1000 items long....

static char* path; 
void todo_generate(API)
{
	path = g_build_filename(g_get_user_special_dir(G_USER_DIRECTORY_DOCUMENTS), "todo-mkr.txt", NULL);
	AUTO io = g_io_channel_new_file(path, "r", NULL);
	char *line = NULL;
	int len;
	GList *list = NULL;
	while (g_io_channel_read_line(io, &line, &len, NULL, NULL) !=
	       G_IO_STATUS_EOF) {
		char **parts = g_strsplit(line, "\t", 2);
		AUTO wid =
		    gtk_check_button_new_with_label(g_strstrip(parts[1]));
		gtk_toggle_button_set_active(wid, 'D' == *parts[0]);
		list = g_list_prepend(list, wid);
	}
	// return;
	list = g_list_reverse(list);
	api->insert_custom_item(list, api->data);
}
