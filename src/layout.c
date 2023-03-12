#include "app.h"
#include "gdk/gdkkeysyms.h"
#include "modes/modes-s.h"

// void update_mode(char p, APP_MUT);

static bool on_window_key_press(GtkWidget *window, GdkEventKey *key, APP) {
  if (key->keyval == GDK_KEY_Escape) {
    QUIT();
  }

  return false;
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
    g_print("%c\n\n", c);

    if (c == 'V')
      return false;

    Mode prev = app->currentMode;
    for (int i = 0; i < sizeof(modes) / sizeof(modes[0]); i++) {
      if (c == *modes[i].label)
        app->currentMode = modes[i];
    }

    modeInit(prev, app);
  }

  return false; // dont propagate
}

static void on_input_changed(GtkWidget *input, APP) {
  if (!(app->currentMode.metadata.type & ITEMS))
    return;
  gtk_list_box_invalidate_sort(app->ui.display);
  update_selected(0xAA, app);
}

static void on_input_focus_lost(GtkWidget *input, GdkEventFocus event,
                                void *n) {
  gtk_widget_grab_focus(input);
}

void scaffold(GtkWidget *window, APP_MUT) {
  g_signal_connect(window, "key-press-event", on_window_key_press, app);
	AUTO windowLayout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	ADD(window, windowLayout);

  AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  ADD(windowLayout, layout);
  CLASS(layout, "layout");

  AUTO preview = gtk_frame_new("");//gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	CLASS(preview, "preview");
	gtk_frame_set_shadow_type(preview, GTK_SHADOW_NONE);
gtk_box_pack_end(windowLayout, preview, true, true, 0);
	// ADD(windowLayout, preview);
	


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

  // AUTO favourites = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  // CLASS(favourites, "favourites");
  // ADD(layout, favourites);

  AUTO scroll = gtk_scrolled_window_new(NULL, NULL);
  // gtk_widget_hide(gtk_scrolled_window_get_hscrollbar(scroll));
  // gtk_box_pack_end(layout, scroll, true, true, 0);
  gtk_box_pack_start(layout, scroll, false, false, 0);
  // ADD(layout, scroll);

  //  AUTO display = gtk_list_box_new();
  //  gtk_list_box_set_sort_func(display, sort, app, NULL);
  // gtk_list_box_set_selection_mode(display, GTK_SELECTION_SINGLE);
  //  CLASS(display, "display");
  //  ADD(scroll, display);

  app->ui = (UI){.window = window,
                 .scroll = scroll,
                 .input = input,
                 .mode = mode,
				 .preview = preview, 
                 .display = NULL};
}
