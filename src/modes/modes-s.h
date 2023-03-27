#include "app.h"
#include "emoji.h"
#include "font.h"
#include "fsearch.h"
#include "generic.h"
#include "hermes.h"
#include "math.h"
#include "mode.h"
#include "script.h"

const static Mode modes[] = {
    {.metadata = {.desc = "File search",
                  .symbol = "S",
                  .type = ITEMS | UPDATE_ON_EDIT | CLEAR_ON_EDIT},
     .label = "FSEARCH",
     .key = 'f',
     .payload = &(struct CommandData){
		 .argv = (char*[4]){"find","-name","[SEARCH]",NULL},
	 	 .workingDirectory = (void*)1// g_get_home_dir() 
	 },
	 .generate = script_generate,
	},
    {.metadata =
         {
             .desc = "App Launcher",
             .symbol = "🚀",
             .type = ITEMS,
         },
     .label = "APP",
     .key = 'r',
     .generate = app_generate,
     .execute = app_execute},

    {.metadata = {.desc = "Emoji Picker", .symbol = "T", .type = ITEMS | CLEAN},
     .label = "EMOJI",
     .key = 'u',
     .generate = emoji_generate,
     .execute = emoji_execute},
    {.metadata =
         {
             .desc = "Downloader",
             .symbol = "💾",
             .type = PROGRESS | COMMAND,
         },
     .label = "DL",
     .key = 'd',
     .execute_command = dl_execute},
    {
        .metadata = {.symbol = "F",
                     .desc = "Font Preview",
                     .type = ITEMS | HAS_PREVIEW | CLEAN},
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
    {.metadata = {.symbol = "⚡",
                  .desc = "Fast Launcher",
                  .type = ONLY_PREVIEW | UPDATE_ON_EDIT},
     .label = "HERMES",
     .key = 'h',
     .preview = hermes_preview},
    {.metadata = {.symbol = "M",
                  .desc = "Calculations And Units",
                  .type = ONLY_PREVIEW | UPDATE_ON_EDIT},
     .key = 'm',
     .label = "MATH",
     .generate = math_mode_update,
     .preview = math_mode_preview},
};
