
#include "mode.h"
#include "../app.h"
#include "../../lib/expr.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
// Alias for expr.h


static struct expr_var_list vars = {
	.head = NULL
};

struct unit {
	char name[20];
	void (*run)();
};

// im not dealing with it
static GtkWidget *label;
static GtkWidget* units;

#define exprWrapper(fn) static float fn##_expr(struct expr_func *f, vec_expr_t *args, void*c) {return fn(expr_eval(&vec_nth(args,0)));}
#define exprCWrapper(name, fn) static float name##_expr(struct expr_func *f, vec_expr_t *args, void*c) {return fn(expr_eval(&vec_nth(args,0)));}
exprWrapper(sinf);

// called before any update events...
GtkWidget* math_mode_preview(void*) {

    AUTO pi = expr_var(&vars, "PI", 2);
	pi->value = M_PI;

	AUTO e = expr_var(&vars, "E", 1);
	e->value = M_E;

	AUTO layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous(layout, false);
	
	label = gtk_label_new("");
	ADD(layout,label);

	units = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(layout,units,false, false,0);

	AUTO inputRow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	AUTO numInput = gtk_spin_button_new_with_range(0, INFINITY, 1);			
	/* AUTO unitInput = gtk_drop_down */ 

	return layout;
};

exprWrapper(cosf);
exprWrapper(logf);
exprWrapper(tanf);
exprWrapper(log10f);
exprCWrapper(secf, 1/cosf);
exprCWrapper(cosecf, 1/sinf);
exprCWrapper(cotf, 1/tanf);
static struct expr_func user_funcs[] = {
	{ "sin", sinf_expr, NULL, 0 },
	{ "cos", cosf_expr, NULL, 0	},
	{ "tan", tanf_expr, NULL, 0 },
	{ "sec", secf_expr, NULL, 0 },
	{ "cosec", cosecf_expr, NULL, 0 },
	{ "cot", cotf_expr, NULL, 0 },
	{ "ln", logf_expr, NULL, 0 },
	{ "log", log10f_expr, NULL, 0 },
	{0}
};

void math_mode_update(API) {
	// TODO: Add a proper way for accesing parts of UI from here....
	// TODO: Make some provision for sandboxing ideally the whole program wouldnt crash cuz
	// of some badly written code...
	// I'm not really a idealistic person though... 

	AUTO state = (State*)api->data;
	const char *expression = gtk_entry_get_text(state->ui.input);

	struct expr *expr = expr_create(expression, strlen(expression), &vars, user_funcs);
	// parsing failed...
	if (expr == NULL)
		return;
	char buf[50];
	snprintf(buf, 50, "%.5f", expr_eval(expr));
	gtk_label_set_text(label, buf); // pretty sure it copies here
	expr_destroy(expr, NULL);
	/* math_unit_handle(expression); */
}


