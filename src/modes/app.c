#include "mode.h"

// NOTE: Memory leak with Results, they aren't ever freed

void app_execute(const char *const s, Result result) {
  g_spawn_command_line_async(result.metadata, NULL);
}

void app_clean(Result *res) { g_slice_free1(sizeof(Result), res); }

void app_generate(API) {
  AUTO list = g_app_info_get_all();
  const AUTO listHead = list;
  GList *copy = NULL;

  while (list != NULL) {
    GAppInfo *app = list->data;
    if (g_app_info_should_show(app)) {
      Result *res = g_slice_alloc(sizeof(Result));
      res->label = g_strdup(g_app_info_get_display_name(app));

      AUTO icon = g_app_info_get_icon(app);
      if (icon != NULL) {
		  g_object_ref(icon);
      	res->icon = icon;
	  }

      res->metadata = g_strdup(g_app_info_get_executable(app));
      copy = g_list_prepend(copy, res);
    }
    list = list->next;
  }

  g_list_free_full(listHead, g_object_unref);
  api->insert_item(copy, api->data);
}
