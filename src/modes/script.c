#include "script.h"
#include <gio/gunixinputstream.h>
#include <string.h>

// TODO: Move the item creation to be async, it takes up the entire thread
// otherwise I dont know why emoji doesnt also suffer from this issue but I
// guess, the little bit of time that reads take is enough to not keep the main
// thread very occupied.

void script_launch_file(const char *lbl, Result res) {
  char *buf = g_slice_alloc(256);
  strcpy(buf, "file://");
  strcat(buf, res.metadata);
  bool u;
  const char *contentType = g_content_type_guess(lbl, NULL, 0, &u);
#ifndef NDEBUG
  printf("Launching: %s with %s\n", buf, contentType);
#endif

  g_app_info_launch_default_for_uri_async(buf, NULL, NULL, NULL, NULL);
  g_slice_free1(256, buf);
}

static void script_wait_cb(GObject *source, GAsyncResult *res, API) {
  g_subprocess_wait_finish(source, res, NULL);
}

struct ScriptData {
  GDataInputStream *stream;
  State *app;
  struct CommandData *cmd;
  GSubprocess *subprocess;
  GSubprocessLauncher *launcher;
  GInputStream *pipe;
};
static struct ScriptData state;

// This function is run in a thread
static void script_read_thread(GTask *task, GObject *source,
                               struct ScriptData *td, void *_) {
  size_t len;
  const char *line;
  GList *list = NULL;
  int count = 100;
  while (count-- && ((line = g_data_input_stream_read_line(
                          td->stream, &len, NULL, NULL)) != NULL)) {
    // line = g_data_input_stream_read_line(td->stream, &len, NULL, NULL);
    Result *res = g_slice_alloc0(sizeof(Result));
    res->label = line;
    GtkWidget *obj = gtk_flow_box_child_new();
    g_object_set_data(obj, "__resptr", res);
    g_object_set_data(obj, "__label", line);
    AUTO layout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    CLASS(layout, "script-item");

    if (td->cmd->flags & FLAG_FILEPATH) {
      const AUTO hDir = g_get_home_dir();
      bool u;
      const char *type = g_content_type_guess(res->label, NULL, 0, &u);
      const GIcon *typeImg = g_content_type_get_icon(type);
      const GtkImage *img =
          gtk_image_new_from_gicon(typeImg, GTK_ICON_SIZE_DND);
      CLASS(img, "script-icon");
      ADD(layout, img); // gtk_container_add(layout, img);
      res->metadata = g_canonicalize_filename(
          res->label,
          td->cmd->workingDirectory != (void *)1 ? g_get_current_dir() : hDir);

      // Replace /home/$USER with ~ for cleaner output
      AUTO b = g_strsplit(line,hDir,-1);
	  res->label = g_strjoinv("~", b);
	  g_strfreev(b);
    }

    GtkWidget *lbl = gtk_label_new(res->label);
    CLASS(lbl, "script-text");
    gtk_label_set_xalign(lbl, 0);

    ADD(layout, lbl);
    ADD(obj, layout);

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
	  //g_object_unref(data->stream);
	  //g_object_unref(data->pipe);
	  /* g_object_unref(data->subprocess); */
	  //g_object_unref(data->launcher);
#ifndef NDEBUG
    g_print("== SCRIPT == Fin: End of list\n");
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

// TODO: Allow any still running async tasks to be cancelled when a new one is
// created..

void script_generate(API) {
  APP = api->data;
  const Mode self = app->currentMode; // rarely required but here it is ;-;
  const struct CommandData *cmdData = self.payload;

  const char *cwd = g_getenv("HOME");

#define EQ(x) strncmp(x, cmdData->argv[i], sizeof(x)) == 0
  char **argv = (char **)g_slice_alloc(256); // space for 32 different arguments

  if (cmdData->flags & FLAG_SHELL) {
    argv[0] = g_strdup("sh");
    argv[1] = g_strdup("-c");
    argv[2] = cmdData->argv[0];
    argv[3] = app->search;
    argv[4] = NULL;
  } else {
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
  }

#undef EQ
#ifndef NDEBUG
  printf("== SCRIPT == Working Directory: %s\n== SCRIPT == Arguments: ", cwd);
  int i = 0;
  while (argv[i] != NULL) {
    printf("%s,", argv[i]);
    i++;
  }
  printf("\n");
#endif

  // -- Process And Pipe Creation --
  GSubprocessLauncher* procLauncher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE);
  g_subprocess_launcher_set_cwd(procLauncher, cwd);
  GError* err = NULL;
  GSubprocess* process = g_subprocess_launcher_spawnv(procLauncher, argv, &err);
 
	if (process == NULL) {
		printf("== SCRIPT == Failed to launch subprocess: %s\n", err->message);
		return;
	}

  g_assert(G_IS_SUBPROCESS_LAUNCHER(procLauncher));
  g_assert(G_IS_SUBPROCESS(process));

  // Initialize global state
  state.app = app;
  state.cmd = cmdData;
  state.pipe = g_subprocess_get_stdout_pipe(process);
  state.stream = g_data_input_stream_new(state.pipe);
	state.launcher = procLauncher;
	state.subprocess = process;

  g_subprocess_wait_async(process, NULL, script_wait_cb, api);
  script_read_async(script_read_cb, &state);
	/* g_free(cwd); */
	g_slice_free1(256, argv);
}
