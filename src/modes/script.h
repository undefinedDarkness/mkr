#ifndef SCRIPT_MODE_H
#define SCRIPT_MODE_H

#include "../app.h"

enum ResultHint {
	REST_NONE,
	REST_FILEPATH,
	REST_APPNAME
};

struct CommandData {
	char** envp; // currently not used
	char*  workingDirectory;
	char** argv;
	enum ResultHint hint;
};
void script_generate(API);
#endif 
