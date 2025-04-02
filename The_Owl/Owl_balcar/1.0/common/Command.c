#include "Command.h"

static void setmidCmd_cd(char* mid,char* detail);
static void setmidCmd_mv(char* mid,char* detail);
static void setmidCmd_rp(char* mid,char* detail);
static void setmidCmd_help(void);

 
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
	else if(strcmp(head, "hp") == 0)
	{
		setmidCmd_help();
	}
	else
		printf("ERROR:No command named %s\n",head);
}

static void setmidCmd_cd(char* mid,char* detail)
{
	if (strcmp(mid, "-pids") == 0)
	{
		if (detail[0] == '[')
		{
			int count=0;
			float datas[12];
			const char delims[] = ",[]"; // 逗号作为分隔符
			char *token;
			// 获取第一个子串
			token = strtok(detail, delims);
			while (token != NULL) 
			{
				float number = strtof(token, NULL); // 使用 strtof 转换为单精度浮点数
				datas[count++] = strtof(token, NULL);
//				printf("<debug>Token: %.2f\n", number);
				token = strtok(NULL, delims);
			}
			Balance_Kp = datas[0];
			Balance_Kd = datas[2];
			
			Velocity_Kp = datas[3];
			Velocity_Ki = datas[4];
			
			Turn_Kp = datas[6];
			Turn_Kd = datas[8];
		}
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
	else if(strcmp(mid, "-save") == 0)
	{
		SavePID();
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
			printf("Car start!\n");
		else
			printf("Car stop!\n");
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
		MPU6050_Flag = !MPU6050_Flag;
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

static void setmidCmd_help(void)
{
    printf("Available commands:\n");
    printf("  cd [option] [detail] - Change PID parameters\n");
    printf("    -pids [Kp,Kd,VelKp,VelKi,TurnKp,TurnKd] - Set all PID parameters\n");
    printf("    -pid1 - Set PID parameters for specific system 1\n");
    printf("    -pid2 - Set PID parameters for specific system 2\n");
    printf("    -pid3 - Set PID parameters for specific system 3\n");
    printf("    -pid4 - Set PID parameters for specific system 4\n");
    printf("    -save - Save current PID settings\n");
    printf("  mv [option] - Move the vehicle\n");
    printf("    -f - Move forward\n");
    printf("    -b - Move backward\n");
    printf("    -l - Turn left\n");
    printf("    -r - Turn right\n");
    printf("    -s - Toggle start/stop state of the car\n");
    printf("  rp [option] - Report datas packet\n");
    printf("    -s - Report SBUS sensor\n");
    printf("    -m - Report MPU6050 sensor\n");
    printf("    -a - All report (details not specified)\n");
    printf("    -pid - Report current PID settings\n");
    printf("  hp - Display this help message\n");
}



//........................end of code..............................
