#include <stdio.h>
#include <string.h>
#include <windows.h>


//管道与CMD通信简单实例
//.C后缀出现了各种神触错误

void cmd(char *cmdStr)
{
	int ret;
	char cmd[50] = "cmd /c ";
	char retBuf[255] = {0};
	DWORD NumRead;
	HANDLE hReadPipe,hWritePipe;
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;

	strcat(cmd,cmdStr);
	puts(cmd);

	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(sa);

	ret = CreatePipe(&hReadPipe,&hWritePipe,&sa,0);
	if(!ret){
		printf("创建管道失败!\nErrorCode:%u",GetLastError());
		return;
	}

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	si.hStdError = si.hStdOutput = hWritePipe;
	si.wShowWindow = SW_HIDE;

	ret = CreateProcess(NULL,cmd,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);
	
	if(!ret){
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		printf("创建进程失败!\nErrorCode:%u\n",GetLastError());
		return;
	}

	//不关闭写句柄会导致anr？？
	CloseHandle(hWritePipe);
	while(ReadFile(hReadPipe,retBuf,255,&NumRead,0))
	{
		if(NumRead == 0) break;

		printf("%s",retBuf);
		ZeroMemory(retBuf,sizeof(retBuf));
	}
	CloseHandle(hReadPipe);
}

int main(void)
{
	char cmdl[50];
	puts("输入你的指令...\n输入exit退出。");
	do{
		ZeroMemory(cmdl,sizeof(cmdl));
		printf(">");
		gets(cmdl);
		cmd(cmdl);
	}while(strcmp(cmdl,"exit"));
	return 0;
}

//地狱绘图，，，