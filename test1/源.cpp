
#include <iostream>
#include <string>
#include <Windows.h>
#include <thread>


using namespace std;

void main()
{
	std::string str = "1234.661,464.189654146156,91253.4000";
	std::string x, y, z;
	x = str.substr(0, str.find(","));
	str = str.substr(str.find(",") + 1, str.length() - str.find(",") - 1);
	y = str.substr(0, str.find(","));
	z = str.substr(str.find(",")+1, str.length());
	cout << "x:" << atof(x.c_str()) << " y:" << atof(y.c_str()) << " z:" << atof(z.c_str()) << endl;

	while (1) {};
}




