#include "sqlliteRW.h"
#include "utility.h"
//#include "..\include\easySQLite\easySqlite.h"
//#include "..\include\easySQLite\SqlCommon.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <streambuf> 

#include <Windows.h>
#include <combaseapi.h>
#pragma comment (lib, "ole32.lib")

using namespace std;

sqlliteRW::sqlliteRW(WString fileName, std::string path):path(path)
{
	obj.id = 1;
	this->fileName = ws2sT(fileName);
	this->objNum = 0;
	this->m_rgba.red = '\0';
	this->m_rgba.green = '\0';
	this->m_rgba.blue = '\0';
	this->m_rgba.alpha = '\0';

	this->mtlFlag = false;
	this->ElementNum = 0;
	this->mtlFileName = this->fileName.substr(0, this->fileName.rfind(".")) + ".mtl";
	replace_all(this->mtlFileName, "/db/", "/obj/");
	this->colorName = "1";
}


sqlliteRW::~sqlliteRW()
{
}

bool sqlliteRW::creatObjTable()
{
	fmsg;
	char *zErrMsg = 0;

	if ((_access(this->mtlFileName.c_str(), 0)) != -1)
	{
		pris("有重复文件存在，文件名："+ this->mtlFileName);
	}

	int result = openDB();
	if (!result)
	{
#ifdef MY_DEBUG
		pri(L"Open the database sqlite.db sucessfully\n");
#endif
	}
	else
	{
		return false;
	}

	/* Create Table_Element */
	char* Table_Element = "CREATE TABLE Table_Element("  \
		"id INTEGER PRIMARY KEY AUTOINCREMENT," \
		"hostfile_name         TEXT," \
		"elem_guid_infile      TEXT," \
		"elem_param_tab      TEXT);";
	result = sqlite3_exec(this->db, Table_Element, 0, 0, &zErrMsg);
	if (result != SQLITE_OK) {
		pri(L"SQL error:创建Table_Element失败");
		sqlite3_free(zErrMsg);
		return false;
	}

	/* Create Obj_Table */
	char* Obj_Table = "CREATE TABLE Obj_Table("  \
		"id INTEGER PRIMARY KEY AUTOINCREMENT," \
		"obj_file_name      TEXT," \
		"id_in_element         INT," \
		"obj_file_Data         BLOB );";
	result = sqlite3_exec(this->db, Obj_Table, 0, 0, &zErrMsg);
	if (result != SQLITE_OK) {
		pri(L"SQL error:创建Obj_Table失败");
		sqlite3_free(zErrMsg);
		return false;
	}
	/* Create Mat_Table */
	char* Mat_Table = "CREATE TABLE Mat_Table("  \
		"id  INT," \
		"Mtl_File_Name         CHAR(50)," \
		"Material_Data         BLOB);";
	result = sqlite3_exec(this->db, Mat_Table, 0, 0, &zErrMsg);
	if (result != SQLITE_OK) {
		pri(L"SQL error:创建Mat_Table失败");
		sqlite3_free(zErrMsg);
		return false;
	}
	/* Create Tex_Table */
	char* Tex_Table = "CREATE TABLE Tex_Table("  \
		"id INTEGER PRIMARY KEY AUTOINCREMENT," \
		"pic_Name         CHAR(50)," \
		"pic_Data         BLOB);";
	result = sqlite3_exec(this->db, Tex_Table, 0, 0, &zErrMsg);
	if (result != SQLITE_OK) {
		pri(L"SQL error:创建Tex_Table失败");
		sqlite3_free(zErrMsg);
		return false;
	}

	/* Create Table_Element */
	char* Table_Element_Parameter = "CREATE TABLE Table_Element_Parameter("  \
		"id_in_element             INT," \
		"param_name           CHAR(50)," \
		"param_group          CHAR(50)," \
		"param_value          CHAR(50)," \
		"unit_type            CHAR(50));";
	result = sqlite3_exec(this->db, Table_Element_Parameter, 0, 0, &zErrMsg);
	if (result != SQLITE_OK) {
		pri(L"SQL error:创建Table_Element_Parameter失败");
		pri(s2wsT(std::to_string(result)).GetWCharCP());
		sqlite3_free(zErrMsg);
		return false;
	}

	/* Create Table_Tree */
	char* Table_Tree = "CREATE TABLE Table_Tree("  \
		"id_in_element             INT," \
		"level_1          TEXT," \
		"level_2          TEXT," \
		"level_3          TEXT," \
		"level_4          TEXT);";
	result = sqlite3_exec(this->db, Table_Tree, 0, 0, &zErrMsg);
	if (result != SQLITE_OK) {
		pri(L"SQL error:创建Table_Tree失败");
		pri(s2wsT(std::to_string(result)).GetWCharCP());
		sqlite3_free(zErrMsg);
		return false;
	}
	/* Create Table_view3D_Info */
	char* Table_view3D_Info = "CREATE TABLE Table_view3D_Info("  \
		"id INTEGER PRIMARY KEY AUTOINCREMENT," \
		"view_name          TEXT," \
		"hostfile_name             INT);";
	result = sqlite3_exec(this->db, Table_view3D_Info, 0, 0, &zErrMsg);
	if (result != SQLITE_OK) {
		pri(L"SQL error:创建Table_view3D_Info失败");
		pri(s2wsT(std::to_string(result)).GetWCharCP());
		sqlite3_free(zErrMsg);
		return false;
	}
	/* Create Table_view_element */
	char* Table_view_element = "CREATE TABLE Table_view_element("  \
		"id_in_view3D             INT," \
		"hostfile_name          TEXT," \
		"id_in_element             INT);";
	result = sqlite3_exec(this->db, Table_view_element, 0, 0, &zErrMsg);
	if (result != SQLITE_OK) {
		pri(L"SQL error:创建Table_view_element失败");
		pri(s2wsT(std::to_string(result)).GetWCharCP());
		sqlite3_free(zErrMsg);
		return false;
	}

	return true;
}

