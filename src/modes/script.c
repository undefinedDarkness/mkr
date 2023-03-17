#include "script.h"
#include <gio/gunixinputstream.h>

// TODO: Move the item creation to be async, it takes up the entire thread
// otherwise I dont know why emoji doesnt also suffer from this issue but I
// guess, the little bit of time that reads take is enough to not keep the main
// thread very occupied.

void script_wait_cb(GObject * source, GAsyncResult * res, API)
{
	g_subprocess_wait_finish(source, res, NULL);
}

struct ScriptData {
	GDataInputStream *stream;
	State *app;
};

// This function is run in a thread
static void script_read_thread(GTask * task, GObject * source,
			       struct ScriptData *td, void *)
{
	int len;
	char *line;
	GList *list = NULL;
	int count = 100;
	while (count--
	       &&
	       ((line =
		 g_data_input_stream_read_line(td->stream, &len, NULL,
					       NULL)) != NULL)) {
		// line = g_data_input_stream_read_line(td->stream, &len, NULL, NULL);
		Result *res = g_slice_alloc0(sizeof(Result));
		res->label = line;
		GtkWidget *obj = gtk_list_box_row_new();
		GtkWidget *lbl = gtk_label_new(line);
		ADD(obj, lbl);
		list = g_list_prepend(list, obj);
		gtk_widget_show_all(obj);
	}
	list = g_list_reverse(list);
	g_task_return_pointer(task, list, NULL);
}

// glib async boilerplate
static GList *script_read_finish(GObject * source, GAsyncResult * res,
				 void *err)
{
	g_return_val_if_fail(g_task_is_valid(res, source), NULL);
	return g_task_propagate_pointer(res, err);
}

static void script_read_async(GAsyncReadyCallback * cb, void *ud)
{
	GTask *task = g_task_new(NULL, NULL, cb, ud);
	g_task_set_task_data(task, ud, NULL);	// TODO: give some handler to free this at some point? i guess
	g_task_run_in_thread(task, script_read_thread);
}

static void script_read_cb(GObject * src, GAsyncResult * res,
			   struct ScriptData *data)
{
	GList *list = script_read_finish(src, res, NULL);
	if (list == NULL) {
		g_print("DONEDONE\n");
		return;
	}
	AUTO display = data->app->ui.display;
	while (list != NULL) {
		ADD(display, list->data);
		list = list->next;
	}
	script_read_async(script_read_cb, data);
}

void script_generate(API)
{
	APP = api->data;
	Mode self = app->currentMode;	// rarely required but here it is ;-;
	struct CommandData *cmdData = self.payload;

	AUTO process =
	    g_subprocess_newv(cmdData->argv, G_SUBPROCESS_FLAGS_STDOUT_PIPE,
			      NULL);
	struct ScriptData *data = malloc(sizeof(struct ScriptData));
	data->app = app;
	data->stream =
	    g_data_input_stream_new(g_subprocess_get_stdout_pipe(process));

	g_subprocess_wait_async(process, NULL, script_wait_cb, api);
	script_read_async(script_read_cb, data);
}
