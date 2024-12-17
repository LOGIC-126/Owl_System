#include "Command.h"


void GetCommand(char* command)
{	
	const char delims[] = " "; // 空格作为分隔符
	char *token;
	// 获取第一个子串
    token = strtok(command, delims);
    while (token != NULL) {
        printf("Token: %s\n", token);
    
        // 继续获取后续子串
        token = strtok(NULL, delims);
    }
}



//........................end of code..............................
