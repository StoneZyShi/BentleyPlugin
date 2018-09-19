#include "BScanfRefers.h"


extern "C" void mdlSystem_setBatchProcessingState(BatchProcessing_States, const WChar*);

BScanfRefers::BScanfRefers()
{
}


BScanfRefers::~BScanfRefers()
{
}


std::string BScanfRefers::shifting(const DgnModelRefP & modelRef, double myTransform[4][4])
{
	WString ws = L"";
	try
	{
		Transform form;
		bool scaleZfor2dRef = true;
		modelRef->AsDgnAttachmentP()->GetTransformToParent(form, scaleZfor2dRef);
		DPoint3d translation;
		form.GetTranslation(translation);

		DPoint3d       fixedPoint;
		DVec3d       directionVector;
		double          radians;//旋转
		double          scale;//缩放
		form.IsUniformScaleAndRotateAroundLine(fixedPoint, directionVector, radians, scale);

		double newTransform[4][4] = { 1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
		double matrix_result[4][4] = { 1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
		double uorPerMeter = mdlModelRef_getUorPerMeter(ACTIVEMODEL);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				newTransform[i][j] = form.form3d[i][j];
			}
			newTransform[i][3] = form.form3d[i][3] / uorPerMeter;
			newTransform[3][i] = 0;
		}
		newTransform[3][3] = 1;
		
		for (int m = 0; m < 4; m++) {
			for (int s = 0; s < 4; s++) {
				matrix_result[m][s] = 0;//变量使用前记得初始化,否则结果具有不确定性
				for (int n = 0; n < 4; n++) {
					matrix_result[m][s] += myTransform[m][n] * newTransform[n][s];
				}
			}
		}
		
		ws.Sprintf(L"(%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,)matrix_result",
			matrix_result[0][0], matrix_result[0][1], matrix_result[0][2], matrix_result[0][3],
			matrix_result[1][0], matrix_result[1][1], matrix_result[1][2], matrix_result[1][3],
			matrix_result[2][0], matrix_result[2][1], matrix_result[2][2], matrix_result[2][3],
			matrix_result[3][0], matrix_result[3][1], matrix_result[3][2], matrix_result[3][3]);

	}
	catch (std::exception& e)
	{
		pris(e.what());
	}
	return ws2sT(ws);
}

void BScanfRefers::scanf(std::string & md)
{
	fmsg;
	try
	{
		std::string thisFileName;
		std::string thisModelName;

		DgnModelP pActiveModel = ISessionMgr::GetActiveDgnModelP();
		DgnModel::ElementsCollection elemColl = pActiveModel->GetElementsCollection();
		for (PersistentElementRefP const& elemRef : elemColl) 
		{
			ModelRefIteratorP   iterator;
			DgnModelRefP       modelRef;
			mdlModelRefIterator_create(&iterator, elemRef->GetDgnModelP(), MRITERATE_PrimaryChildRefs, 0);

			while (nullptr != (modelRef = mdlModelRefIterator_getNext(iterator)))
			{
				if (nullptr != modelRef->GetDgnFileP())
				{
					thisFileName = ws2sT(modelRef->GetDgnFileP()->GetFileName());
					if (thisFileName.substr(thisFileName.size() - 3, 3) == "dgn")
					{
						thisModelName = ws2sT(modelRef->GetDgnModelP()->GetModelName());
						if (modelRef->AsDgnAttachmentP()->IsDisplayed())
						{
							std::string logicalName = ws2sT(WString(modelRef->AsDgnAttachmentP()->GetLogicalName()));
							//std::string refer = shifting(modelRef); 
							double myTransform[4][4] = { 1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1 };
							std::string refer = shifting(modelRef, myTransform);
							
							treeClient t;
							t.sendMeg("GetPath:", ip.c_str(), port.c_str());
							std::string rp = t.getRead();

							std::string path = ws2sT(getRootPath(ROOTFILE::rootFilePath));
							path = path.substr(rp.length(), path.length() - rp.length() - 4);
							std::string model = ws2sT(getRootPath(ROOTFILE::rootModel));

							replace_all(thisFileName, "\\", "/");
							thisFileName = thisFileName.substr(rp.length(), thisFileName.length() - rp.length() - 4);

							pris("    扫描到:"+path+" " +model+" " +thisFileName+" "+thisModelName);
							//thisFileName = thisFileName.substr(0,thisFileName.rfind(".dgn"));

							sendToServer(path, model, thisFileName, thisModelName, logicalName, refer,md);
						}
					}
				}
			}
			
			mdlModelRefIterator_free(&iterator);

			break;
		}
		
	}
	catch (std::exception& e)
	{
		pris(e.what());
	}
}

void BScanfRefers::sendToServer(std::string & PFile, std::string & PModel, std::string & File, std::string & Model, std::string & logical, std::string & refer, std::string & md)
{
	fmsg;
	replace_all(PFile, "\\", "/");
	replace_all(File, "\\", "/");

	dllInterfacemyjsonInterface inter;
	inter.Init();
	inter.c->clear();

	std::string sNull = "";
	inter.c->Add("pf", PFile);
	inter.c->Add("pm", PModel); 
	inter.c->Add("f", File); 
	inter.c->Add("m", Model); 
	inter.c->Add("refer", refer);
	inter.c->Add("logical", logical); 
	inter.c->Add("id", sNull); 
	inter.c->Add("fmd", md);

	std::string str = inter.c->ToString();

	treeClient t;
	pris("发送："+str);
	t.sendMeg(("NewTree:" + str).c_str(), ip.c_str(), port.c_str());
	pris("  接收："+t.getRead());
}

void BScanfRefers::newOne(std::string & File, std::string & Model, std::string & md)
{
	fmsg;
	treeClient t;
	t.sendMeg("GetPath:", ip.c_str(), port.c_str());
	std::string rp = t.getRead();

	File = rp + File + ".dgn";
	mdlSystem_setBatchProcessingState(PROCESSSTATE_Processing, NULL);
	if (SUCCESS == mdlSystem_newDesignFileAndModel(s2wsT(File).GetWCharCP(), s2wsT(Model).GetWCharCP()))
	{
		pris("--跳转" + File + " " + Model);
		BScanfRefers r;
		r.scanf(md);
	}
}
