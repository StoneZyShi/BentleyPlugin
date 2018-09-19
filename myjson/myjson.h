#pragma once
#include <iostream>
#include <string>
#ifndef MYJSON_API
#define MYJSON_API __declspec(dllexport)
#else
#define MYJSON_API __declspec(dllimport)
#endif

class MYJSON_API myjsonInterface
{
public:
	virtual bool Add(const std::string& strKey, const std::string& strValue) = 0;
	virtual bool Replace(const std::string& strKey, const std::string& strValue) = 0;
	virtual std::string ToString() const = 0;


	virtual bool InitJson(const std::string& str) = 0;
	virtual std::string Get(const std::string& strKey, std::string& strValue) = 0;
	virtual bool GetB(const std::string& strKey, std::string& strValue) = 0;
	virtual void clear() = 0;
};

extern "C" MYJSON_API myjsonInterface* getInstanceJson();

#ifdef MYJSON_API
class myjson :public myjsonInterface
{
public:
	bool Add(const std::string& strKey, const std::string& strValue);
	bool Replace(const std::string& strKey, const std::string& strValue);
	std::string ToString() const;

	bool InitJson(const std::string& str);
	std::string Get(const std::string& strKey, std::string& strValue);
	bool GetB(const std::string& strKey, std::string& strValue);
	void clear();
};

#endif


