#pragma once

#include <string>
#include "utility.h"

class BScanfRefers
{
public:
	BScanfRefers();
	~BScanfRefers();

	std::string shifting(const DgnModelRefP& modelRef, double myTransform[4][4]);
	void scanf(std::string & md);
	void sendToServer(std::string& PFile, std::string& PModel, std::string& File, std::string& Model, std::string& logical, std::string& refer, std::string & md);
	
	void newOne(std::string& File, std::string& Model, std::string & md);

	std::string ip = "127.0.0.1";
	std::string port = "6688";
};

