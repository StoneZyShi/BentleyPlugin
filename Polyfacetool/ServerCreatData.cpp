#include "polyfacetool.h"
#include "..\include\dgnplatform\MeshHeaderHandler.h"
#include "utility.h"
#include "dllInterface.h"
#include "BScanfRefers.h"
#include "BGetOneDbData.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;


Public void ServerCreatData(WCharCP unparsed)
{
	fmsg;
	
	BScanfRefers refer;
	std::string path = ws2sT(getRootPath(ROOTFILE::rootFile));
	std::string model = ws2sT(getRootPath(ROOTFILE::rootModel));
	std::string root = "root";
	std::string k = "";
	std::string rootPath = "NewNode:" + ws2sT(getRootPath(ROOTFILE::rootPath)) + "/";
	treeClient t;
	pris("发送：" + rootPath);
	t.sendMeg(rootPath.c_str(), refer.ip.c_str(), refer.port.c_str());
	pris("  接收：" + t.getRead());
	
	double pre = mdlModelRef_getUorPerMeter(ACTIVEMODEL);
	std::string precision = ws2sT((WString)unparsed).c_str();
	if (!isNum(precision))
	{
		precision = std::to_string(pre);
	}
	precision = "SetPrec:" + precision;
	pris("发送：" + precision);
	t.sendMeg(precision.c_str(), refer.ip.c_str(), refer.port.c_str());
	pris("  接收：" + t.getRead());
	
	DgnModelP pActiveModel = ISessionMgr::GetActiveDgnModelP();
	ScanRange Srange;
	pActiveModel->GetRange(Srange);
	double x = (Srange.xhighlim + Srange.xlowlim) / 2 / pre;
	double y = (Srange.yhighlim + Srange.ylowlim) / 2 / pre;
	double z = (Srange.zhighlim + Srange.zlowlim) / 2 / pre;
	//priOther(Srange.xhighlim);
	//priOther(Srange.yhighlim);
	//priOther(Srange.zhighlim);
	//priOther(Srange.xlowlim);
	//priOther(Srange.ylowlim);
	//priOther(Srange.zlowlim);
	std::string centre = "SetCent:" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
	pris("发送：" + centre);
	t.sendMeg(centre.c_str(), refer.ip.c_str(), refer.port.c_str());
	pris("  接收：" + t.getRead());


	//发送root model
	refer.sendToServer(root, root, path, model, k, k,k);
	
	std::string str = "";
	do {
		pris("发送ScaTree:");
		t.sendMeg("ScaTree:", refer.ip.c_str(), refer.port.c_str());
		str = t.getRead();
		pris("  接收ScaTree:"+str);

		if (str != "无")
		{
			dllInterfacemyjsonInterface inter;
			inter.Init();
			inter.c->InitJson(str);

			BScanfRefers r;
			std::string f = inter.c->Get("f", k);
			std::string m = inter.c->Get("m", k);
			std::string md = inter.c->Get("md", k);
			r.newOne(f, m,md);
		}
		if (str == "远程中断")
		{
			Sleep(1000);
		}

	} while (str != "无");

	pris("发送ScaOver:");
	t.sendMeg("ScaOver:", refer.ip.c_str(), refer.port.c_str());
	str = t.getRead();
	pris("  接收ScaOver:" + str);
	
	BGetOneDbData d(refer.ip, refer.port);
	d.getOneModel();

	system("TASKKILL /F /IM BentleyPluginServer.exe");
}

