#include "Command.h"

static void setmidCmd_cd(char* mid,char* detail);
static void setmidCmd_mv(char* mid,char* detail);
static void setmidCmd_rp(char* mid,char* detail);

 
void GetCommand(char* command,struct Commandlists *list)
{	
	const char delims[] = " "; // 空格作为分隔符
	char *token;
	static uint8_t ComState = 0;		//定义表示当前状态机状态的静态变量
	ComState = 0;
	memset(list->head, 0, sizeof(list->head));  // 清空字符串
	memset(list->mid, 0, sizeof(list->mid));  // 清空字符串
	memset(list->detail, 0, sizeof(list->detail));  // 清空字符串
	// 获取第一个子串
    token = strtok(command, delims);
    while (token != NULL) {
//        printf("<debug>Token: %s\n", token);
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
//	printf("<debug>Head: %s,Mid: %s,Detail: %s\n",list->head,list->mid,list->detail);
}

void setCmd(char* head,char* mid,char* detail)
{
	if (strcmp(head, "cd") == 0)
	{
		setmidCmd_cd(mid,detail);
	}
	else if(strcmp(head, "mv") == 0)
	{
		setmidCmd_mv(mid,detail);
	}
	else if(strcmp(head, "rp") == 0)
	{
		setmidCmd_rp(mid,detail);
	}
	else
		printf("ERROR:No command named %s\n",head);
}

static void setmidCmd_cd(char* mid,char* detail)
{
	if (strcmp(mid, "-pids") == 0)
	{
		
	}
	else if(strcmp(mid, "-pid1") == 0)
	{

	}
	else if(strcmp(mid, "-pid2") == 0)
	{

	}
	else if(strcmp(mid, "-pid3") == 0)
	{

	}
	else if(strcmp(mid, "-pid4") == 0)
	{

	}
	else
		printf("ERROR:cd command have no %s\n",mid);
}

static void setmidCmd_mv(char* mid,char* detail)
{
	if (strcmp(mid, "-f") == 0)
	{
		
	}
	else if(strcmp(mid, "-b") == 0)
	{

	}
	else if(strcmp(mid, "-l") == 0)
	{

	}
	else if(strcmp(mid, "-r") == 0)
	{

	}
	else if(strcmp(mid, "-s") == 0)
	{
		Stop_Flag = !Stop_Flag;
		if (Stop_Flag == 0)
			printf("Car start!");
		else
			printf("Car stop!");
	}
	else
		printf("ERROR:mv command have no %s\n",mid);
}

static void setmidCmd_rp(char* mid,char* detail)
{
	if (strcmp(mid, "-s") == 0)
	{
		
	}
	else if(strcmp(mid, "-m") == 0)
	{

	}
	else if(strcmp(mid, "-a") == 0)
	{

	}
	else if(strcmp(mid, "-pid") == 0)
	{
		printf(">>> Balance_Kp:%.2f  ",Balance_Kp);
		printf("Balance_Kd:%.2f  ",Balance_Kd);
		                    
		printf("Velocity_Kp:%.2f  ",Velocity_Kp);
		printf("Velocity_Ki:%.2f  ",Velocity_Ki);
		                    
		printf("Turn_Kp:%.2f  ",Turn_Kp);
		printf("Turn_Kd:%.2f  \n",Turn_Kd);
	}
	else
		printf("ERROR:rp command have no %s\n",mid);
}

void CodeCommand(struct Commandlists cclist)
{
	

}


//........................end of code..............................
