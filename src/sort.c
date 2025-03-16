#include "app.h"
#include <string.h>
/* #define FTS_FUZZY_MATCH_IMPLEMENTATION */
#include "../lib/match.h"
#include "glib-object.h"
#include "gtk/gtk.h"


int fuzzy_sort_by_label(GtkWidget * rowA, GtkWidget * rowB, APP)
{	
	const char *a = g_object_get_data(rowA, "__label");
	const char *b = g_object_get_data(rowB, "__label");
	return fuzzy_sort(a, b, app->search, false);
}

bool fuzzy_filter_by_label(GtkWidget* row, APP_MUT) {
	bool f;
	if (app->search[0] == '\0') {
		f = false;
		goto skip;
	}
	f = fuzzy_match(app->search, g_object_get_data(row, "__label"), true);
skip:
	if (!f)
		app->userdata.visibleItems--;
	return f;
}
