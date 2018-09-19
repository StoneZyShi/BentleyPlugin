#pragma once
#include <string>
#include "utility.h"
#include "..\include\easySQLite\sqlite3.h"


struct PROPERTRY {
	struct GENERAL {
		std::string ElementDescription;
		std::string Level;
		std::string Color;
		std::string Style;
		std::string Weight;
		std::string Transparency;
	} general;
	struct GEOMETRY {
		std::string RotationX;
		std::string RotationY;
		std::string RotationZ;
		std::string Volume;
		std::string SurfaceArea;
	}geometry;
	struct MATERIAL {
		std::string AttachedMaterialConcretePavers;
	}material;
	struct EXTENDED {
		std::string Model;
		std::string ModifiedTime;
		std::string Modified;
		std::string New;
		std::string Locked;
	}externded;
	struct RAWDATA {
		std::string ElementID;
		std::string ElementSize;
		std::string Linkages;
		std::string XAttributes;
	}rawdata;
};

class sqlliteRW
{
public:
	struct obj_struct
	{
		int id;
		std::string hostfile_name;
		std::string elem_guid_infile;
		unsigned int id_infile;
		int guid;
	};
	
public:
	sqlliteRW(WString fileName,std::string path);
	~sqlliteRW();
	bool creatObjTable();
	bool deleteDbFile();

	int openDB();
	void closeDB();
	void insertTbaleElement(const std::string& hostfile_name, const std::string& elem_guid_infile, const std::string& elem_param_tab);
	bool insertObjTbale(const int id_in_element, const std::string & modelN, const std::string& obj_file_Data);
	bool insertMatTable(const int id, const std::string& Mtl_File_Name, const std::string& Material_Data);
	void insertTexTable(const std::string& pic_Name, const std::string& pic_Data);
	void insertTableElementParameter(const int id_in_element, const std::string& param_name, const std::string& param_group, const std::string& param_value, const std::string& unit_type);



	//bool addData(WString STR, int& id);

	void set_id(int& id);
	void set_hostfile_name(WString hostfile_name);
	void set_elem_guid_infile(WString elem_guid_infile);
	void set_id_infile(int id_infile);
	void set_guid(int guid);
	void set_elementID(int id);
	int get_id();
	std::string getMtlFile();
	
	void set_color(const RgbaColorDef& rgba, const std::string& color, const std::string& picIn);
	void addDbElementParameterOne2(const std::string& name, const std::string& group, const std::string& value, const std::string& type);
	void addDbElementParameter2(PROPERTRY& pros);
	std::string checkMtl(const RgbaColorDef& rgba, const std::string& picIn);
	std::string rgbaToStr(const RgbaColorDef& rgba, std::string& ka, std::string& kd, std::string& d);

private:
	obj_struct obj;
	std::string fileName; 
	int objNum;
	RgbaColorDef m_rgba;
	std::string colorName;
	std::string mtlFileName;
	std::string path;
	bool mtlFlag;
	std::string pic;
	//std::string dbF;
	int elementID;
	int ElementNum;
	sqlite3 *db;
};

