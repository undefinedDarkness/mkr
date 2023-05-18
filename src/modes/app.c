#include "mode.h"

// NOTE: Memory leak with Results, they aren't ever freed

void app_execute(const char *const s, Result result) {
  if (result.id != -1)
    g_spawn_command_line_async(result.metadata, NULL);
}

static void app_exe_resptr(GtkWidget*, Result*res) {
	g_assert(res!=NULL && res->metadata!=NULL);
	g_spawn_command_line_async(res->metadata, NULL);
}

void app_clean(Result *res) { 
	g_slice_free1(sizeof(Result), res);
}

const char *favouriteNames[] = {"Firefox ESR",
                                "Inkscape Vector Graphics Editor", "Files"};

void app_generate(API) {
  AUTO list = g_app_info_get_all();
  const AUTO listHead = list;
  GList *copy = NULL;

  Result *favourites[sizeof(favouriteNames) / sizeof(favouriteNames[0])];
  int nFound = 0;

  while (list != NULL) {
    GAppInfo *app = list->data;
    if (g_app_info_should_show(app)) {
      Result *res = g_slice_alloc(sizeof(Result));
      res->label = g_strdup(g_app_info_get_display_name(app));

      // if matches favourites
      if (nFound != sizeof(favouriteNames) / sizeof(favouriteNames[0])) {
        for (int i = 0; i < sizeof(favouriteNames) / sizeof(favouriteNames[0]);
             i++) {
          if (NULL != strstr(res->label, favouriteNames[i])) {
            favourites[nFound++] = res;
            break;
          }
        }
      }

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

  static Result favResult = {
      .icon = NULL, .metadata = NULL, .label = "Favourites", .id = -1};

  AUTO favBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
  AUTO favScroll = gtk_scrolled_window_new(NULL, NULL);
  ADD(favScroll, favBox);
	CLASS(favBox,"favourites-row");
  g_object_set_data(favScroll, "__resptr", &favResult);
  for (int i = 0; i < sizeof(favourites) / sizeof(favourites[0]); i++) {
    AUTO btn = gtk_button_new();//_with_label(favourites[i]->label);
    gtk_button_set_image(btn, gtk_image_new_from_gicon(favourites[i]->icon,
                                                       GTK_ICON_SIZE_DND));
		CLASS(btn, "favourites-button");
    g_signal_connect(btn, "clicked", app_exe_resptr, favourites[i]);
    ADD(favBox, btn);
  }

  api->insert_custom_item(g_list_prepend(NULL, favScroll), api->data);
  g_list_free_full(listHead, g_object_unref);
  api->insert_item(copy, api->data);
}
