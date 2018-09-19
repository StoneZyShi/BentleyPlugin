#pragma once
#ifndef _MY_UTILITY_H_
#define _MY_UTILITY_H_
#include "polyfacetool.h"
#include "..\include\dgnplatform\MeshHeaderHandler.h"
#include "..\include\DgnPlatform\ElementGraphics.h"
//#include "..\include\DgnPlatform\ColorBook.h"
//#include "..\include\DgnPlatform\ColorUtil.h"
#include "..\include\DgnView\LocateSubEntityTool.h"
#include <Mstn\ISessionMgr.h> 
#include <Mstn\MdlApi\MdlApi.h>
#include <DgnPlatform\DgnPlatformApi.h>
#include <DgnPlatform\Handler.h>
#include <PSolid\PSolidCoreAPI.h>
#include <DgnPlatform\SectionClip.h>

#include <Windows.h>
#include <iostream>                                       
#include <fstream>
#include <string>
#include "sqlliteRW.h"

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include  <direct.h>  
#include <math.h>
#include <tchar.h>

#include <thread>
#include "dllInterface.h"
#include "mycpp.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;

#include    <Mstn\MdlApi\MdlApi.h>
#include    <Mstn\MdlApi\mselems.h>
#include    <DgnPlatform\DgnECManager.h>
using namespace Bentley::ECN;

#define pri(str) mdlDialog_dmsgsPrint(str);
#define pris(str) mdlDialog_dmsgsPrint(s2wsT(str).GetWCharCP());
#define priw(wstr) mdlDialog_dmsgsPrint(wstr.GetWCharCP());
#define priOther(val) mdlDialog_dmsgsPrint(s2wsT(std::to_string(val)).GetWCharCP());

#define MY_DB
#define MY_OBJ

#define MY_ZIP
#define MY_INITIAL_ZIP

#define MY_LOG_PROGESS

#include <exception>

//#define STEP
//#define ECPROPERTY


#define COLOR_TEST
#define MTL_FILE


std::string getTime();

std::string deleteLine(std::string FileName);

std::string WChar2Ansi(LPCWSTR pwszSrc);

std::string ws2s(const std::wstring& ws);
std::string ws2sT(const WString& ws);
std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);

std::wstring s2ws(const std::string& s);

WString s2wsT(const std::string& s);

enum ROOTFILE {
	rootModel = 0,
	rootFile,
	rootPath,
	rootFilePath,
	rootFileAndModelPath,
};
WString getRootPath(ROOTFILE rf);

void deleteAllMark(WString& wstr, const std::string & mark);

enum CHECKDB
{
	del = 0,
	check,
};


void replace_all(std::string & str, char * oldValue, char * newValue);

void fileAndModel(const std::string& fileModel,std::string& file,std::string& model);



WCHAR* mbcsToUnicode(const char *zFilename);
char* unicodeToUtf8(const WCHAR *zWideFilename);


std::string toUtf8(const std::string& str);
char* UnicodeToUtf8(const wchar_t* unicode);

std::string UnicodeToUtf8N(const wchar_t* unicode);

bool RemoveDir(const char* szFileDir);


void msg(const TCHAR *s);

void info(const std::string& path, const std::string& infoMsg);
void infoModel(const std::string& path, const std::string& infoMsg);

bool isNum(const std::string& str);


#endif