bool sqlliteRW::deleteDbFile()
{
	fmsg;
	if ((_access(this->fileName.c_str(), 0)) != -1)
	{
		if (remove(this->fileName.c_str()) != 0)
		{
			pri(L"删除文件失败");
		}
	}
	return false;
}

int sqlliteRW::openDB()
{
	fmsg;
	sqlite3_open(toUtf8(this->fileName).c_str(), &this->db);

	char *errorMsg;
	if (sqlite3_exec(this->db, "BEGIN", NULL, NULL, &errorMsg) == SQLITE_OK)
	{
		return 0;
	}
	return 1;
}

void sqlliteRW::closeDB()
{
	fmsg;
	char *errorMsg;
	sqlite3_exec(this->db, "COMMIT", NULL, NULL, &errorMsg);

	sqlite3_close(this->db);
}

void sqlliteRW::insertTbaleElement(const std::string & hostfile_name, const std::string & elem_guid_infile, const std::string & elem_param_tab)
{
	fmsg;
#ifdef  MY_DB
	sqlite3_stmt *stat;
	sqlite3_prepare(this->db, "insert into Table_Element (hostfile_name,elem_guid_infile,elem_param_tab) values(?,?,?);", (int)-1, &stat, (int)0);
	std::string hostfile = UnicodeToUtf8N(s2ws(hostfile_name).c_str());
	sqlite3_bind_text(stat, 1, hostfile.c_str(), (int)(hostfile.length() + 1), NULL);
	std::string fileN = UnicodeToUtf8N(s2ws(elem_guid_infile).c_str());
	std::string elem_guid = UnicodeToUtf8N(s2ws(fileN).c_str());
	sqlite3_bind_text(stat, 2, elem_guid.c_str(), (int)elem_guid.length(), NULL);
	sqlite3_bind_blob(stat, 3, elem_param_tab.c_str(), (int)(elem_param_tab.length() + 1), NULL);

	sqlite3_step(stat);
	sqlite3_finalize(stat);
#endif //  MY_DB
}


void writeObjFile(std::string objName,std::string obj_file_Data)
{
	fmsg;
	ofstream out(objName, ios::app);
	if (out.is_open())
	{
		out << obj_file_Data.c_str() << endl;
		out.close();
	}
}


