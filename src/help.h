#include "modes/mode.h"
#include "modes/modes-s.h"

GtkWidget* helpPreview(Result*);

const static Mode helpMode = {
		.metadata = {
			.symbol = "💊",
			.type = ONLY_PREVIEW
		},
		.label = "HELP",
		.preview = helpPreview
};
