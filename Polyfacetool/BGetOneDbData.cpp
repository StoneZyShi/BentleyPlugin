#include "BGetOneDbData.h"
#include "utility.h"
#include <thread>
#include "mycpp.h"

#include "md5.h"

extern "C" void mdlSystem_setBatchProcessingState(BatchProcessing_States, const WChar*);

BGetOneDbData::BGetOneDbData(const std::string & ip, const std::string & port):ip(ip),port(port)
{
}

BGetOneDbData::~BGetOneDbData()
{
}

void BGetOneDbData::getOneModel()
{
	fmsg;
	treeClient t;
	if (port != "127.0.0.1")
	{
		std::string rootPath = ws2sT(getRootPath(ROOTFILE::rootPath)) + "/";
		pris("发送：" + rootPath);
		t.sendMeg(("NewNode:" + rootPath).c_str(), ip.c_str(), port.c_str());
		pris("  接收：" + t.getRead());
	}

	t.sendMeg("GetPath:", ip.c_str(), port.c_str());
	path = t.getRead();
	pris(path);
	creatFiles();

	std::string node = "";
	while (node != "无")
	{
		t.sendMeg("GetTree:", ip.c_str(), port.c_str());
		node = t.getRead();
		pris(node);

		if (node.find("refer") != std::string::npos && node.find("logical") != std::string::npos)
		{
			dllInterfacemyjsonInterface inter;
			inter.Init();
			inter.c->clear();
			inter.c->InitJson(node);
			std::string k = "";
			std::string pf = inter.c->Get("pf", k);
			std::string pm = inter.c->Get("pm", k);
			std::string f = inter.c->Get("f", k);
			std::string m = inter.c->Get("m", k);
			std::string refer = inter.c->Get("refer", k);
			std::string logical = inter.c->Get("logical", k);
			std::string modelId = inter.c->Get("id", k);
			std::string md = inter.c->Get("md", k);
			f = path + f + ".dgn";

			pris(f + "  " + m);
			mdlSystem_setBatchProcessingState(PROCESSSTATE_Processing, NULL);
			if (SUCCESS == mdlSystem_newDesignFileAndModel(s2wsT(f).GetWCharCP(), s2wsT(m).GetWCharCP()))
			{
				pris("--跳转" + f + " " + m + " " + refer);
				findAllElements(pf, pm, f, m, refer, logical, modelId);
			}
		}
		else
		{
			Sleep(1000);
		}
	}	
}

void BGetOneDbData::creatFiles()
{
	fmsg;
	while ((_access((path + "db").c_str(), 0)) != -1)
	{
		if (RemoveDir((path + "db").c_str()))
			break;
		Sleep(5000);
		pris(path + "db 中的文件被别的程序打开，请关闭");
	}
	while ((_access((path + "obj").c_str(), 0)) != -1)
	{
		if (RemoveDir((path + "obj").c_str()))
			break;
		Sleep(5000);
		pris(path + "obj 中的文件被别的程序打开，请关闭");
	}

	while ((_access((path + "db").c_str(), 0)) == -1)
	{
		CreateDirectory((path + "db").c_str(), NULL);
		if ((_access((path + "db").c_str(), 0)) != -1)
		{
			pris("创建文件夹db成功");
			break;
		}
		else
		{
			Sleep(5000);
			pris(path + "创建文件夹db失败");
		}
	}
	while ((_access((path + "db/log").c_str(), 0)) == -1)
	{
		CreateDirectory((path + "db/log").c_str(), NULL);
		if ((_access((path + "db/log").c_str(), 0)) != -1)
		{
			pris("创建文件夹db/log成功");
			break;
		}
		else
		{
			Sleep(5000);
			pris(path + "创建文件夹db/log失败");
		}
	}
	while ((_access((path + "db/config").c_str(), 0)) == -1)
	{
		CreateDirectory((path + "db/config").c_str(), NULL);
		if ((_access((path + "db/config").c_str(), 0)) != -1)
		{
			pris("创建文件夹db/config成功");
			break;
		}
		else
		{
			Sleep(5000);
			pris(path + "创建文件夹db/config失败");
		}
	}

#ifdef MY_OBJ
	while ((_access((path + "obj").c_str(), 0)) == -1)
	{
		CreateDirectory((path + "obj").c_str(), NULL);
		if ((_access((path + "obj").c_str(), 0)) != -1)
		{
			pris("创建文件夹obj成功");
			break;
		}
		else
		{
			Sleep(5000);
			pris(path + "创建文件夹obj失败");
		}
	}
#endif
	//betley 版本输出
	VersionNumber   versionNumber;
	int             development;
	mdlVersion_getVersionNumbers(&versionNumber, &development);

	info(path, "bentley version:" + std::to_string(versionNumber.release) + "." + std::to_string(versionNumber.major) + "." + std::to_string(versionNumber.minor) + "." + std::to_string(versionNumber.subMinor));

	//插件 版本输出
	info(path, "plug-in version:0.1.0");

	//开始转换时间
	info(path, "开始提取时间" + getTime());

}