bool sqlliteRW::insertObjTbale(const int id_in_element, const std::string & modelN, const std::string & obj_file_Data)
{
	fmsg;

#ifdef  MY_DB
	sqlite3_stmt *stat;
	sqlite3_prepare(this->db, "insert into Obj_Table (id_in_element,obj_file_name,obj_file_Data) values(?,?,?);", (int)-1, &stat, (int)0);

	sqlite3_bind_int(stat, 1, id_in_element);
	std::string elem_guid = UnicodeToUtf8N(s2ws(modelN).c_str());
	sqlite3_bind_text(stat, 2, elem_guid.c_str(), (int)elem_guid.length(), NULL);
	sqlite3_bind_blob(stat, 3, obj_file_Data.c_str(), (int)(obj_file_Data.length() + 1), NULL);

	sqlite3_step(stat);
	sqlite3_finalize(stat);
#endif
#ifdef MY_OBJ	
	std::string addNum1 = this->fileName.substr(this->fileName.find("(") + 1, this->fileName.find(")")- this->fileName.find("(") -1);

	std::string objName = this->fileName.substr(0, this->fileName.rfind(".")) + "_[" + std::to_string(id_in_element) + "].obj";

	replace_all(objName,"/db/","/obj/");

	thread t(writeObjFile, objName, obj_file_Data);
	t.detach();

#endif // 
	return true;
}

bool sqlliteRW::insertMatTable(const int id, const std::string & Mtl_File_Name, const std::string & Material_Data)
{
	fmsg;

	std::string rgbaKa, rgbaKd, rgbaD;
	std::string mtlContent = "newmtl " + this->colorName + "\n";

	mtlContent += rgbaToStr(this->m_rgba, rgbaKa, rgbaKd, rgbaD);

	if(this->pic != "")
		mtlContent += "map_Kd " + this->pic + "\n";
	else
		mtlContent += "\n";

#ifdef  MY_DB
	sqlite3_stmt *stat;
	sqlite3_prepare(this->db, "insert into Mat_Table (id,Mtl_File_Name,Material_Data) values(?,?,?);", (int)-1, &stat, (int)0);

	sqlite3_bind_int(stat, 1, id);
	std::string File_Name = UnicodeToUtf8N(s2ws(Mtl_File_Name).c_str());
	sqlite3_bind_text(stat, 2, File_Name.c_str(), (int)File_Name.length(), NULL);
	sqlite3_bind_blob(stat, 3, mtlContent.c_str(), (int)(mtlContent.length() + 1), NULL);

	sqlite3_step(stat);
	sqlite3_finalize(stat);
#endif

#ifdef MY_OBJ
	mtlContent += "\n";

	thread t(writeObjFile, this->mtlFileName, mtlContent);
	t.detach();

#endif
	return true;
}

void sqlliteRW::insertTexTable(const std::string & pic_Name, const std::string & pic_Data)
{
	fmsg;
	sqlite3_stmt *stat;
	sqlite3_prepare(this->db, "insert into Tex_Table (pic_Name,pic_Data) values(?,?);", (int)-1, &stat, (int)0);

	std::string picName = UnicodeToUtf8N(s2ws(pic_Name).c_str());
	sqlite3_bind_text(stat, 1, picName.c_str(), (int)picName.length(), NULL);
	sqlite3_bind_blob(stat, 2, pic_Data.c_str(), (int)(pic_Data.length() + 1), NULL);

	sqlite3_step(stat);
	sqlite3_finalize(stat);
}

