#include "mysocket.h"
#include "utility.h"



int CServerNet::Init(const char* address, int port)
{
	int rlt = 0;

	//用于记录错误信息，并输出  
	int iErrorMsg;

	//初始化WinSock  
	WSAData wsaData;
	iErrorMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);

	if (iErrorMsg != NO_ERROR)
	{
		//初始化WinSock失败  
		//pri(L"wsastartup failed with error : %d\n", iErrorMsg);

		rlt = 1;
		return rlt;
	}

	//创建服务端Socket  
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)

	{
		//创建Socket异常  
		//pri(L"socket failed with error : %d\n", WSAGetLastError());

		rlt = 2;
		return rlt;
	}

	//声明信息  
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = inet_addr(address);

	//绑定  
	iErrorMsg = bind(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (iErrorMsg < 0)
	{
		//绑定失败  
		//printf(L"bind failed with error : %d\n", iErrorMsg);
		rlt = 3;
		return rlt;
	}



	return rlt;
}

void CServerNet::Run()
{
	//公开连接  
	listen(m_sock, 5);

	sockaddr_in tcpAddr;
	int len = sizeof(sockaddr);
	SOCKET newSocket;
	char buf[1024];
	int rval;

	do
	{
		//接收信息  
		newSocket = accept(m_sock, (sockaddr*)&tcpAddr, &len);


		if (newSocket == INVALID_SOCKET)
		{
			//非可用socket  

		}
		else
		{
			//新socket连接  
			//pri(L"new socket connect : %d\n", newSocket);


			//消息处理  
			do
			{
				//pri(L"process\n");
				//接收数据  
				memset(buf, 0, sizeof(buf));
				rval = recv(newSocket, buf, 1024, 0);


				if (rval == SOCKET_ERROR)
					//这应该是个异常，当客户端没有调用closeSocket就直接退出游戏的时候，将会进入这里  
					printf("recv socket error\n");



				if (rval == 0)
					//recv返回0表示正常退出  
					printf("ending connection");
				//else
					//显示接收到的数据  
					//pri(L"recv %s\n", buf);


			} while (rval != 0);

			//关闭对应Accept的socket  
			closesocket(newSocket);
		}



	} while (1);

	//关闭自身的Socket  
	closesocket(m_sock);
}



int CClientNet::Connect(int port, const char* address)
{
	int rlt = 0;

	//用于记录错误信息并输出
	int iErrMsg;
	//启动WinSock
	WSAData wsaData;
	iErrMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (iErrMsg != NO_ERROR)
		//有错误
	{
		rlt = 1;
		return rlt;
	}

	//创建Socket
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (m_sock == INVALID_SOCKET)
		//创建Socket失败
	{
		pri(L"创建Socket失败");
		rlt = 2;
		return rlt;
	}

	//目标服务器数据
	sockaddr_in sockaddrServer;
	sockaddrServer.sin_family = AF_INET;
	sockaddrServer.sin_port = port;
	sockaddrServer.sin_addr.s_addr = inet_addr(address);

	//连接,sock与目标服务器连接
	iErrMsg = connect(m_sock, (sockaddr*)&sockaddrServer, sizeof(sockaddrServer));
	if (iErrMsg < 0)
	{
		pri(L"连接Socket失败");
		rlt = 3;
		return rlt;
	}

	return rlt;
}




int CClientNet::SendMsg(const char* msg, int len)
{
	int rlt = 0;

	int iErrMsg = 0;

	//发送消息，指定sock发送消息
	iErrMsg = send(m_sock, msg, len, 0);
	if (iErrMsg < 0)
		//发送失败
	{
		pri(L"发送Socket失败");
		rlt = 1;
		return rlt;
	}

	return rlt;
}

void CClientNet::Close()
{
	closesocket(m_sock);
}


//int main(int argc, char **argv)
//{
//	CServerNet serverNet;
//	int iRlt = serverNet.Init("127.0.0.1", 6688);
//	if (iRlt == 0)
//	{
//		printf("init ok...\n");
//		serverNet.Run();
//	}
//	else
//		printf("serverNet init failed with error : %d\n", iRlt);
//
//	system("pause");
//}