void BGetOneDbData::toTransform(std::string str, double myTransform[4][4])
{
	if (str != "")
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				std::string transformValue = str.substr(0, str.find(","));
				myTransform[i][j] = atof(transformValue.c_str());
				str = str.substr(str.find(",") + 1, str.length() - str.find(",") - 1);
			}
		}
	}
}

void BGetOneDbData::getCentre()
{
	treeClient t;
	t.sendMeg("GetCent:", ip.c_str(), port.c_str());
	std::string str = t.getRead();
	pris(str);

	std::string x, y, z;
	x = str.substr(0, str.find(","));
	str = str.substr(str.find(",") + 1, str.length() - str.find(",") - 1);
	y = str.substr(0, str.find(","));
	z = str.substr(str.find(",") + 1, str.length());
	centre.x = atof(x.c_str());
	centre.y = atof(y.c_str());
	centre.z = atof(z.c_str());
	priOther(centre.x); priOther(centre.y); priOther(centre.z);
}

void BGetOneDbData::findAllElements(std::string& oldFile,std::string& oldModel, std::string& thisFileName, std::string& thisModelName, std::string& transform, std::string& logicalName, std::string& modelId)
{
	fmsg;
	//pris(oldFile);
	//pris(oldModel);
	//pris(thisFileName);
	//pris(thisModelName);
	//pris(transform);
	//pris(logicalName);
	//pris(modelId);
	//pris(path);

	//treeClient t;
	//t.sendMeg("GetPath:", ip.c_str(), port.c_str());
	//path = t.getRead();

	try
	{
		getCentre();

		DgnModelP pActiveModel = ISessionMgr::GetActiveDgnModelP();
		DgnModel::ElementsCollection elemColl = pActiveModel->GetElementsCollection();

		int i = 0;
		int j = 1000000 * atoi(modelId.c_str());

		thisFileName = thisFileName.substr(thisFileName.rfind("/") + 1, thisFileName.rfind(".") - thisFileName.rfind("/") - 1);
		std::string db = this->path + "db/" + "(" + modelId + ")" + thisFileName + "_" + thisModelName + ".db";
		replace_all(db, " ", "_");
		WString dbName = s2wsT(db);
		sqlliteRW sqlW(dbName, this->path);
		if (sqlW.creatObjTable())
		{
			std::string tableElementElemGuidInfile = oldFile + "_" + oldModel + "(" + logicalName + ")--" + thisFileName + "_" + thisModelName + "_";

			double myTransform[4][4] = { 1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1 };
			if (transform != ""&& oldFile != "root" && oldModel != "root")
			{
				toTransform(transform, myTransform);
			}
			for (PersistentElementRefP elemRef : elemColl)
			{
				if (elemRef->IsGraphics())
				{
					EditElementHandle sourceEh(elemRef);
					i = (int)sourceEh.GetElementId() + j;
					callbackMsg(i, sqlW, dbName, sourceEh, tableElementElemGuidInfile, thisFileName, myTransform);
				}

			}
			sqlW.closeDB();
			if (i != 0)
			{
#ifdef  MY_ZIP
				thread t(compress, db);
				t.detach();
#endif //  MY_ZIP

#ifdef MY_LOG_PROGESS
				info(path, "结束提取：" + db);
#endif // MY_LOG_PROGESS
			}
			else
			{
#ifdef MY_LOG_PROGESS
				info(path, db + "中元素数为零，结束提取");
#endif // MY_LOG_PROGESS

				if ((_access(db.c_str(), 0)) != -1)
				{
					if (remove(db.c_str()) != 0)
					{
						pri(L"删除文件失败");
					}
				}
			}
		}
		else
		{
			sqlW.closeDB();
		}
	}
	catch (std::exception& e)
	{
		pris(e.what());
	}
}

