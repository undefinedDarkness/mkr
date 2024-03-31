#include "app.h"
#include "modes/mode.h"
#include "modes/modes-s.h"

static void modeItems(APP_MUT) {
  AUTO display = gtk_list_box_new();
  gtk_list_box_set_sort_func(display, (app->currentMode.metadata.type & CLEAR_ON_EDIT) ? NULL : sort, app, NULL);
  gtk_list_box_set_selection_mode(display, GTK_SELECTION_SINGLE);
  CLASS(display, "display");

  if (app->currentMode.metadata.type & HAS_PREVIEW) {
    g_signal_connect(display, "row-activated", display_preview, app);
  }
  ADD(app->ui.scroll, display);
  app->ui.display = display;

  g_assert_nonnull(app->currentMode.generate);
  app->currentMode.generate(&app->api);

  CLASS(app->ui.scroll, "items");
  gtk_widget_show_all(app->ui.scroll);

  // Preview Code
  AUTO previewWidget = gtk_bin_get_child(app->ui.preview);
  if (GTK_IS_WIDGET(previewWidget))
	  gtk_widget_destroy(previewWidget);
  
  if (app->currentMode.metadata.type & HAS_PREVIEW) {
    gtk_widget_show(app->ui.preview);
  } else {
    gtk_widget_hide(app->ui.preview);
  }
}

static void modeCommand(APP_MUT) {
  gtk_style_context_remove_class(gtk_widget_get_style_context(app->ui.scroll),
                                 "items");
  gtk_widget_hide(app->ui.scroll);
  gtk_window_resize(app->ui.window, 1, 1);

  if (app->currentMode.metadata.type & PROGRESS) {
  }
}

static void modePreview(APP_MUT) {
  gtk_widget_hide(app->ui.preview);
  AUTO child = app->currentMode.preview(NULL);
  ADD(app->ui.scroll, child);
  app->ui.display = child;
  CLASS(child, "display");
  gtk_widget_show_all(app->ui.scroll);
}

static bool updateModeLabel(APP) {
  gtk_label_set_text(app->ui.mode, app->currentMode.metadata.symbol);
  AUTO modeStyleCtx = gtk_widget_get_style_context(app->ui.mode);
  for (int i = 0; i < sizeof(modes) / sizeof(modes[0]); i++)
    gtk_style_context_remove_class(modeStyleCtx, modes[i].label);
  gtk_style_context_add_class(modeStyleCtx, app->currentMode.label);
}

// TODO: Allow modes to implement their own cleanup
// WARN: Dangerous if your memory isnt managed correctly
static void modeItemCleanup(Mode prevMode, APP) {
  g_print("== SYS == Cleaning Up Individiually\n");
  AUTO children = gtk_container_get_children(app->ui.display);
  AUTO head = children;
  g_assert(prevMode.clean != NULL);

  while (children != NULL) {
    AUTO child = children->data;
	Result* res = g_object_get_data(child, "__resptr");
    if (!(res->flags & NO_CLEANUP))
			prevMode.clean(res);
	children = children->next;
  }
  g_print("== SYS == Finished cleaning\n");
  g_list_free_full(head, gtk_widget_destroy);
  g_assert(GTK_IS_WIDGET(app->ui.display));
}

void modeInit(Mode prevMode, APP_MUT) {
  g_print("== SYS == Changing mode to %s\n", app->currentMode.label);
  if (GTK_IS_WIDGET(app->ui.display)) {
    if (prevMode.metadata.type & CLEAN) {
		printf("== SYS == Calling cleaning function of %s\n", prevMode.label);
		modeItemCleanup(prevMode, app);
	}
    /* gtk_widget_destroy(app->ui.display); */
	gtk_container_remove(app->ui.scroll, app->ui.display);
	app->ui.display = NULL;
  }

  enum ModeType type = app->currentMode.metadata.type;
  // g_idle_add(updateModeLabel, app);
  updateModeLabel(app);

  if (type & ITEMS) {
    // g_print("Mode is of type items\n");
    modeItems(app);

  } else if (type & COMMAND) {
    // g_print("Mode is of type command\n");
    modeCommand(app);
  } else if (type & ONLY_PREVIEW) {
    modePreview(app);
  }
  // g_print("Finished init code\n");
  // g_idle_add_full(gint priority, GSourceFunc function, gpointer data,
  // GDestroyNotify notify)
}
