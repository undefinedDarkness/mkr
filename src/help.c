#include "modes/modes-s.h"

GtkWidget *helpPreview(Result*_) {
	GtkWidget* layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); CLASS(layout, "help-layout");
	for (int i = 0; i < sizeof(modes)/sizeof(modes[0]); i++) {
		AUTO mode = modes[i];
		AUTO mhbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0); CLASS(mhbox, "help-mode");
		AUTO mvbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
		AUTO mtitle = gtk_label_new(mode.label); CLASS(mtitle, "help-mode-title"); gtk_label_set_xalign(mtitle,0);
		AUTO mdesc = gtk_label_new(mode.metadata.desc); CLASS(mdesc, "help-mode-desc");
		AUTO micon = gtk_label_new(mode.metadata.symbol); CLASS(micon, "help-mode-icon");
		ADD(mvbox, mtitle); ADD(mvbox, mdesc);
		ADD(mhbox, micon);
		ADD(mhbox,mvbox);
		ADD(layout,mhbox);
	}
	return layout;
}