void BGetOneDbData::callbackMsg(int & i, sqlliteRW & sqlW, WString & dbName, ElementHandleR sourceEh, std::string & tableElementElemGuidInfile, std::string & referFile, double myTransform[4][4])
{
	fmsg;
	WString name = sourceEh.GetDgnModelP()->GetModelName();
	std::string pros = ws2sT(doShowECInstance(sourceEh));

	if (pros.length() > 10)
	{
		if (findOne(pros, "ECClass  ").find("NormalCellElement") != std::string::npos)//cell 
		{
			for (ChildElemIter child(sourceEh); child.IsValid(); child = child.ToNext())
			{
				if (pros.length() > 100)
				{
					callbackMsg(i, sqlW, dbName, child, tableElementElemGuidInfile, referFile, myTransform);
				}
			}
		}
		else if (findOne(pros, "ECClass  ").find("Line") != std::string::npos)
		{
			//pris("Line");
		}
		else if (findOne(pros, "ECClass  ").find("ArcElement") != std::string::npos)
		{
			//pris("ArcElement");
		}
		else if (findOne(pros, "ECClass  ").find("Complex Chain") != std::string::npos)
		{
			//pris("ArcElement");
		}
		else if (findOne(pros, "ECClass  ").find("ComplexStringElement") != std::string::npos)
		{
			//pris("ArcElement");
		}
		else if (findOne(pros, "ECClass  ").find("BSplineCurveElement") != std::string::npos)
		{
			//pris("ArcElement");
		}
		else
		{
			if (pros.length() > 100)
				writeMsg(i, sqlW, dbName, sourceEh, pros, name, tableElementElemGuidInfile, referFile, myTransform);
		}
	}
}

void BGetOneDbData::writeMsg(int & i, sqlliteRW & sqlW, WString & dbName, ElementHandleR sourceEh, std::string & pros, WString & name, std::string & tableElementElemGuidInfile, std::string & referFile, double myTransform[4][4])
{
	fmsg;
	try
	{
		info(path, findOne(pros, "ECClass  "));

		sqlW.set_id(i);
		sqlW.set_hostfile_name(dbName);
		sqlW.set_id_infile((unsigned int)sourceEh.GetElementId());
		sqlW.set_elementID((int)(sourceEh.GetElementId()));

		getProperties(pros);//属性信息
		std::string dataStr; //几何信息
		std::string pic;
		std::string mtlName;
		bool colFlag;
		creatReference(sourceEh, name, sqlW, myTransform, dataStr, pic, mtlName, colFlag);

		//插入TbaleElement
		tableElementElemGuidInfile += std::to_string(sourceEh.GetElementId());
		//sqlW.insertTbaleElement(referFile, tableElementElemGuidInfile, pros);
		CMD5 md5;
		sqlW.insertTbaleElement(referFile, md5.md5(tableElementElemGuidInfile), md5.md5(pros));

		//插入TableElementParameter
		sqlW.addDbElementParameter2(this->properties);//OK

													  //插入ObjTbale
		sqlW.insertObjTbale(i, ws2sT(getRootPath(ROOTFILE::rootModel)), dataStr);

		//插入MatTable
		if (colFlag)
		{
			sqlW.insertMatTable(i, mtlName, "");
		}

		//插入TexTable
		//sqlW.insertTexTable(pic, "");

		i++;
	}
	catch (std::exception& e)
	{
		char strE[256] = "\0";
		sprintf(strE, "%s %s %d", __FILE__, __FUNCTION__, __LINE__);
		infoModel(path, strE);
		infoModel(path, ws2sT(getRootPath(ROOTFILE::rootFileAndModelPath)));

		infoModel(path, e.what());
	}
}

