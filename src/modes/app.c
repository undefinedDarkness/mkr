#include "mode.h"

// NOTE: Memory leak with Results, they aren't ever freed

void app_execute(const char *const s, Result result) {
  g_spawn_command_line_async(result.metadata, NULL);
}

void app_generate(API) {
  AUTO list = g_app_info_get_all();
	GList* copy = NULL;

  while (list != NULL) {
    GAppInfo *app = list->data;
    if (g_app_info_should_show(app)) {
      Result res = {.label = g_app_info_get_display_name(app),
                    .icon = g_app_info_get_icon(app),
                    .metadata = (char *)g_app_info_get_executable(app)};
      memmove(app, &res, sizeof(res));
	  copy = g_list_prepend(copy, app);
    }
    list = list->next;
  }

  api->insert_item(g_list_reverse(copy), api->data);
}
