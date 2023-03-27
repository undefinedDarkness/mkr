#include "app.h"

/* static void generate_on_finish(GObject*src,GAsyncResult*res,void*d) { */
/* 	printf("finished generation\n"); */
/* } */

/* static void generate_thread(GTask*task, GObject*source,APP,void*) { */
/* 	app->currentMode.generate(&app->api); */
/* 	g_task_return_error_if_cancelled(task); */
/* } */

/* void generate_async(APP) { */
/* 	GTask* task = g_task_new(NULL, NULL, generate_on_finish, NULL); */
/* 	g_task_set_task_data(task, app, NULL); */	
/* 	g_task_run_in_thread(task, app->currentMode.generate); */
/* } */