WString BGetOneDbData::doShowECInstance(ElementHandleR elem)
{
	fmsg;
	WString classText = L"";
	try
	{
		FindInstancesScopePtr scope = FindInstancesScope::CreateScope(elem, FindInstancesScopeOption(DgnECHostType::Element));
		ECQueryPtr query = ECQuery::CreateQuery(ECQUERY_PROCESS_SearchAllClasses);

		DgnECInstanceIterable iterable = Bentley::DgnPlatform::DgnECManager::GetManager().FindInstances(*scope, *query);
		for (DgnECInstanceIterable::const_iterator it = iterable.begin(); it != iterable.end(); ++it)
		{
			DgnECInstancePtr instance = *it;
			ECN::ECClassCR instanceClass = instance->GetClass();
			classText += WString(L"ECClass  ") + instanceClass.GetName() + WString(L"\n");

			ECPropertyIterable pros = instanceClass.GetProperties();
			for (ECPropertyIterable::const_iterator it_local = pros.begin(); it_local != pros.end(); ++it_local)
			{
				ECValue v;
				ECPropertyCP pProp = (*it_local);

				if (SUCCESS == instance->GetValue(v, pProp->GetName().c_str()))
					classText += pProp->GetName() + WString(L"  ") + v.ToString() + WString(L"\n");
			}
		}
	}
	catch (std::exception& e)
	{
		pris(e.what());
	}
	return classText;
}

std::string BGetOneDbData::findOne(const std::string & str, const std::string & find)
{
	//fmsg;
	std::string rtn = "";
	try
	{
		int start = (int)(str.find(find.c_str()) + find.length());
		int end = (int)(str.find("\n", start));

		if (start < end && end < str.length())
			rtn = str.substr(start, end - start);
	}
	catch (std::exception& e)
	{
		char strE[256] = "\0";
		sprintf(strE, "%s %s %d", __FILE__, __FUNCTION__, __LINE__);
		infoModel(path, strE);
		infoModel(path, ws2sT(getRootPath(ROOTFILE::rootFileAndModelPath)));
		infoModel(path, e.what());
	}
	return rtn;
}


void BGetOneDbData::getProperties(std::string & str)
{
	fmsg;
	try
	{
		this->properties.general.ElementDescription = findOne(str, "ElementDescription  ");
		this->properties.general.Color = findOne(str, "Color  ");
		this->properties.general.Style = findOne(str, "Style  ");
		this->properties.general.Weight = findOne(str, "Weight  ");
		this->properties.general.Transparency = findOne(str, "Transparency  ");

		//几何图形
		this->properties.geometry.RotationX = findOne(str, "RotationX  ");
		this->properties.geometry.RotationY = findOne(str, "RotationY  ");
		this->properties.geometry.RotationZ = findOne(str, "RotationZ  ");

		double uorPerMeter = mdlModelRef_getUorPerMeter(ACTIVEMODEL);
		this->properties.geometry.Volume = std::to_string(atof(findOne(str, "Volume  ").c_str()) / pow(uorPerMeter, 3));
		this->properties.geometry.SurfaceArea = std::to_string(atof(findOne(str, "SurfaceArea  ").c_str()) / pow(uorPerMeter, 2));

		//材料
		this->properties.material.AttachedMaterialConcretePavers = findOne(str, "AttachedMaterial  ");

		//延长
		this->properties.externded.Model = findOne(str, "Model  ");
		this->properties.externded.ModifiedTime = findOne(str, "ModifiedTime  ");
		this->properties.externded.Modified = findOne(str, "Modified  ");
		this->properties.externded.New = findOne(str, "New  ");
		this->properties.externded.Locked = findOne(str, "Locked  ");

		//原始数据
		this->properties.rawdata.ElementID = findOne(str, "ElementID  ");
		this->properties.rawdata.ElementSize = findOne(str, "ElementSize  ");
		this->properties.rawdata.Linkages = findOne(str, "Linkages  ");
		this->properties.rawdata.XAttributes = findOne(str, "XAttributes  ");
	}
	catch (std::exception& e)
	{
		char strE[256] = "\0";
		sprintf(strE, "%s %s %d", __FILE__, __FUNCTION__, __LINE__);
		infoModel(path, strE);
		infoModel(path, ws2sT(getRootPath(ROOTFILE::rootFileAndModelPath)));
		infoModel(path, e.what());
	}
}



