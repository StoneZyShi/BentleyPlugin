#include "polyfacetool.h"
#include "..\include\dgnplatform\MeshHeaderHandler.h"
#include "utility.h"
#include "dllInterface.h"
#include "BGetOneDbData.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;


Public void ClientCreatData(WCharCP unparsed)
{
	fmsg;
	//pri(L"CMDNAME_ClientCreatData£¡£¡£¡");
	//pri(unparsed);
	std::string str = ws2sT(WString(unparsed));

	std::string ip = str.substr(0, str.find(" "));
	std::string port = str.substr(str.rfind(" ") + 1, str.length() - str.rfind(" ") - 1);

	pris(ip + "  " + port);
	
	BGetOneDbData d(ip,port);
	d.getOneModel();

}