#include "mode.h"
#include "app.h"
#include "emoji.h"
#include "font.h"
#include "script.h"

const static Mode modes[] = {
	{
		.metadata = {
			.symbol = "T",
			.type = ITEMS | CLEAN 
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
		.generate = app_generate,
		.execute = app_execute
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
	},
	{
		.metadata = {
			.symbol = "🏠",
			.type = ONLY_PREVIEW
		},
		.label = "DASHBOARD",
		.preview = dashboard	
	},

	// TEMP
	{
		.metadata = {
			.symbol = "R",
			.type = ITEMS
		},
		.label = "BIN",
		.payload = &(struct CommandData){
			.envp = NULL,
			.workingDirectory = NULL,
			.argv = (char*[2]){"dmenu_path",NULL}
		},
		.generate = script_generate,	
	},
};
