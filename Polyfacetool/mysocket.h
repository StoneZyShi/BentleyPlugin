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

	//��ʼ��������,����0��ʾ�ɹ�  
	int Init(const char* address, int port);

	//��������  
	void Run();


private:
	SOCKET m_sock;
};


//void main()
//{
//	CClientNet client;
//
//	//���ӵ�127.0.0.1�������أ�,�˿ں�Ϊ8888�ķ����
//	printf("Connect\n");
//	client.Connect(8888, "127.0.0.1");
//
//	//������Ϣ
//	printf("send msg\n");
//	client.SendMsg("hello\0", sizeof("hello\0"));
//
//	//�ر�socket
//	printf("close\n");
//	client.Close();
//
//	system("pause");
//}
class CClientNet
{
public:
	//������ָ��������
	int Connect(int port, const char* address);
	//������Ϣ
	int SendMsg(const char* msg, int len);
	//�ر�
	void Close();

private:
	SOCKET m_sock;
};
