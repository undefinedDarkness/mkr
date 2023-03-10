#ifndef MODES_H
#define MODES_H

#include "../basic.h"
void update_progress(float,void*);

enum ModeType {
	COMMAND = 1,
	ITEMS = 2,
	PROGRESS = 4
};

typedef struct {
	const char* label;
	GIcon* icon; // gtk specific
	void* metadata;
	int id;
} Result;


typedef struct {
   struct {
	   char* symbol; // TODO: Allow any image
		enum ModeType type;
		uint32_t color;
	} metadata;
	char* label; 
	GList *(*generate)();
	union {
		void (*execute)(Result);
		void (*execute_command)(const char*);
		void (*execute_command_with_progress)(const char*,void*);
	};
} Mode;

GList* app_mode_generate();
void app_mode_execute(Result);

void dl_execute(const char* url, void*);

const static Mode modes[] = {
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
		.generate = NULL,
		.execute_command_with_progress = dl_execute
	}
};
#endif
