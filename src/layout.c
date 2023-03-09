#include "app.h"

void update_selected(int, APP_MUT);
void update_mode(char p, APP_MUT);

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
  AUTO type = app->currentMode.type;
  bool quit = C_GET("OPERATION", "exitOnExecute", boolean, true);
  if (type & ITEMS) {
    AUTO children = gtk_container_get_children(app->ui.display);
    AUTO selected = g_list_nth_data(children, app->selectedItem);
    Result *result = g_object_get_data(selected, "__resptr");
    app->currentMode.execute(*result);
    g_list_free(children);

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

  if (k == GDK_KEY_Left)
    update_selected(app->selectedItem - 1, app);
  else if (k == GDK_KEY_Right)
    update_selected(app->selectedItem + 1, app);
  else if (k == GDK_KEY_Return)
    execute(app);
  else if (key->state & GDK_CONTROL_MASK) {
    update_mode(gdk_keyval_to_unicode(gdk_keyval_to_upper(k)), app);
  }

  return false;
}

void scaffold(GtkWidget *window, APP_MUT) {
  g_signal_connect(window, "key-press-event", on_window_key_press, app);

  AUTO layout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  ADD(window, layout);

  AUTO mode = gtk_label_new(app->currentMode.label);
  CLASS(mode, "mode");
  ADD(layout, mode);



  AUTO input = gtk_entry_new();
  g_signal_connect(input, "key-press-event", on_input_key_press, app);
  gtk_box_pack_start(layout, input, false, false, 0);

  AUTO favourites = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  CLASS(favourites, "favourites");
  ADD(layout, favourites);
  
  AUTO scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(scroll, GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
  gtk_widget_hide(gtk_scrolled_window_get_vscrollbar(scroll));
  gtk_widget_hide(gtk_scrolled_window_get_hscrollbar(scroll));
  gtk_box_pack_end(layout, scroll, true, true, 0);

  AUTO display = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  CLASS(display, "display");
  ADD(scroll, display);

  app->ui =
      (UI){.window = window, .scroll = scroll, .favourites = favourites, .input = input, .mode = mode, .display = display};
}
