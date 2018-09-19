#include "mysocket.h"
#include "utility.h"



int CServerNet::Init(const char* address, int port)
{
	int rlt = 0;

	//���ڼ�¼������Ϣ�������  
	int iErrorMsg;

	//��ʼ��WinSock  
	WSAData wsaData;
	iErrorMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);

	if (iErrorMsg != NO_ERROR)
	{
		//��ʼ��WinSockʧ��  
		//pri(L"wsastartup failed with error : %d\n", iErrorMsg);

		rlt = 1;
		return rlt;
	}

	//���������Socket  
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)

	{
		//����Socket�쳣  
		//pri(L"socket failed with error : %d\n", WSAGetLastError());

		rlt = 2;
		return rlt;
	}

	//������Ϣ  
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = inet_addr(address);

	//��  
	iErrorMsg = bind(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (iErrorMsg < 0)
	{
		//��ʧ��  
		//printf(L"bind failed with error : %d\n", iErrorMsg);
		rlt = 3;
		return rlt;
	}



	return rlt;
}

void CServerNet::Run()
{
	//��������  
	listen(m_sock, 5);

	sockaddr_in tcpAddr;
	int len = sizeof(sockaddr);
	SOCKET newSocket;
	char buf[1024];
	int rval;

	do
	{
		//������Ϣ  
		newSocket = accept(m_sock, (sockaddr*)&tcpAddr, &len);


		if (newSocket == INVALID_SOCKET)
		{
			//�ǿ���socket  

		}
		else
		{
			//��socket����  
			//pri(L"new socket connect : %d\n", newSocket);


			//��Ϣ����  
			do
			{
				//pri(L"process\n");
				//��������  
				memset(buf, 0, sizeof(buf));
				rval = recv(newSocket, buf, 1024, 0);


				if (rval == SOCKET_ERROR)
					//��Ӧ���Ǹ��쳣�����ͻ���û�е���closeSocket��ֱ���˳���Ϸ��ʱ�򣬽����������  
					printf("recv socket error\n");



				if (rval == 0)
					//recv����0��ʾ�����˳�  
					printf("ending connection");
				//else
					//��ʾ���յ�������  
					//pri(L"recv %s\n", buf);


			} while (rval != 0);

			//�رն�ӦAccept��socket  
			closesocket(newSocket);
		}



	} while (1);

	//�ر������Socket  
	closesocket(m_sock);
}



int CClientNet::Connect(int port, const char* address)
{
	int rlt = 0;

	//���ڼ�¼������Ϣ�����
	int iErrMsg;
	//����WinSock
	WSAData wsaData;
	iErrMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (iErrMsg != NO_ERROR)
		//�д���
	{
		rlt = 1;
		return rlt;
	}

	//����Socket
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (m_sock == INVALID_SOCKET)
		//����Socketʧ��
	{
		pri(L"����Socketʧ��");
		rlt = 2;
		return rlt;
	}

	//Ŀ�����������
	sockaddr_in sockaddrServer;
	sockaddrServer.sin_family = AF_INET;
	sockaddrServer.sin_port = port;
	sockaddrServer.sin_addr.s_addr = inet_addr(address);

	//����,sock��Ŀ�����������
	iErrMsg = connect(m_sock, (sockaddr*)&sockaddrServer, sizeof(sockaddrServer));
	if (iErrMsg < 0)
	{
		pri(L"����Socketʧ��");
		rlt = 3;
		return rlt;
	}

	return rlt;
}




int CClientNet::SendMsg(const char* msg, int len)
{
	int rlt = 0;

	int iErrMsg = 0;

	//������Ϣ��ָ��sock������Ϣ
	iErrMsg = send(m_sock, msg, len, 0);
	if (iErrMsg < 0)
		//����ʧ��
	{
		pri(L"����Socketʧ��");
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