IntColorDef BelemColor(ElementHandleR sourceEh)
{
	fmsg;
	IntColorDef colorDef;
	ElementPropertiesGetterPtr getColorPara = ElementPropertiesGetter::Create(sourceEh);
	UInt32 colorIndex;
	bool isTrueColor;
	WString bookName;
	WString colorName;
	DgnFileP dgnFile = sourceEh.GetDgnFileP();
	//DgnColorMap::ExtractElementColorInfo(&colorDef, &colorIndex, &isTrueColor, &bookName, &colorName, getColorPara->GetColor(), *dgnFile);

	UInt32 clrId = getColorPara->GetColor();
	if (COLOR_BYLEVEL == clrId)
	{
		LevelId lvlId = getColorPara->GetLevel();
		LevelCacheR lvlCache = sourceEh.GetDgnModelP()->GetLevelCacheR();
		LevelHandle lvlHandle = lvlCache.GetLevel(lvlId);
		clrId = lvlHandle.GetByLevelColor().GetColor();
	}

	DgnColorMap::ExtractElementColorInfo(&colorDef, &colorIndex, &isTrueColor, &bookName, &colorName, clrId, *dgnFile);

	return colorDef;
}

bool BGetOneDbData::creatReference(ElementHandleR sourceEh, WString modelNamel, sqlliteRW & sqlW, double myTransform[4][4], std::string & dataStr, std::string & pic, std::string & mtlName, bool & colorFlag)
{
	fmsg;
	bool rtn = true;
	try
	{
		treeClient t;
		t.sendMeg("GetPrec:", ip.c_str(), port.c_str());
		std::string precision = t.getRead();

		//pri(L"CreateElement");
		IFacetOptionsPtr facetOptions = IFacetOptions::New();
		//Set different parameters for facet.
		facetOptions->SetIgnoreFaceMaterialAttachments(false); // Don't separate multi-symbology BReps by face symbology...
		facetOptions->SetChordTolerance(atof(precision.c_str()));//many different parameters to control the final result mesh
		facetOptions->SetAngleTolerance(0.0);
		facetOptions->SetMaxEdgeLength(0.0);
		facetOptions->SetMaxFacetWidth(0.0);
		facetOptions->SetNormalsRequired(true);
		facetOptions->SetParamsRequired(true);
		facetOptions->SetVertexColorsRequired(true);
		facetOptions->SetMaxPerFace(3);
		facetOptions->SetCurvedSurfaceMaxPerFace(3);
		facetOptions->SetEdgeHiding(true);
		facetOptions->SetSmoothTriangleFlowRequired(true);

		bvector<PolyfaceHeaderPtr> meshes;

		if (true == ElementToApproximateFacets(sourceEh, meshes, facetOptions.get()))
		{
			size_t n = meshes.size();
			for (size_t i = 0; i < n; i++)
			{
				IntColorDef colorDef = BelemColor(sourceEh);

				pic = materialInfo(sourceEh);
				mtlName = sqlW.checkMtl(colorDef.m_rgba, pic);//检测材质是否相同
				if ("" == mtlName)
				{
					mtlName = std::to_string(sqlW.get_id());
					sqlW.set_color(colorDef.m_rgba, mtlName, pic);
					colorFlag = true;
				}
				else
					colorFlag = false;

				WString str = get(meshes[i], myTransform, mtlName, sqlW);
				//sqlW.addData(str, id);		
				dataStr = ws2sT(str);
			}
		}
		else
			rtn = false;

	}
	catch (std::exception& e)
	{
		char strE[256] = "\0";
		sprintf(strE, "%s %s %d", __FILE__, __FUNCTION__, __LINE__);
		infoModel(path, strE);
		infoModel(path, ws2sT(getRootPath(ROOTFILE::rootFileAndModelPath)));
		infoModel(path, e.what());
	}
	return rtn;
}

