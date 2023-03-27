#include "script.h"
#include <gio/gunixinputstream.h>

// TODO: Move the item creation to be async, it takes up the entire thread
// otherwise I dont know why emoji doesnt also suffer from this issue but I
// guess, the little bit of time that reads take is enough to not keep the main
// thread very occupied.

void script_wait_cb(GObject *source, GAsyncResult *res, API) {
  g_subprocess_wait_finish(source, res, NULL);
}

struct ScriptData {
  GDataInputStream *stream;
  State *app;
};
static struct ScriptData state;

// This function is run in a thread
static void script_read_thread(GTask *task, GObject *source,
                               struct ScriptData *td, void *) {
  int len;
  char *line;
  GList *list = NULL;
  int count = 100;
  while (count-- && ((line = g_data_input_stream_read_line(
                          td->stream, &len, NULL, NULL)) != NULL)) {
    // line = g_data_input_stream_read_line(td->stream, &len, NULL, NULL);
    Result *res = g_slice_alloc0(sizeof(Result));
    res->label = line;
    GtkWidget *obj = gtk_list_box_row_new();
    GtkWidget *lbl = gtk_label_new(line);
	gtk_label_set_xalign(lbl, 0);
    ADD(obj, lbl);
    list = g_list_prepend(list, obj);
    gtk_widget_show_all(obj);
  }
  list = g_list_reverse(list);
  g_task_return_pointer(task, list, NULL);
}

// glib async boilerplate
static GList *script_read_finish(GObject *source, GAsyncResult *res,
                                 void *err) {
  g_return_val_if_fail(g_task_is_valid(res, source), NULL);
  return g_task_propagate_pointer(res, err);
}

static void script_read_async(GAsyncReadyCallback *cb, void *ud) {
  GTask *task = g_task_new(NULL, NULL, cb, ud);
  g_task_set_task_data(
      task, ud,
      NULL); // TODO: give some handler to free this at some point? i guess
  g_task_run_in_thread(task, script_read_thread);
}

static void script_read_cb(GObject *src, GAsyncResult *res,
                           struct ScriptData *data) {
  GList *list = script_read_finish(src, res, NULL);
 
  if (list == NULL) {
  	#ifndef NDEBUG
	  g_print("Script: End of list\n");
	#endif
    return;
  }
  AUTO display = data->app->ui.display;
  while (list != NULL) {
    ADD(display, list->data);
    list = list->next;
  }
  script_read_async(script_read_cb, data);
}

/* static struct CommandData cmd; */

// TODO: Allow any still running async tasks to be cancelled when a new one is created..

void script_generate(API) {
  APP = api->data;
  const Mode self = app->currentMode; // rarely required but here it is ;-;
  const struct CommandData *cmdData = self.payload;

  const char *cwd = cmdData->workingDirectory;
  if (cwd == NULL)
    cwd = g_get_current_dir();
  else if (cwd == (void *)1)
    cwd = g_get_home_dir();

#define EQ(x) strncmp(x, cmdData->argv[i], sizeof(x)) == 0
  /* char* bucket = malloc(1024); */
  char **argv = (char **)g_slice_alloc(256);
  int i = 0;
  while (cmdData->argv[i] != NULL) {
    if (EQ("[SEARCH]")) {
      argv[i] = app->search;
    } else {
      argv[i] = cmdData->argv[i];
    }
    i++;
  }
  argv[i] = NULL;

#ifndef NDEBUG
  printf("Arguments: ");
  i = 0;
  while (argv[i] != NULL) {
	  printf("%s\t", argv[i]);
	  i++;
  }
  printf("\n");
#endif

  AUTO procLauncher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE);
  g_subprocess_launcher_set_cwd(procLauncher, cwd);
  AUTO process =
      g_subprocess_launcher_spawnv(procLauncher, argv, NULL);
  /* AUTO process =
      g_subprocess_newv(cmdData->argv, G_SUBPROCESS_FLAGS_STDOUT_PIPE,
                        NULL); */
  /* struct ScriptData *data = malloc(sizeof(struct ScriptData)); */
  state.app = app;
  state.stream = g_data_input_stream_new(g_subprocess_get_stdout_pipe(process));

  g_subprocess_wait_async(process, NULL, script_wait_cb, api);
  script_read_async(script_read_cb, &state);
}
