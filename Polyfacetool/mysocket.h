#pragma once

#include <stdio.h>
//#include<windows.h>
#include<winsock.h>

#pragma comment(lib, "Ws2_32.lib")

//int main(int argc, char **argv)
//{
//	CServerNet serverNet;
//	int iRlt = serverNet.Init("127.0.0.1", 8888);
//	if (iRlt == 0)
//	{
//		pri(L"init ok...\n");
//		serverNet.Run();
//	}
//	else
//		pri(L"serverNet init failed with error : %d\n", iRlt);
//	system("pause");
//}
class CServerNet
{
public:

	//初始化服务器,返回0表示成功  
	int Init(const char* address, int port);

	//更新数据  
	void Run();


private:
	SOCKET m_sock;
};


//void main()
//{
//	CClientNet client;
//
//	//连接到127.0.0.1（即本地）,端口号为8888的服务端
//	printf("Connect\n");
//	client.Connect(8888, "127.0.0.1");
//
//	//发送消息
//	printf("send msg\n");
//	client.SendMsg("hello\0", sizeof("hello\0"));
//
//	//关闭socket
//	printf("close\n");
//	client.Close();
//
//	system("pause");
//}
class CClientNet
{
public:
	//连接上指定服务器
	int Connect(int port, const char* address);
	//发送信息
	int SendMsg(const char* msg, int len);
	//关闭
	void Close();

private:
	SOCKET m_sock;
};
