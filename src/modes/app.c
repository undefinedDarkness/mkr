#include "app.h"

// NOTE: Memory leak with Results, they aren't ever freed

void app_execute(const char*const s, Result result) {
	g_spawn_command_line_async(result.metadata, NULL);
}

void app_generate(API) {
	AUTO list = g_app_info_get_all();
	AUTO head = list;

	while (list != NULL) {
		GAppInfo *app = list->data;
		Result res = {
			.label = g_app_info_get_display_name(app),
			.icon = g_app_info_get_icon(app),
			.metadata = (char*)g_app_info_get_executable(app)
		};
		memmove(app, &res, sizeof(res));
		list = list->next;
	}

	api->insert_item(head, api->data);
	// cb(head,cbd);
}
