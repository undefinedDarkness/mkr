#include "mode.h"
#include <stdlib.h>

struct EmojiData {
	void*cbd;
	void (*cb)(GList*, void*);
	GList*buffer;
	int count;
};

void emoji_execute(const char* label, Result) {
	// Result is already invalid by now, since it was stack memory
	char** parts = g_strsplit(label, "\t", 2);
	AUTO clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text(clip, parts[0], -1);
	gtk_clipboard_store(clip);
	g_strfreev(parts);
	g_object_unref(clip);
}

// herculean gymnastics to make this work correctly
void emoji_read_cb(GObject *source, GAsyncResult *res, struct EmojiData *ed) {
  char *line = g_data_input_stream_read_line_finish(
      source, res, NULL,
      NULL);
  
  if (line == NULL) {
	  free(ed);
   	  g_print("Finished loading emoji\n");
	  return;
  }
 
  Result *result = calloc(1, sizeof(Result));
  result->label = line;
  ed->buffer = g_list_prepend(ed->buffer, result);
	

	if (ed->count++ == 50) {
		// g_list_foreach(ed->buffer, create_widget, NULL);
		ed->cb(g_list_reverse(ed->buffer), ed->cbd);
		ed->count = 0;
		ed->buffer = NULL; // reset list
		// g_print("buffered 50 items\n");
	}

  // If added to idle queue, will only work at end
  // g_idle_add(api->insert_item_single, id);
  g_data_input_stream_read_line_async(source, G_PRIORITY_DEFAULT, NULL,
                                      emoji_read_cb, ed);
}

void emoji_generate(API) {
  AUTO file = g_file_new_for_path("./resources/emoji-data.txt");
  AUTO fstream = g_file_read(file, NULL, NULL);
  AUTO fdstream = g_data_input_stream_new(fstream);
  struct EmojiData *ed = malloc(sizeof(struct EmojiData));
  ed->cb = api->insert_item;
  ed->cbd = api->data;
  ed->buffer = NULL;
  ed->count = 0;
  g_data_input_stream_read_line_async(fdstream, G_PRIORITY_DEFAULT, NULL,
                                      emoji_read_cb, ed);
}
