#ifndef MODES_H
#define MODES_H

#include "../basic.h"
// void async_update_progress(float,void*);



typedef struct {
	const char* label;
	GIcon* icon; // gtk specific
	void* metadata;
	int id;
} Result;

struct InsertData {
	void* ptr;
	Result res;
};

// bool async_insert_item_single(struct InsertData *i);

	struct State;
typedef struct {
	void (*update_progress)(float v, struct State *);
	void (*insert_item)(GList *, struct State *);
	struct State* data;	
	// void (*insert_item_single)(struct { void* app; Result res; } *i);
} Api_t;
#define API Api_t *api 
enum ModeType {
	COMMAND = 1,
	AUTOCLEAN = 8,
	ITEMS = 2,
	PROGRESS = 4,
};



typedef struct {
   struct {
	   char*symbol; // TODO: Allow any image
		enum ModeType type;
	} metadata;
	char* label; 
	void (*generate)(API);	
	union {
	void (*execute)(const char*, Result);
	void (*execute_command)(const char*, API);
	};
} Mode;

void app_mode_generate(API);
void emoji_generate(API);
void app_mode_execute(const char*const, Result);

void dl_execute(const char* url, API);

const static Mode modes[] = {
	{
		.metadata = {
			.symbol = "T",
			.type = ITEMS 
		},
		.label = "EMOJI",
		.generate = emoji_generate,
		.execute = NULL
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
		.generate = NULL,
		.execute_command = dl_execute
	}
};
#endif
