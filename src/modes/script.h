#ifndef SCRIPT_MODE_H
#define SCRIPT_MODE_H

#include "../app.h"

struct CommandData {
	char** argv;
	char** envp; // currently not used
	char*  workingDirectory;
};
void script_generate(API);
#endif 
