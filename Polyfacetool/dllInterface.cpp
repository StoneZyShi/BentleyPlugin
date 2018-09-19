#include "dllInterface.h"



dllInterfacemylogInterface::dllInterfacemylogInterface()
{
}


dllInterfacemylogInterface::~dllInterfacemylogInterface()
{
	Close();
}

void dllInterfacemylogInterface::Init(const std::string& logPath)
{
	hDll = LoadLibrary("dllMyLog.dll");
	pExportmylogInterface Get = (pExportmylogInterface)GetProcAddress(hDll, "getInstanceLog");

	c = Get();
	c->init(logPath);
}

void dllInterfacemylogInterface::Close()
{
	if (c != nullptr)
		c->close();

	if(c != nullptr)
		delete c;
	if (hDll != nullptr)
		FreeLibrary(hDll);
}





dllInterfacemyjsonInterface::dllInterfacemyjsonInterface()
{
}

dllInterfacemyjsonInterface::~dllInterfacemyjsonInterface()
{
	Close();
}


void dllInterfacemyjsonInterface::Init()
{
	hDll = LoadLibrary("dlljson.dll");
	pExportmyjsonInterface Get = (pExportmyjsonInterface)GetProcAddress(hDll, "getInstanceJson");

	c = Get();
}

void dllInterfacemyjsonInterface::Close()
{
	if (c != nullptr)
		delete c;
	if (hDll != nullptr)
		FreeLibrary(hDll);
}



bool flag = false;
std::string strB;

void callback(char *str)
{
	strB = str;
	flag = true;
}

void treeClient::sendMeg(const char* meg, const char* ip, const char* port)
{
	HINSTANCE hDll = LoadLibrary("client.dll");
	apiMessageS mes = (apiMessageS)GetProcAddress(hDll, "messageS");

	if (mes)
	{
		mes(meg,callback,ip,port);
	}
}


std::string treeClient::getRead()
{
	while (!flag) { Sleep(50); }

	flag = false;
	return strB;
}
