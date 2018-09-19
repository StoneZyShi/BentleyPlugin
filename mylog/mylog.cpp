#define MYLOG_API
#include "mylog.h"
#define GLOG_NO_ABBREVIATED_SEVERITIES 
#include "glog\logging.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdio.h>

using namespace std;

extern "C" MYLOG_API mylogInterface* getInstanceLog()
{
	mylogInterface* pInstance = new mylog();
	return pInstance;
}
string GetExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
	string path = szFilePath;

	return path;
}
void mylog::init(const std::string& logPath)
{
	//std::ofstream outTem("e:/helenxr_glog_log_dir/a.txt", std::ios::app);
	//if (outTem.is_open())
	//{
	//	outTem << GetExePath().c_str();
	//}
	//outTem.close();

	FLAGS_log_dir = logPath.c_str();
	google::InitGoogleLogging("bentley");
}

void mylog::log_info(const std::string& info)
{
	LOG(INFO) << info;
}

void mylog::log_warning(const std::string& warning)
{
	LOG(WARNING) << warning;
}

void mylog::log_error(const std::string& error)
{
	LOG(ERROR) << error;
}

void mylog::close()
{
	google::ShutdownGoogleLogging();
}


