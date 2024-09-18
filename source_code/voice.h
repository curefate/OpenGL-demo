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

//发送信息给MCI
bool SendToMCI(std::string command)
{
	if (!mciSendString(command.c_str(), NULL, 0, 0))
		return true;
	return false;
}

//从MCI获取信息
std::string GetFromMCI(std::string command)
{
	char message[20];
	mciSendString(command.c_str(), message, 20, 0);
	std::string str(message);
	return str;
}

//音乐线程的方法
unsigned __stdcall ThreadPlayMusic(LPVOID lpParameter)
{
	std::string filepath = "歌唱祖国.mid";
	std::string Open = "OPEN " + filepath + " ALIAS MUSIC";
	std::string Close = "CLOSE MUSIC";
	std::string Status = "status MUSIC mode";
	std::string Play = "PLAY MUSIC FROM 0";
	SendToMCI(Open);	//打开音乐文件
	while (true)
	{
		Sleep(1);
		std::string result = GetFromMCI(Status);	//获取音乐状态
		if (result == "stopped")
			SendToMCI(Play);	//如果音乐停止，重新播放
		if (g_bEndMusicThread == true)		//接收主线程发送的退出信号
			break;
	}
	SendToMCI(Close);	//关闭音乐文件
	return 0;
}

//入口函数
int main(int argc, char* argv[])
{
	g_bEndMusicThread = false;	//初始化子线程标志位
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadPlayMusic, NULL, 0, NULL);	//创建线程

	g_bEndMusicThread = true;	//通知子线程退出
	WaitForSingleObject(hThread, INFINITE);		//线程结束之后再释放资源
	return 0;
}

#endif // !VOICE_H
