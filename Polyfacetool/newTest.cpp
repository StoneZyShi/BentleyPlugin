#include "polyfacetool.h"
#include "..\include\dgnplatform\MeshHeaderHandler.h"
#include "utility.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;

#include <stdio.h>
#include <winsock2.h>
#include <Windows.h>
#include <Ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")
#define  PORT 6688

int test()
{
	//��ʼ�����绷��
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		pri(L"WSAStartup failed");
		return -1;
	}
	// ��ʼ����ɣ�����һ��TCP��socket
	SOCKET sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)
	{
		pri(L"socket failed");
		return -1;
	}
	//ָ�����ӵķ������Ϣ
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(PORT);
	//�ͻ���ֻ��Ҫ����ָ���ķ�������ַ��127.0.0.1�Ǳ����Ļػ���ַ
	addrServ.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// ������Bind �ͻ����ǽ�������
	int ret = connect(sServer, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));//��ʼ����
	if (SOCKET_ERROR == ret)
	{
		pri(L"socket connect failed");
		WSACleanup();
		closesocket(sServer);
		return -1;
	}
	//���ӳɹ��󣬾Ϳ��Խ���ͨ����
	char szBuf[1024];
	memset(szBuf, 0, sizeof(szBuf));
	sprintf_s(szBuf, sizeof(szBuf), "Hello server");
	//���������recv��ʱ�򣬿ͻ��˾���Ҫsend��������ͬʱ�����շ���Ῠ�������Ϊrecv��send��������
	ret = send(sServer, szBuf, (int)strlen(szBuf), 0);
	if (SOCKET_ERROR == ret)
	{
		pri(L"socket send failed");
		closesocket(sServer);
		return -1;
	}

	ret = recv(sServer, szBuf, sizeof(szBuf), 0);
	if (SOCKET_ERROR == ret)
	{
		pri(L"socket recv failed");
		closesocket(sServer);
		return -1;
	}
	pri(s2wsT(std::string(szBuf)).GetWCharCP());
	closesocket(sServer);
	WSACleanup();
	return 0;
}


Public void newTest(WCharCP unparsed)
{
	pri(L"new������");
	pri(unparsed);

	test();
}