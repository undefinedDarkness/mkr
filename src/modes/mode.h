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
	char* label; // MUST BE UNIQUE
	GList *(*generate)();
	union {
		void (*execute)(Result);
		void (*execute_command)(const char*);
		void (*execute_command_with_progress)(const char*,void*);
	};
	enum ModeType type;
} Mode;

GList* app_mode_generate();
void app_mode_execute(Result);

void dl_execute(const char* url, void*);

static Mode modes[] = {
	{
		.label = "APP",
		.type = 0 | ITEMS,
		.generate = app_mode_generate,
		.execute = app_mode_execute
	},
	{
		.label = "DL",
		.type = 0 | PROGRESS | COMMAND,
		.generate = NULL,
		.execute_command_with_progress = dl_execute
	}
};
#endif
