#include "utility.h"
#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
#include <string.h>

using namespace std;

std::string getTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d--%H-%M-%S", localtime(&timep));
	return tmp;
}

std::string deleteLine(std::string FileName)
{
	vector<string> vecContent;
	string strLine, str;
	ifstream inFile(FileName);

	getline(inFile, str);
	vecContent.push_back(str);
	while (inFile)
	{
		getline(inFile, strLine);
		vecContent.push_back(strLine);
	}
	inFile.close();
	
	vecContent.erase(vecContent.begin());
	ofstream outFile(FileName);
	vector<string>::const_iterator iter = vecContent.begin();
	for (; vecContent.end() != iter; ++iter)
	{
		outFile.write((*iter).c_str(), (*iter).size()) << endl;
	}

	outFile.close();

	return str;
}


std::string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen <= 0) return std::string("");
	char* pszDst = new char[nLen]();
	if (NULL == pszDst) return std::string("");
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;
	std::string strTemp(pszDst);
	delete[] pszDst;
	return strTemp;
}

std::string ws2s(const std::wstring& ws)
{
	return WChar2Ansi(ws.c_str());
}

std::string ws2sT(const WString& ws)
{
	return WChar2Ansi(ws.c_str());
}
std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if (nSize <= 0) return NULL;

	WCHAR *pwszDst = new WCHAR[nSize + 1]();
	if (NULL == pwszDst) return NULL;

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;

	if (pwszDst[0] == 0xFEFF) // skip Oxfeff  
		for (int i = 0; i < nSize; i++)
			pwszDst[i] = pwszDst[i + 1];

	std::wstring wcharString(pwszDst);
	delete pwszDst;


	return wcharString;
}

std::wstring s2ws(const std::string& s)
{
	return Ansi2WChar(s.c_str(), (int)(s.size()));
}

WString s2wsT(const std::string & s)
{
	return WString(s.c_str());
}


WString getRootPath(ROOTFILE rf)
{
	WString wstr = L"";
	WString filePath = ISessionMgr::GetActiveDgnFile()->GetFileName();
	WString model = ISessionMgr::GetActiveDgnModelP()->GetDgnModelP()->GetModelName();
	std::string sFilePath = ws2sT(filePath);
	replace_all(sFilePath, "\\", "/");
	filePath = s2wsT(sFilePath);

	switch (rf)
	{
	case ROOTFILE::rootFilePath:
		wstr = filePath;
		break;
	case ROOTFILE::rootPath:
		wstr = filePath.substr(0, filePath.rfind(L"/"));
		break;
	case ROOTFILE::rootModel:
		wstr = model;
		break;
	case ROOTFILE::rootFile:
		wstr = filePath.substr(filePath.rfind(L"/")+1,filePath.rfind(L".")- filePath.rfind(L"/") -1);
		break;
	case ROOTFILE::rootFileAndModelPath:
		wstr = filePath + L"(" + model + L")";
		break;
	default:
		break;
	}
	return wstr;
}

void deleteAllMark(WString& wstr, const std::string & mark)
{
	std::string s = ws2s(wstr.GetWCharCP());
	int nSize = (int)(mark.size());
	while (1)
	{
		std::string::size_type pos = (int)(s.find(mark));
		if (pos == string::npos)
		{
			return;
		}

		s.erase(pos, nSize);
	}
	wstr = s2wsT(s);
}


void replace_all(std::string & str, char * oldValue, char * newValue)
{
	std::string::size_type pos(0);

	while (true) 
	{
		pos = str.find(oldValue, pos);
		if (pos != (std::string::npos))
		{
			str.replace(pos, strlen(oldValue), newValue);
			pos += 2;//注意是加2，为了跳到下一个反斜杠
		}
		else
			break;
	}
}

void fileAndModel(const std::string & fileModel, std::string & file, std::string& model)
{
	if (fileModel.find("<->")!=std::string::npos)
	{
		int len = (int)(fileModel.find("<->"));
		file = fileModel.substr(0, len);
		model = fileModel.substr(len + 3, fileModel.length() - len - 3);
	}
}


WCHAR* mbcsToUnicode(const char *zFilename)
{
	int nByte;
	WCHAR *zMbcsFilename;
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, NULL, 0) * sizeof(WCHAR);
	zMbcsFilename = (WCHAR *)malloc(nByte * sizeof(zMbcsFilename[0]));
	if (zMbcsFilename == 0)
	{
		return 0;
	}
	nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, zMbcsFilename, nByte);
	if (nByte == 0)
	{
		free(zMbcsFilename);
		zMbcsFilename = 0;
	}
	return zMbcsFilename;
}

//有Unicode转为UTF-8
char* unicodeToUtf8(const WCHAR *zWideFilename)
{
	int nByte; char *zFilename;
	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, 0, 0, 0, 0);
	zFilename = (char *)malloc(nByte);
	if (zFilename == 0)
	{
		return 0;
	}
	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, zFilename, nByte, 0, 0);
	if (nByte == 0)
	{
		free(zFilename);
		zFilename = 0;
	}
	return zFilename;
}

std::string toUtf8(const std::string& str)
{
	char * pPath;
	WCHAR *wcPath;
	wcPath = mbcsToUnicode(("12"+str).c_str());//strFulPath即为带有中文的全路径字符串
	pPath = unicodeToUtf8(wcPath);

	std::string szDBFile = pPath;

	return szDBFile.substr(2, szDBFile.length() - 2);
}

char* UnicodeToUtf8(const wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

std::string UnicodeToUtf8N(const wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	std::string rtn = szUtf8;
	free(szUtf8);
	return rtn;
}

bool RemoveDir(const char* szFileDir)
{
	std::string strDir = szFileDir;
	if (strDir.at(strDir.length() - 1) != '\\')
		strDir += '\\';
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile((strDir + "*.*").c_str(), &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	do
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (stricmp(wfd.cFileName, ".") != 0 &&
				stricmp(wfd.cFileName, "..") != 0)
				RemoveDir((strDir + wfd.cFileName).c_str());
		}
		else
		{
			if (!DeleteFile((strDir + wfd.cFileName).c_str()))
			{
				FindClose(hFind);
				RemoveDirectory(szFileDir);
				return false;
			}
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
	RemoveDirectory(szFileDir);
	return true;
}


void msg(const TCHAR *s)
{
#ifdef UNDER_CE
	int res = IDOK;
	if (s[0] == '*') res = MessageBox(0, s, _T("Zip error"), MB_ICONERROR | MB_OKCANCEL);
	else if (s[0] == '.') MessageBeep(0);
	else MessageBox(0, s, _T("Zip test"), MB_OKCANCEL);
	if (res == IDCANCEL) p_abort = true;
#else
	_tprintf(_T("%s\n"), s);
#endif
}

void info(const std::string& path,const std::string& infoMsg)
{
	dllInterfacemylogInterface myDllAll;
	myDllAll.Init(path + "/db/log");
	myDllAll.c->log_info(infoMsg);

	//myDllAll.logWarning("google log first warning level message!");
	//myDllAll.logError("google log first error level message!");
}

void infoModel(const std::string & path, const std::string & infoMsg)
{
	info(path, ws2sT(getRootPath(ROOTFILE::rootFileAndModelPath)) + infoMsg);
}

bool isNum(const std::string& str)
{
	bool rtn = true;

	int len = (int)str.length();
	for (int i = 0; i < len; i++)
	{
		if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.' || str[i] == ' ')
		{
			rtn = true;
		}
		else
		{
			rtn = false;
			break;
		}
	}
	if (str[0] == '.')
		rtn = false;

	return rtn;
}






