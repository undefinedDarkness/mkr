#include "mode.h"

typedef struct {
  void *app;
  float fsize;
  float completed;
  GFileOutputStream *ofstream;
} DLState;

static void async_read_cb(GInputStream *source, GAsyncResult *res,
                          DLState *state) {
  GBytes *out = g_input_stream_read_bytes_finish(source, res, NULL);
 
  // dont try making this async, it wont work...
  g_output_stream_write_bytes(state->ofstream, out, NULL, NULL);
  
  size_t size = g_bytes_get_size(out);
  if (size == 0) {
    g_object_unref(source);
	g_object_unref(state->ofstream);
    free(state);
	return;
  }
  state->completed += size;
  update_progress(state->completed / state->fsize, state->app);
  g_input_stream_read_bytes_async(source, 1096, G_PRIORITY_DEFAULT, NULL,
                                  async_read_cb, state);
}

void dl_execute(const char *url, void *app) {
  AUTO file = g_file_new_for_uri(url);
  
  AUTO ofile = g_file_new_build_filename(g_get_user_special_dir(G_USER_DIRECTORY_DOWNLOAD), g_file_get_basename(file), NULL);

  update_progress(0.0, app); // run init code

  GError *err = NULL;
  AUTO fstream = g_file_read(file, NULL, &err);
  if (err != NULL) {
    g_print("ERROR WITH READING FILE....");
    update_progress(100, app);
    g_object_unref(err);
    return;
  }
 
  DLState *state = malloc(sizeof(DLState));
  state->app = app;
  AUTO finfo = g_file_input_stream_query_info(
      fstream, G_FILE_ATTRIBUTE_STANDARD_SIZE, NULL, NULL);
  // g_print("File Size:%lu\n ", g_file_info_get_size(finfo));
  state->fsize = g_file_info_get_size(finfo);
  state->completed = 0;
  state->ofstream = g_file_append_to(ofile, G_FILE_CREATE_NONE, NULL, NULL);
  g_object_unref(finfo);
  g_object_unref(file);
  g_object_unref(ofile);

  g_input_stream_read_bytes_async(fstream, 1096, G_PRIORITY_DEFAULT, NULL,
                                  async_read_cb, state);
}
