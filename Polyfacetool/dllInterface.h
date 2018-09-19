#pragma once
#include <Windows.h>
#include "..\mylog\mylog.h"
using pExportmylogInterface = mylogInterface* (*)(void);

class dllInterfacemylogInterface
{
public:
	dllInterfacemylogInterface();
	~dllInterfacemylogInterface();

	void Init(const std::string& logPath);
	void Close();

	mylogInterface *c;
private:
	HINSTANCE hDll;

};


#include "..\myjson\myjson.h"
using pExportmyjsonInterface = myjsonInterface* (*)(void);

class dllInterfacemyjsonInterface
{
public:
	dllInterfacemyjsonInterface();
	~dllInterfacemyjsonInterface();

	void Init();
	void Close();

	myjsonInterface *c;
private:
	HINSTANCE hDll;

};

#include "..\CS\client\client.h"

void callback(char *str);

class treeClient
{
public:

	void sendMeg(const char* meg, const char* ip, const char* port);
	std::string getRead();
};



