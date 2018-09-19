#pragma once

#ifndef CLIENTDLL_EXPORTS
#define CLIENTDLL_EXPORTS __declspec(dllexport)
#else
#define CLIENTDLL_EXPORTS __declspec(dllimport)
#endif

#define MaxLen 10240

extern "C"
{
	typedef void(*Fun)(char *);
	CLIENTDLL_EXPORTS void messageS(const char* send,Fun pFun, const char* ip, const char* port);
	typedef void(*apiMessageS)(const char*, Fun,const char* ip,const char* port);
}