WString BGetOneDbData::get(PolyfaceHeaderPtr meshData, double myTransform[4][4], const std::string & colorName, sqlliteRW & sqlW)
{
	fmsg;
	std::string str = "";
	try
	{
		std::string mtlFile = sqlW.getMtlFile();
		mtlFile = mtlFile.substr(mtlFile.rfind("/") + 1, mtlFile.length() - mtlFile.rfind("/") - 1);
		str = "mtllib " + mtlFile + "\no\n";
		char buf[256] = "\0";
		WString msg;

#ifdef GET
		pri(L"点");
#endif // STEP
		//点 
		size_t pointCount = meshData->GetPointCount();
		double uorPerMeter = mdlModelRef_getUorPerMeter(ACTIVEMODEL);
		DPoint3dCP point = meshData->GetPointCP();
		
		for (size_t i = 0; i < pointCount; i++)
		{
			double myPoint[4] = { point->x / uorPerMeter,point->y / uorPerMeter,point->z / uorPerMeter,1 };
			double myOut[3] = { 0 };

			for (int n = 0; n < 4; n++)
			{
				myOut[n] = myTransform[n][0] * myPoint[0] + myTransform[n][1] * myPoint[1] + myTransform[n][2] * myPoint[2] + myTransform[n][3] * myPoint[3];
			}

			sprintf(buf, "v %.7f %.7f %.7f\n", myOut[0] - centre.x, myOut[1] - centre.y, myOut[2] - centre.z);
			//sprintf(buf, "v %.7f %.7f %.7f\n", myOut[0], myOut[1], myOut[2]);
			str += buf;
			memset(buf, '\0', 256);

			point++;
		}

#ifdef GET
		pri(L"法线");
#endif // STEP
		//法线
		size_t normalCount = meshData->GetNormalCount();
		DVec3dCP normal = meshData->GetNormalCP();
		for (size_t i = 0; i < normalCount; i++)
		{
			double myPoint[4] = { normal->x,normal->y,normal->z,1 };
			double myOut[3] = { 0 };

			for (int n = 0; n < 3; n++)
			{
				myOut[n] = myTransform[n][0] * myPoint[0] + myTransform[n][1] * myPoint[1] + myTransform[n][2] * myPoint[2];
			}

			double divide = sqrt(myOut[0] * myOut[0] + myOut[1] * myOut[1] + myOut[2] * myOut[2]);

			sprintf(buf, "vn %.7f %.7f %.7f\n", myOut[0] / divide, myOut[1] / divide, myOut[2] / divide);
			str += buf;
			memset(buf, '\0', 256);

			normal++;
		}

		if (colorName != "")
			str += "usemtl " + colorName + "\n";


#ifdef GET
		pri(L"面");
#endif // STEP
		//面		
		size_t PointIndexCount = meshData->GetPointIndexCount();
		int32_t const* normalIndex = meshData->GetNormalIndexCP(false);
		const int32_t* pi = meshData->GetPointIndexCP();
		str += "f ";
		for (size_t i = 0; i < PointIndexCount; i++)
		{
			if (normalIndex != nullptr)
				sprintf(buf, "%d//%d ", (int)(abs(*pi)), *normalIndex);
			else
				sprintf(buf, "%d ", (int)(abs(*pi)));
			if (*pi != 0)
			{
				str += buf;
				memset(buf, '\0', 256);
			}
			else
			{
				if (i < PointIndexCount - 1)
					str += "\nf ";
			}
			pi++;
			if (normalIndex != nullptr)
				normalIndex++;
		}


		meshData->GetTextureId();
	}
	catch (std::exception& e)
	{
		char strE[256] = "\0";
		sprintf(strE, "%s %s %d", __FILE__, __FUNCTION__, __LINE__);
		infoModel(path, strE);
		infoModel(path, ws2sT(getRootPath(ROOTFILE::rootFileAndModelPath)));
		infoModel(path, e.what());
	}
	return WString(str.c_str());
}

bool BGetOneDbData::ElementToApproximateFacets(ElementHandleCR source, bvector<PolyfaceHeaderPtr>& output, IFacetOptionsP options)
{
	fmsg;
	try
	{
		output.clear();
		MeshProcessor dest(output, options);

		ElementGraphicsOutput::Process(source, dest);
	}
	catch (std::exception& e)
	{
		infoModel(path, e.what());
	}
	return output.size() > 0 ? true : false;
}

