#include "app.h"
#include <string.h>
#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "../lib/fts_fuzzy_match.h"
//
// static int fuzzy_sort(GtkWidget*a,GtkWidget*b,char*search) {
//      char* contA = ((Result*)g_object_get_data(a, "__resptr"))->label;
//      contA = g_ascii_strdown(contA, -1);
//      char* contB = ((Result*)g_object_get_data(b, "__resptr"))->label;
//      contB = g_ascii_strdown(contB, -1);
//      size_t scoreA = levenshtein(contA, search);
//      size_t scoreB = levenshtein(contB, search);
//      // printf("Comparing %s (%d) and %s (%d) with %s\n", contA, scoreA, contB, scoreB, search);
//      if (scoreA == scoreB)
//              return 0;
//      else if (scoreA > scoreB)
//              return -1;
//      else
//       return 1;
// }

int sort(GtkWidget * rowA, GtkWidget * rowB, APP)
{
	char *search = gtk_entry_get_text(app->ui.input);
	if (*search == '\0')
		return 0;
	char *a = g_object_get_data(rowA, "__label");
	char *b = g_object_get_data(rowB, "__label");
	int scoreA = 0, scoreB = 0;
	fuzzy_match(search, a, &scoreA);
	fuzzy_match(search, b, &scoreB);
	if (scoreA > scoreB)
		return -1;
	else if (scoreA == scoreB)
		return 0;
	else
		return 1;
}
