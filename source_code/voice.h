#pragma once
#ifndef VOICE_H
#define VOICE_H

#include <iostream>
#include <thread>
#include <tchar.h>
#include <windows.h>
//#include <mmsystem.h>
//#include <dsound.h>
#pragma comment(lib,"Winmm.lib")

using namespace std;

volatile bool g_bEndMusicThread;

//������Ϣ��MCI
bool SendToMCI(std::string command)
{
	if (!mciSendString(command.c_str(), NULL, 0, 0))
		return true;
	return false;
}

//��MCI��ȡ��Ϣ
std::string GetFromMCI(std::string command)
{
	char message[20];
	mciSendString(command.c_str(), message, 20, 0);
	std::string str(message);
	return str;
}

//�����̵߳ķ���
unsigned __stdcall ThreadPlayMusic(LPVOID lpParameter)
{
	std::string filepath = "�質���.mid";
	std::string Open = "OPEN " + filepath + " ALIAS MUSIC";
	std::string Close = "CLOSE MUSIC";
	std::string Status = "status MUSIC mode";
	std::string Play = "PLAY MUSIC FROM 0";
	SendToMCI(Open);	//�������ļ�
	while (true)
	{
		Sleep(1);
		std::string result = GetFromMCI(Status);	//��ȡ����״̬
		if (result == "stopped")
			SendToMCI(Play);	//�������ֹͣ�����²���
		if (g_bEndMusicThread == true)		//�������̷߳��͵��˳��ź�
			break;
	}
	SendToMCI(Close);	//�ر������ļ�
	return 0;
}

//��ں���
int main(int argc, char* argv[])
{
	g_bEndMusicThread = false;	//��ʼ�����̱߳�־λ
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadPlayMusic, NULL, 0, NULL);	//�����߳�

	g_bEndMusicThread = true;	//֪ͨ���߳��˳�
	WaitForSingleObject(hThread, INFINITE);		//�߳̽���֮�����ͷ���Դ
	return 0;
}

#endif // !VOICE_H