std::string BGetOneDbData::materialInfo(ElementHandleR sourceEh)
{
	fmsg;
	try
	{
		WString atomTypeing;
		if (sourceEh.GetElementRef()->IsGraphics())
		{
			IMaterialPropertiesExtension* mExtension = IMaterialPropertiesExtension::Cast(sourceEh.GetHandler());
			if (mExtension)
			{
				DgnModelRefP modelRef = sourceEh.GetModelRef();
				MaterialCP m = mExtension->FindMaterialAttachment(sourceEh, *modelRef);
				if (m != NULL)
				{
					MaterialId  tempId(*m);
					MaterialMapCollectionCR maps = m->GetSettings().GetMaps();
					MaterialMapCP pMap = maps.GetMapCP(MaterialMap::MAPTYPE_Pattern);
					if (pMap != NULL && pMap->GetType() == MaterialMap::MAPTYPE_Pattern)
					{
						MaterialMapLayerCR  layer = pMap->GetLayers().GetTopLayer();
						ImageMapMonikersCR mapmons = layer.GetImageMapMonikers();
						for (auto mapmon : mapmons)
						{
							std::string src = ws2sT(mapmon->GetSavedFileName());
							if (!CopyFile(src.c_str(), (this->path + "obj/" + ws2sT(mapmon->GetShortDisplayName())).c_str(), true))
							{
								if (2 == GetLastError())
								{
									src = "C:\\ProgramData\\Bentley\\connectsharedcontent\\materials\\Patterns\\" + src;
									CopyFile(src.c_str(), (this->path + "obj/" + ws2sT(mapmon->GetShortDisplayName())).c_str(), true);
								}
							}
							return ws2sT(mapmon->GetShortDisplayName());
						}
					}
				}
			}
		}
	}
	catch (std::exception& e)
	{
		char strE[256] = "\0";
		sprintf(strE, "%s %s %d", __FILE__, __FUNCTION__, __LINE__);
		infoModel(path, strE);
		infoModel(path, ws2sT(getRootPath(ROOTFILE::rootFileAndModelPath)));
		infoModel(path, e.what());
	}
	return "";
}


//调用命令行命令而不显示命令行窗口  
BOOL systemHide(char* CommandLine)
{
	SECURITY_ATTRIBUTES   sa;
	HANDLE   hRead, hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return   FALSE;
	}

	STARTUPINFO   si;
	PROCESS_INFORMATION   pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	//关键步骤，CreateProcess函数参数意义请查阅MSDN     
	if (!CreateProcess(NULL, CommandLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		return   FALSE;
	}
	CloseHandle(hWrite);

	char   buffer[4096] = { 0 };
	DWORD   bytesRead;
	while (true)
	{
		memset(buffer, 0, strlen(buffer));
		if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL)
			break;
		//buffer中就是执行的结果，可以保存到文本，也可以直接输出     
		//printf(buffer);//这行注释掉就可以了     
		Sleep(100);
	}
	return   TRUE;
}
void BGetOneDbData::compress(const std::string & src)
{
	std::string tobe = "tobe.exe ";
	std::string des1 = src;
	replace_all(des1, ".db", ".zip ");
	std::string newFile1 = src.substr(src.rfind("/") + 1, src.length() - src.rfind("/") - 1) + " ";
	std::string pw = newFile1;
	replace_all(pw, ".db ", ".be");
	CMD5 md5;
	std::string strS1 = tobe + des1 + newFile1 + src + " " + md5.md5(toUtf8(pw));

	char commLine[1024] = "\0";
	strcpy_s(commLine, strS1.c_str());
	systemHide(commLine);
	//system(strS1.c_str());

#ifndef MY_INITIAL_ZIP
	remove(src.c_str());
#endif // !MY_INITIAL_ZIP



	std::string tobj = "tobj.exe ";
	std::string des2 = src;
	replace_all(des2, ".db", "_bj.zip ");
	std::string newFile2 = newFile1;
	replace_all(newFile2, ".db", ".be ");
	std::string src2 = src;
	replace_all(src2, ".db", ".be");
	std::string strS2 = tobj + des2 + newFile2 + src2;
	//pris(strS2);

	memset(commLine, '\0', 1024);
	strcpy_s(commLine, strS2.c_str());
	systemHide(commLine);

	//system(strS2.c_str());

#ifndef MY_INITIAL_ZIP
	remove(src2.c_str());
#endif // !MY_INITIAL_ZIP
}
