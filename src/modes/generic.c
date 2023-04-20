#include "generic.h"

void copy_label_to_clipboard(const char *label, Result _)
{
	AUTO clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text(clip, label, -1);
	gtk_clipboard_store(clip);
}
