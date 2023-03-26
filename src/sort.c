#include "app.h"
#include <string.h>
/* #define FTS_FUZZY_MATCH_IMPLEMENTATION */
#include "../lib/match.c"


int sort(GtkWidget * rowA, GtkWidget * rowB, APP)
{	
	const char *a = g_object_get_data(rowA, "__label");
	const char *b = g_object_get_data(rowB, "__label");
	return fuzzy_sort(a, b, app->search, true);
}
