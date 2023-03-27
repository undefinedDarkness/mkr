#ifndef SCRIPT_MODE_H
#define SCRIPT_MODE_H

#include "../app.h"

struct CommandData {
	char** envp; // currently not used
	char*  workingDirectory;
	char** argv;
};
void script_generate(API);
#endif 
