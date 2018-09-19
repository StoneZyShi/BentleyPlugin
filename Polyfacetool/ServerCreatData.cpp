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
	pris("���ͣ�" + rootPath);
	t.sendMeg(rootPath.c_str(), refer.ip.c_str(), refer.port.c_str());
	pris("  ���գ�" + t.getRead());
	
	double pre = mdlModelRef_getUorPerMeter(ACTIVEMODEL);
	std::string precision = ws2sT((WString)unparsed).c_str();
	if (!isNum(precision))
	{
		precision = std::to_string(pre);
	}
	precision = "SetPrec:" + precision;
	pris("���ͣ�" + precision);
	t.sendMeg(precision.c_str(), refer.ip.c_str(), refer.port.c_str());
	pris("  ���գ�" + t.getRead());
	
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
	pris("���ͣ�" + centre);
	t.sendMeg(centre.c_str(), refer.ip.c_str(), refer.port.c_str());
	pris("  ���գ�" + t.getRead());


	//����root model
	refer.sendToServer(root, root, path, model, k, k,k);
	
	std::string str = "";
	do {
		pris("����ScaTree:");
		t.sendMeg("ScaTree:", refer.ip.c_str(), refer.port.c_str());
		str = t.getRead();
		pris("  ����ScaTree:"+str);

		if (str != "��")
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
		if (str == "Զ���ж�")
		{
			Sleep(1000);
		}

	} while (str != "��");

	pris("����ScaOver:");
	t.sendMeg("ScaOver:", refer.ip.c_str(), refer.port.c_str());
	str = t.getRead();
	pris("  ����ScaOver:" + str);
	
	BGetOneDbData d(refer.ip, refer.port);
	d.getOneModel();

	system("TASKKILL /F /IM BentleyPluginServer.exe");
}

