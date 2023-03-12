#include "mode.h"

const static Mode modes[] = {
	{
		.metadata = {
			.symbol = "T",
			.type = ITEMS | AUTOCLEAN 
		},
		.label = "EMOJI",
		.generate = emoji_generate,
		.execute = emoji_execute
	},
	{
		.metadata = {
			.symbol = "🚀",
			.type = ITEMS,
		},
		.label = "APP",
		.generate = app_mode_generate,
		.execute = app_mode_execute
	},
	{
		.metadata = {
			.symbol = "💾",
			.type = PROGRESS | COMMAND,
		},
		.label = "DL",
		.execute_command = dl_execute
	},
	{
		.metadata = {
			.symbol = "F",
			.type = ITEMS | HAS_PREVIEW
		},
		.label = "FONT",
		.preview = font_preview,
		.generate = font_generate,
		.execute = NULL
	}
};