void sqlliteRW::insertTableElementParameter(const int id_in_element, const std::string & param_name, const std::string & param_group, const std::string & param_value, const std::string & unit_type)
{
	fmsg;
	sqlite3_stmt *stat;
	sqlite3_prepare(db, "insert into Table_Element_Parameter (id_in_element,param_name,param_group,param_value,unit_type) values(?,?,?,?,?);", (int)-1, &stat, (int)0);

	sqlite3_bind_int(stat, 1, id_in_element);
	std::string str1 = UnicodeToUtf8(s2wsT(param_name).c_str());
	sqlite3_bind_text(stat, 2, str1.c_str(), (int)(str1.length()) + 1, NULL);
	std::string str2 = UnicodeToUtf8(s2wsT(param_group).c_str());
	sqlite3_bind_text(stat, 3, str2.c_str(), (int)(str2.length()) + 1, NULL);

	std::string valueStr;
	if (param_value == "true")
		valueStr = "是";
	else
		valueStr = "否";
	std::string str3 = UnicodeToUtf8(s2wsT(valueStr).c_str());
	sqlite3_bind_text(stat, 4, str3.c_str(), (int)(str3.length()) + 1, NULL);
	std::string str4 = UnicodeToUtf8(s2wsT(unit_type).c_str());
	sqlite3_bind_text(stat, 5, str4.c_str(), (int)(str4.length()) + 1, NULL);

	sqlite3_step(stat);
	sqlite3_finalize(stat);
}

void sqlliteRW::set_id(int& id)
{
	this->obj.id = id;
}

void sqlliteRW::set_hostfile_name(WString hostfile_name)
{
	this->obj.hostfile_name = ws2s(hostfile_name.GetWCharCP());
}

void sqlliteRW::set_elem_guid_infile(WString elem_guid_infile)
{
	this->obj.elem_guid_infile = ws2s(elem_guid_infile.GetWCharCP());
}

void sqlliteRW::set_id_infile(int id_infile)
{
	this->obj.id_infile = id_infile;
}

void sqlliteRW::set_guid(int guid)
{
	this->obj.guid = guid;
}

void sqlliteRW::set_elementID(int id)
{
	this->elementID = id;
}

int sqlliteRW::get_id()
{
	return this->obj.id;
}

std::string sqlliteRW::getMtlFile()
{
	return this->mtlFileName;
}

void sqlliteRW::set_color(const RgbaColorDef& rgba,const std::string& color, const std::string& picIn)
{
	fmsg;
	this->m_rgba.red = rgba.red;
	this->m_rgba.green = rgba.green;
	this->m_rgba.blue = rgba.blue;
	this->m_rgba.alpha = rgba.alpha;

	this->colorName = color;
	this->mtlFlag = true;
	this->pic = picIn;
}

void sqlliteRW::addDbElementParameterOne2(const std::string & name, const std::string & group, const std::string & value, const std::string & type)
{
	//fmsg;
	sqlite3_stmt *stat;
	sqlite3_prepare(this->db, "insert into Table_Element_Parameter (id_in_element,param_name,param_group,param_value,unit_type) values(?,?,?,?,?);", (int)-1, &stat, (int)0);

	sqlite3_bind_int(stat, 1, this->obj.id);
	std::string str1 = UnicodeToUtf8(s2wsT(name).c_str());
	sqlite3_bind_text(stat, 2, str1.c_str(), (int)(str1.length()) + 1, NULL);
	std::string str2 = UnicodeToUtf8(s2wsT(group).c_str());
	sqlite3_bind_text(stat, 3, str2.c_str(), (int)(str2.length()) + 1, NULL);

	std::string valueStr;
	if (value == "True")
		valueStr = "是";
	else if(value == "False")
		valueStr = "否";

	std::string str3 = UnicodeToUtf8(s2wsT(value).c_str());
	sqlite3_bind_text(stat, 4, str3.c_str(), (int)(str3.length()) + 1, NULL);
	std::string str4 = UnicodeToUtf8(s2wsT(type).c_str());
	sqlite3_bind_text(stat, 5, str4.c_str(), (int)(str4.length()) + 1, NULL);

	sqlite3_step(stat);
	sqlite3_finalize(stat);
}

#define myPI 3.14159265358979323846

