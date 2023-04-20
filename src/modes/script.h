#ifndef SCRIPT_MODE_H
#define SCRIPT_MODE_H

#include "../app.h"

enum CommandFlags {
	FLAG_NONE = 0,
	FLAG_FILEPATH = 1,
	FLAG_APPNAME = 2,
	FLAG_SHELL = 3
};

struct CommandData {
	char** envp; // currently not used
	char*  workingDirectory;
	char** argv;
	enum CommandFlags flags;
};
void script_generate(API);
void script_launch_file(const char*lbl,Result);
#endif 
