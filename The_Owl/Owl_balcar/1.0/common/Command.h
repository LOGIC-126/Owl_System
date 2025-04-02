#ifndef __COMMAND_H
#define __COMMAND_H

#include "allconfig.h"

struct Commandlists
{
	char head[3];
	char mid[10];
	char detail[120];
};

void GetCommand(char* command,struct Commandlists *list);
void setCmd(char* head,char* mid,char* detail);




#endif
