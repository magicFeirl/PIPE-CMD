#include <stdio.h>
#include <string.h>
#include <windows.h>


//�ܵ���CMDͨ�ż�ʵ��
//.C��׺�����˸����񴥴���

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
		printf("�����ܵ�ʧ��!\nErrorCode:%u",GetLastError());
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
		printf("��������ʧ��!\nErrorCode:%u\n",GetLastError());
		return;
	}

	//���ر�д����ᵼ��anr����
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
	puts("�������ָ��...\n����exit�˳���");
	do{
		ZeroMemory(cmdl,sizeof(cmdl));
		printf(">");
		gets(cmdl);
		cmd(cmdl);
	}while(strcmp(cmdl,"exit"));
	return 0;
}

//������ͼ������