#include "myjson.h"
#include "CJsonObject.h"

using namespace neb;

CJsonObject j;

MYJSON_API myjsonInterface * getInstanceJson()
{
	myjsonInterface* pInstance = new myjson();
	return pInstance;
}

bool myjson::Add(const std::string & strKey, const std::string & strValue)
{
	return j.Add(strKey,strValue);
}

bool myjson::Replace(const std::string & strKey, const std::string & strValue)
{
	return j.Replace(strKey, strValue);
}

std::string myjson::ToString() const
{
	return j.ToString();
}

bool myjson::InitJson(const std::string & str)
{	
	j = CJsonObject(str);
	return true;
}

std::string myjson::Get(const std::string & strKey, std::string & strValue)
{
	j.Get(strKey, strValue);
	return strValue;
}

bool myjson::GetB(const std::string & strKey, std::string & strValue)
{
	return j.Get(strKey, strValue);
}

void myjson::clear()
{
	j.Clear();
}
