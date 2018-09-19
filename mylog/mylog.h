#pragma once
#include <iostream>
#include <string>
#ifdef MYLOG_API
#define MYLOG_API __declspec(dllexport)
#else
#define MYLOG_API __declspec(dllimport)
#endif

class MYLOG_API mylogInterface
{
public:
	virtual void init(const std::string& logPath) = 0;
	virtual void log_info(const std::string& info) = 0;
	virtual void log_warning(const std::string& warning) = 0;
	virtual void log_error(const std::string& error) = 0;
	virtual void close() = 0;
};

extern "C" MYLOG_API mylogInterface* getInstanceLog();

#ifdef MYLOG_API
class mylog:public mylogInterface
{
public:
	void init(const std::string& logPath);
	void log_info(const std::string& info);
	void log_warning(const std::string& warning);
	void log_error(const std::string& error);
	void close();
};

#endif
