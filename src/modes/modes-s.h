#include "hermes.h"
#include "generic.h"
#include "mode.h"
#include "app.h"
#include "emoji.h"
#include "font.h"
#include "script.h"
#include "math.h"

const static Mode modes[] = {
	{
		.metadata = {
			.symbol = "🚀",
			.type = ITEMS,
		},
		.label = "APP",
		.key = 'r',
		.generate = app_generate,
		.execute = app_execute
	},

	{
		.metadata = {
			.symbol = "T",
			.type = ITEMS | CLEAN 
		},
		.label = "EMOJI",
		.key = 'u',
		.clean = emoji_clean,
		.generate = emoji_generate,
		.execute = emoji_execute
	},
	{
		.metadata = {
			.symbol = "💾",
			.type = PROGRESS | COMMAND,
		},
		.label = "DL",
		.key = 'd',
		.execute_command = dl_execute
	},
	{
		.metadata = {
			.symbol = "F",
			.type = ITEMS | HAS_PREVIEW | CLEAN
		},
		.label = "FONT",
		.key = 'F',
		.preview = font_preview,
		.generate = font_generate,
		.clean = font_clean,
		.execute = copy_label_to_clipboard //, Result)
	},
	/*{
		.metadata = {
			.symbol = "🏠",
			.type = ONLY_PREVIEW
		},
		.key = 'h',
		.label = "DASHBOARD",
		.preview = dashboard	
	},*/
	{
	.metadata = {
		.symbol = "⚡",
		.type = ONLY_PREVIEW | UPDATE_ON_EDIT
	},
	.label = "HERMES",
	.key = 'h',
	.preview = hermes_preview
},
	{
		.metadata = {
			.symbol = "M",
			.type = ONLY_PREVIEW | UPDATE_ON_EDIT
		},
		.key = 'm',
		.label = "MATH",
		.generate = math_mode_update,
		.preview = math_mode_preview
	},
};
