#include "mode.h"
#include "../app.h"
#include "../../lib/expr.h"
#include <stdio.h>
// Alias for expr.h


static struct expr_var_list vars = {
	.head = NULL
};
// im not dealing with it
static GtkWidget *label;

// called before any update events...
GtkWidget* math_mode_preview(void*) {

    AUTO pi = expr_var(&vars, "PI", 2);
	pi->value = M_PI;

	AUTO e = expr_var(&vars, "E", 1);
	e->value = M_E;

	label = gtk_label_new("");
	return label;
};

static struct expr_func user_funcs = {0};

void math_mode_update(API) {
	// TODO: Add a proper way for accesing parts of UI from here....
	// TODO: Make some provision for sandboxing ideally the whole program wouldnt crash cuz
	// of some badly written code...
	// I'm not really a idealistic person though... 
	const char *expression = gtk_entry_get_text(((State*)api->data)->ui.input);
	struct expr *expr = expr_create(expression, strlen(expression), &vars, &user_funcs);
	// parsing failed...
	if (expr == NULL)
		return;
	char buf[50];
	snprintf(buf, 50, "%.3f", expr_eval(expr));
	gtk_label_set_text(label, buf); // pretty sure it copies here
	expr_destroy(expr, NULL);
}
