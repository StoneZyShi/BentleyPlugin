#include "mycpp.h"


std::string nothing(char * file, int line, char * fun)
{
	char buf[512] = "\0";
	sprintf_s(buf, "file:%s--line=%d--fun:%s()-->\n", file, line, fun);
	return buf;
};

void strToTransform(std::string str, double myTransform[4][4])
{
	if (str != "")
	{
		str = str.substr(1, str.length()-1);
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

std::string transformToStr(double myTransform[4][4])
{
	std::string str = "";
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			str += std::to_string(myTransform[i][j]);
			str += ",";
		}
	}
	return str;
}

void toTransform(double transformA[4][4], double transformB[4][4])
{
		double newTransform[4][4] = { 1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
		double matrix_result[4][4] = { 0 };
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				newTransform[i][j] = transformB[i][j];
			}
			newTransform[i][3] = transformB[i][3];
			newTransform[3][i] = 0;
		}
		newTransform[3][3] = 1;
		
		for (int m = 0; m < 4; m++) {
			for (int s = 0; s < 4; s++) {
				matrix_result[m][s] = 0;//变量使用前记得初始化,否则结果具有不确定性
				for (int n = 0; n < 4; n++) {
					matrix_result[m][s] += transformA[m][n] * newTransform[n][s];
				}
			}
		}
		for (int m = 0; m < 4; m++) {
			for (int s = 0; s < 4; s++) {
				transformA[m][s] = matrix_result[m][s];
			}
		}
}

std::string funMsg(char* file,int line,char* fun)
{
	char buf[512] = "\0";
	sprintf_s(buf, "file:%s--line=%d--fun:%s()-->\n", file, line, fun);
	return std::string(buf);
}

