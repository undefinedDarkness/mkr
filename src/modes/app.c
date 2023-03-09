#include "mode.h"

void app_mode_execute(Result result) {
	g_spawn_command_line_async(result.metadata, NULL);
}

GList* app_mode_generate(int*length) {
	AUTO list = g_app_info_get_all();
	AUTO head = list;

	while (list->next != NULL) {
		GAppInfo *app = list->data;
		Result res = {
			.label = g_app_info_get_display_name(app),
			.icon = g_app_info_get_icon(app),
			.metadata = (char*)g_app_info_get_executable(app)
		};
		memmove(app, &res, sizeof(res));
		list = list->next;
	}

	return head;
}

