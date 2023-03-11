#include "mode.h"

void emoji_execute(const char* label, void*) {
	// Result is already invalid by now, since it was stack memory
	char** parts = g_strsplit(label, "\t", 2);
	AUTO clip = gtk_clipboard_get_default(gdk_display_get_default());
	gtk_clipboard_set_text(clip, parts[0], -1);
	g_strfreev(parts);
}

// herculean gymnastics to make this work correctly
void emoji_read_cb(GObject *source, GAsyncResult *res, API) {
  char *line = g_data_input_stream_read_line_finish(
      source, res, NULL,
      NULL);
  
  if (line == NULL) {
   	  g_print("Finished loading emoji\n");
	  return;
  }
  // g_print("Got line %s\n", line);

	api->insert_item(&(GList){
			.next = NULL,
			.prev = NULL,
			.data = &(Result){
				.label = line
			}
		}, api->data); 
  // If added to idle queue, will only work at end
  // g_idle_add(api->insert_item_single, id);
  g_data_input_stream_read_line_async(source, G_PRIORITY_DEFAULT, NULL,
                                      emoji_read_cb, api);
}

void emoji_generate(API) {
  AUTO file = g_file_new_for_path("./resources/emoji-data.txt");
  AUTO fstream = g_file_read(file, NULL, NULL);
  AUTO fdstream = g_data_input_stream_new(fstream);
  g_data_input_stream_read_line_async(fdstream, G_PRIORITY_DEFAULT, NULL,
                                      emoji_read_cb, api);
}
