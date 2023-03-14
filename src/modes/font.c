#include "font.h"

GtkWidget* font_preview(Result *selectedResult) {
	AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	PangoFontFamily *family = selectedResult->metadata;
	PangoFontFace **faces; int n_faces;
	pango_font_family_list_faces(family, &faces, &n_faces);
	for (int i = 0; i < n_faces; i++) {
		AUTO face = faces[i];
		if (pango_font_face_is_synthesized(face))
			continue;
		AUTO label = gtk_label_new(""); 
		gtk_label_set_markup(label, 
				g_strdup_printf("<span font=\"%s %s\">%s</span>", 
					pango_font_family_get_name(family),
					pango_font_face_get_face_name(face), 
					pango_font_face_get_face_name(face)
					));
		gtk_label_set_xalign(label,0);
		ADD(layout, label);
	}
	AUTO alphabet = gtk_label_new("");
	gtk_label_set_markup(alphabet, g_strdup_printf("<span font=\"%s\">Whereas disregard and contempt for human rights have resulted </span>", pango_font_family_get_name(family)));
	gtk_label_set_line_wrap(alphabet, true);
	gtk_label_set_line_wrap_mode(alphabet, PANGO_WRAP_WORD);
	gtk_box_pack_end(layout, alphabet, false, false, 0);
	AUTO scw = gtk_scrolled_window_new(NULL, NULL);
	ADD(scw, layout);
	return scw;
}

void font_execute(const char*, Result res) {
	char *fname = pango_font_family_get_name(res.metadata);
	AUTO clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text(clip, fname, -1);
	gtk_clipboard_store(clip);
	g_object_unref(clip);
}

void font_generate(API) {
	int len; PangoFontFamily **families;
	AUTO temp = gtk_label_new("");
	PangoContext *ctx = gtk_widget_get_pango_context(temp);
	pango_context_list_families(ctx, &families, &len);
	GList*list = NULL;

	// NOTE: families isnt freed here, it is used later but never freed completley.
	for (int i = 0; i<len;i++){
		Result *r = g_slice_alloc0(sizeof(Result));//calloc(1,sizeof(Result));
		AUTO family = families[i];
		r->label = pango_font_family_get_name(family);
		r->metadata = family;

		AUTO box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
		g_object_set_data(box, "__resptr", r);
		AUTO fname = gtk_label_new(""); CLASS(fname, "font-family-name"); ADD(box,fname);
		gtk_label_set_markup(fname, g_strdup_printf("<span font=\"%s\">%s</span>", r->label, r->label));
		gtk_label_set_xalign(fname,0);
		AUTO row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0); gtk_box_pack_end(box,row,false,false,0);
		
		if (pango_font_family_is_variable(family)) {
			AUTO variable = gtk_label_new("VARIABLE"); CLASS(variable, "bubble"); ADD(row, variable);
		}
		if (pango_font_family_is_monospace(family)) {
			AUTO monospace = gtk_label_new("MONOSPACE"); CLASS(monospace, "bubble"); ADD(row, monospace);
		}

		list = g_list_prepend(list, box);
		// g_print("%s\n", r->label);
	}

	api->insert_custom_item(g_list_reverse(list), api->data);
}
