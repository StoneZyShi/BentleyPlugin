#pragma once
#include <string>
#include <map>
#include "Tree.h"

#define TreeFile "tree.mtl"

class server
{
public:
	server();
	~server();

	std::string analyze(std::string& str, std::string & ip);

	void treeFile(std::string& md,int& id, const std::string & ip);
	bool newTree(const std::string& str, const std::string & ip);
	std::string scaTree();
	std::string getTree();

private:
	std::string msg;
	CTree* tree;

	std::map<std::string, std::string> rootPaths;
	int uorPerMeter;
	std::string startTime;
	std::string precision;
	std::string centre;
	bool ScaOver;
};

