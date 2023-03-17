#ifndef FONT_MODE_H
#define FONT_MODE_H

#include "mode.h"

void font_clean(Result*);
void font_generate(API);
GtkWidget* font_preview(Result*);
void font_execute(const char*, Result); 


#endif 
