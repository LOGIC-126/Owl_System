#include "Command.h"


void GetCommand(char* command,struct Commandlists *list)
{	
	const char delims[] = " "; // 空格作为分隔符
	char *token;
	static uint8_t ComState = 0;		//定义表示当前状态机状态的静态变量
	memset(list->head, 0, sizeof(list->head));  // 清空字符串
	memset(list->mid, 0, sizeof(list->mid));  // 清空字符串
	memset(list->detail, 0, sizeof(list->detail));  // 清空字符串
	// 获取第一个子串
    token = strtok(command, delims);
    while (token != NULL) {
//        printf("Token: %s\n", token);
		if (ComState == 0)
		{
			strcpy(list->head,token);
			ComState = 1;	
		}
		else if (ComState == 1)
		{
			strcpy(list->mid,token);
			ComState = 2;
		}
		else if (ComState == 2)
		{
			strcpy(list->detail,token);
			ComState = 0;
		}
        // 继续获取后续子串
        token = strtok(NULL, delims);
    }
//	command[0] = '\0';
//	memset(command, 0, sizeof(*command));  // 清空Command数组
//	printf("Head: %s,Mid: %s,Detail: %s\n",list->head,list->mid,list->detail);
}



//........................end of code..............................
