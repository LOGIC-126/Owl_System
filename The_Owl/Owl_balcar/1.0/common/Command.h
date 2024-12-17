#ifndef __COMMAND_H
#define __COMMAND_H

#include "allconfig.h"

struct Commandlists
{
	char head[3];
	char mid[5];
	char detail[12];
};

void GetCommand(char* command,struct Commandlists *list);




#endif
