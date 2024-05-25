#include "app.h"
#include "gdk/gdkkeysyms.h"
#include "modes/mode.h"
/* #include "modes/modes-s.h" */
#include "help.h"

// void update_mode(char p, APP_MUT);

static bool on_window_key_press(GtkWidget *window, GdkEventKey *key, APP) {
  if (key->keyval == GDK_KEY_Escape) {
    if (!g_key_file_get_boolean(app->config, "OPERATION", "daemonMode", NULL)) {
		QUIT();
} else {
		toggle_display(0);	
		}
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
  else if (k == GDK_KEY_F1) {
  	AUTO prev = app->currentMode;
	app->currentMode = helpMode;
	modeInit(prev, app);
  }
  else if (key->state & GDK_CONTROL_MASK) {
    char c = gdk_keyval_to_unicode(k);
    g_print("%c\n\n", c);

    if (c == 'V')
      return false;

    Mode prev = app->currentMode;
    for (int i = 0; i < sizeof(modes) / sizeof(modes[0]); i++) {
      if (c == modes[i].key)
        app->currentMode = modes[i];
    }

    modeInit(prev, app);
  }

  return false; // dont propagate
}

static void on_input_changed(GtkWidget *input, APP_MUT) {
  if (app->currentMode.metadata.type & CLEAR_ON_EDIT){
  
	GList *children = gtk_container_get_children(app->ui.display);
	GList* lh = children;
	while (children != NULL) {
		/* g_assert(GTK_IS_WIDGET(children->data)); */
		gtk_widget_destroy(children->data); // TODO: hook into cleanup system
		children = children->next;
	}
	g_list_free(lh);


  }
	if (app->currentMode.metadata.type & UPDATE_ON_EDIT) {
    app->currentMode.generate(&app->api);
  }
  if (app->currentMode.metadata.type & ITEMS) {
  app->search = gtk_entry_get_text(input);
  gtk_flow_box_invalidate_sort(app->ui.display);
  update_selected(0xAA, app);
  }
}

static void on_input_focus_lost(GtkWidget *input, GdkEventFocus event,
                                void *n) {
  gtk_widget_grab_focus(input);
}

// NOTE: Maybe this function could be gotten from a loaded library to allow custom libraries.
void layout_rofi(GtkWidget *window, APP_MUT) {

/*
 *   | LAYOUT ---------------------------------------------- |
 *   |                                                       |
 *   | | HEADER ---------------------| PREVIEW ----------- | |
 *   | |                             |                     | |
 *   | | | MODE | INPUT -----------| |                     | |
 *   | | | XXX | XXXXXXXXXXXXXXXXX | |                     | |
 *   | | |-------------------------| |                     | |
 *   | |-----------------------------|                     | |
 *   |                               |                     | |
 *   | | CONTENTS -------------------|                     | |
 *   | |                             |                     | |
 *   | | *** CONTENTS DETERMINED BY  |                     | |
 *   | | *** CURRENTLY ACTIVE MODE   |                     | |
 *   | |-----------------------------|---------------------| |
 *   |                                
 *   |-------------------------------------------------------|
 * */

  g_signal_connect(window, "key-press-event", on_window_key_press, app);
  // AUTO windowLayout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  // ADD(window, windowLayout);

  AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  CLASS(layout, "layout");

  AUTO preview = gtk_frame_new(""); // gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  CLASS(preview, "preview");
  gtk_frame_set_shadow_type(preview, GTK_SHADOW_NONE);
  // gtk_box_pack_end(windowLayout, preview, true, true, 0);

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

  AUTO contents = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  ADD(contents, layout);

  AUTO scroll = gtk_scrolled_window_new(NULL, NULL);
  CLASS(scroll, "items-viewport");
  ADD(layout, scroll);

  ADD(contents, preview);
  ADD(window, contents);

  app->ui = (UI){.window = window,
                 .scroll = scroll,
                 .input = input,
                 .mode = mode,
                 .preview = preview,
                 .display = NULL};
}


// NOTE: Maybe this function could be gotten from a loaded library to allow custom libraries.
void layout_dmenu(GtkWidget *window, APP_MUT) {
  g_signal_connect(window, "key-press-event", on_window_key_press, app);
  // AUTO windowLayout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  // ADD(window, windowLayout);

  AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  CLASS(layout, "layout");

  AUTO preview = gtk_frame_new(""); // gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  CLASS(preview, "preview");
  gtk_frame_set_shadow_type(preview, GTK_SHADOW_NONE);
  // gtk_box_pack_end(windowLayout, preview, true, true, 0);

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

  AUTO contents = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  ADD(contents, layout);

  AUTO scroll = gtk_scrolled_window_new(NULL, NULL);
  CLASS(scroll, "items-viewport");
  ADD(layout, scroll);

  ADD(contents, preview);
  ADD(window, contents);

  app->ui = (UI){.window = window,
                 .scroll = scroll,
                 .input = input,
                 .mode = mode,
                 .preview = preview,
                 .display = NULL};
}

void scaffold(GtkWidget *window, APP_MUT) {
	const AUTO userLayout = C_GET("WINDOW", "layout", string, "rofi");
	printf("== LAYOUT == Using layout: %s", userLayout);
	if (g_ascii_strcasecmp(userLayout, "rofi") == 0) {
		layout_rofi(window, app);
	} else if (g_ascii_strcasecmp(userLayout, "rofi") == 0) {
		layout_dmenu(window, app);
	}
}
