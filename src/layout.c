#include "app.h"
#include "gdk/gdkkeysyms.h"

// void update_mode(char p, APP_MUT);

static bool on_window_key_press(GtkWidget *window, GdkEventKey *key, APP) {
  if (key->keyval == GDK_KEY_Escape) {
    QUIT();
  }

  return false;
}

// WARN: Trigger happy
void update_progress(float v, void *ptr) {
  APP = ptr;
  static GtkWidget *progress;
  // NOTE: v is a fraction not a percentage
  if (v == 0) {
    AUTO scroll = app->ui.scroll;
    progress = gtk_progress_bar_new();
    gtk_widget_set_vexpand(progress, true);
    gtk_widget_set_valign(progress, GTK_ALIGN_FILL);
    gtk_widget_hide(app->ui.input);
    gtk_box_pack_start(app->ui.display, progress, true, true, 0);
    gtk_widget_show_all(scroll);
  }
  gtk_progress_bar_set_fraction(progress, v);
  if (v == 1) {
    gtk_widget_destroy(progress);
    if (C_GET("BEHAVIOUR", "exitOnExecute", boolean, true))
      QUIT();
  }
}

static void execute(APP) {
  AUTO type = app->currentMode.metadata.type;
  bool quit = C_GET("OPERATION", "exitOnExecute", boolean, true);
  if (type & ITEMS) {
    // AUTO children = gtk_container_get_children(app->ui.display);
    AUTO selected = gtk_list_box_get_selected_row(app->ui.display);
    Result *result = g_object_get_data(selected, "__resptr");
    app->currentMode.execute(*result);
    // g_list_free(children);

    if (quit)
      QUIT();
  } else if (type & COMMAND) {
    if (type & PROGRESS) {
      app->currentMode.execute_command_with_progress(
          gtk_entry_get_text(app->ui.input), app);
    } else {
      app->currentMode.execute_command(gtk_entry_get_text(app->ui.input));
      if (quit)
        QUIT();
    }
  }
}

static bool on_input_key_press(GtkWidget *input, GdkEventKey *key, APP_MUT) {
  int k = key->keyval;
  AUTO display = app->ui.display;

  if (k == GDK_KEY_Up) {
    update_selected(-1, app);
  } else if (k == GDK_KEY_Down) {
    update_selected(+1, app);
  } else if (k == GDK_KEY_Return)
    execute(app);
  else if (key->state & GDK_CONTROL_MASK) {
    char c = gdk_keyval_to_unicode(gdk_keyval_to_upper(k));
    for (int i = 0; i < sizeof(modes) / sizeof(modes[0]); i++) {
      if (c == *modes[i].label)
        app->currentMode = modes[i];
    }
      modeInit(app);
  }

  return false; // dont propagate
}

static void on_input_changed(GtkWidget *input, APP) {
  gtk_list_box_invalidate_sort(app->ui.display);
  update_selected(0xAA, app);
}

static void on_input_focus_lost(GtkWidget *input, GdkEventFocus event,
                                void *n) {
  gtk_widget_grab_focus(input);
}

void scaffold(GtkWidget *window, APP_MUT) {
  g_signal_connect(window, "key-press-event", on_window_key_press, app);

  AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  ADD(window, layout);
  CLASS(layout, "layout");

  AUTO header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  CLASS(header, "header");
  AUTO mode = gtk_label_new("");
  CLASS(mode, "mode");
  ADD(header, mode);
  AUTO input = gtk_entry_new();
  g_signal_connect(input, "key-press-event", on_input_key_press, app);
  g_signal_connect(input, "changed", on_input_changed, app);
  g_signal_connect(input, "focus-out-event", on_input_focus_lost, NULL);
  gtk_box_pack_start(header, input, true, true, 0);
  ADD(layout, header);

  AUTO favourites = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  CLASS(favourites, "favourites");
  ADD(layout, favourites);

  AUTO scroll = gtk_scrolled_window_new(NULL, NULL);
  // gtk_widget_hide(gtk_scrolled_window_get_hscrollbar(scroll));
  gtk_box_pack_end(layout, scroll, true, true, 0);

  //  AUTO display = gtk_list_box_new();
  //  gtk_list_box_set_sort_func(display, sort, app, NULL);
  // gtk_list_box_set_selection_mode(display, GTK_SELECTION_SINGLE);
  //  CLASS(display, "display");
  //  ADD(scroll, display);

  app->ui = (UI){.window = window,
                 .scroll = scroll,
                 .favourites = favourites,
                 .input = input,
                 .mode = mode,
                 .display = NULL};
}