void sqlliteRW::addDbElementParameter2(PROPERTRY & pros)
{
	fmsg;
#ifdef  MY_DB
	//综合
	addDbElementParameterOne2( "描述", "综合", pros.general.ElementDescription, "");
	addDbElementParameterOne2( "颜色", "综合", pros.general.Color, "");
	addDbElementParameterOne2( "线型", "综合", pros.general.Style, "");
	addDbElementParameterOne2( "线宽", "综合", pros.general.Weight, "");
	addDbElementParameterOne2( "透明度", "综合", pros.general.Transparency, "");

	//几何图形
	double rotationX = atof(pros.geometry.RotationX.c_str());
	rotationX = myPI * rotationX;
	addDbElementParameterOne2( "绕 X 轴旋转", "几何图形", std::to_string(rotationX), "度");
	double rotationY = atof(pros.geometry.RotationY.c_str());
	rotationY = myPI * rotationY;
	addDbElementParameterOne2( "绕 Y 轴旋转", "几何图形", std::to_string(rotationY), "度");
	double rotationZ = atof(pros.geometry.RotationZ.c_str());
	rotationZ = myPI * rotationZ;
	addDbElementParameterOne2( "绕 Z 轴旋转", "几何图形", std::to_string(rotationZ), "度");
	addDbElementParameterOne2( "体积", "几何图形", pros.geometry.Volume, "立方米");
	addDbElementParameterOne2( "表面积", "几何图形", pros.geometry.SurfaceArea, "平方米");

	//材料
	addDbElementParameterOne2( "连接材料", "材料", pros.material.AttachedMaterialConcretePavers, "");

	//延长
	addDbElementParameterOne2( "模型", "延长", pros.externded.Model, "");
	addDbElementParameterOne2( "上次修改时间", "延长", pros.externded.ModifiedTime, "");
	std::string modified;
	addDbElementParameterOne2( "修改过", "延长", pros.externded.Modified, "");
	addDbElementParameterOne2( "新建", "延长", pros.externded.New, "");
	addDbElementParameterOne2( "已锁定", "延长", pros.externded.Locked, "");

	//原始数据
	addDbElementParameterOne2( "元素标识", "原始数据", pros.rawdata.ElementID, "");
	addDbElementParameterOne2( "尺寸", "原始数据", pros.rawdata.ElementSize, "米");
	addDbElementParameterOne2( "链接", "原始数据", pros.rawdata.Linkages, "");
	addDbElementParameterOne2( "XAttributes", "原始数据", pros.rawdata.XAttributes, "");
#endif
}

std::string sqlliteRW::checkMtl(const RgbaColorDef & rgba, const std::string& picIn)
{
	fmsg;
	std::string rtn = "";
	ifstream inFile(this->mtlFileName);
	std::string line;
	std::string ka, kb, d, map_Kd;

	std::string rgbaKa, rgbaKd, rgbaD;
	rgbaToStr(rgba, rgbaKa, rgbaKd, rgbaD);
	
	while (inFile)
	{
		getline(inFile, line);
		if (line.find("newmtl ") != 0)
			continue;

		getline(inFile, ka);
		getline(inFile, kb);
		getline(inFile, d);
		getline(inFile, map_Kd);

		if (ka.substr(0,29) == rgbaKa.substr(0,29) && kb.substr(0, 29) == rgbaKd.substr(0,29) && d.substr(0, 10) == rgbaD.substr(0,10))
		{
			if (picIn == "")
			{
				if (map_Kd.length() <= 7)
				{
					return line.substr(7, line.length() - 7);
				}
			}
			else
			{
				if(map_Kd.length() > 7)
					if (map_Kd.substr(7, map_Kd.length() - 7) == picIn.substr(0, map_Kd.length() - 7))
					{
						return line.substr(7, line.length() - 7);
					}
			}
		}
	}
	inFile.close();
	return rtn;
}

std::string sqlliteRW::rgbaToStr(const RgbaColorDef & rgba, std::string& ka, std::string& kd, std::string& d)
{
	fmsg;
	std::string value;

	value = std::to_string((double)(rgba.red) / 255) + " " + std::to_string((double)(rgba.green) / 255) + " " + std::to_string((double)(rgba.blue) / 255) + "\n";
	ka = "ka " + value;
	kd = "kd " + value;
	d = "d " + std::to_string(1 - (double)(rgba.alpha)) + "\n";

	return ka+kd+d;
}


