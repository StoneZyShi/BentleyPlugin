#pragma once
#include "findAllReference.h"
#include "fileControl.h"
#include "getData.h"
#include "utility.h"


Public void startPolyfaceConvertTool(WCharCP unparsed)
{
	pri(L"开始提取！！！");
	
	//pri(L"startPolyfaceConvertTool！！！");
	fileControl file(ws2sT(getRootPath(ROOTFILE::rootFilePath)), ws2sT(getRootPath(ROOTFILE::rootModel)));
	file.start(unparsed);
	
	//if ((_access((ws2sT(getRootPath(ROOTFILE::rootPath)) + "/precision.config").c_str(), 0)) != -1)
	//	RemoveDir((ws2sT(getRootPath(ROOTFILE::rootPath)) + "/precision.config").c_str());
	pri(L"提取完成！！！");
}



