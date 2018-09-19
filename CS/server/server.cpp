#include "server.h"
#include <iostream>                                       
#include <fstream>
#include <io.h>
#include "../../Polyfacetool/dllInterface.h"
#include "../../Polyfacetool/MD5.h"

using namespace std;

server::server()
{
	tree = nullptr;
	ScaOver = false;
}


server::~server()
{
	if (tree != nullptr)
	{
		delete tree;
		tree = nullptr;
	}
}


std::string server::analyze(std::string & str, std::string & ip)
{
	//Sleep(1000);
	std::string rtn = "©у";
	if (str.find("NewNode:") != std::string::npos)
	{
		std::string nodeRootPath = str.substr(8, str.length() - 8);
		rootPaths[ip.c_str()] = nodeRootPath;

		rtn = "NewNode:OK";
	}
	else if (str.find("GetPath:") != std::string::npos)
	{
		rtn = rootPaths[ip.c_str()];
	}
	else if (str.find("NewTree:") != std::string::npos)
	{
		if (newTree(str,ip))
			rtn = "NewTree:OK";
		else
			rtn = "NewTree:No father tree";
	}
	else if (str.find("ScaTree:") != std::string::npos && tree != nullptr)
	{
		rtn = scaTree();
	}
	else if (str.find("ScaOver:") != std::string::npos && tree != nullptr)
	{
		ScaOver = true;
		rtn = "ScaOver:OK";
	}
	else if (str.find("GetTree:") != std::string::npos && tree != nullptr)
	{
		if (ScaOver == true)
		{
			rtn = getTree();
		}
		else
		{
			rtn = "Scanf is not ok,please wait";
		}
	}
	else if (str.find("ShowAll:") != std::string::npos && tree != nullptr)
	{
		tree->Preorder(tree->pRoot);
	}
	else if (str.find("SetPrec:") != std::string::npos)
	{
		precision = str.substr(8, str.length() - 8);
		rtn = "SetPrec:OK";
	}
	else if (str.find("GetPrec:") != std::string::npos)
	{
		rtn = precision;
	}
	else if (str.find("SetCent:") != std::string::npos)
	{
		centre = str.substr(8, str.length() - 8);
		rtn = "SetCent:OK";
	}
	else if (str.find("GetCent:") != std::string::npos)
	{
		rtn = centre;
	}
	return rtn;
}

void server::treeFile(std::string & md, int & id, const std::string & ip)
{
	if ((_access((rootPaths[ip.c_str()] + TreeFile).c_str(), 0)) != -1)
	{
		std::string buffer;
		ifstream in(rootPaths[ip.c_str()] + TreeFile);
		bool flag = false;
		int maxId = 0;
		if (!in.is_open())
		{
			cout << "Error opening file";
			return;
		}
		else
		{
			while (!in.eof())
			{
				getline(in, buffer);

				if (buffer.length() >= 34)
				{
					int bID = atoi(buffer.substr(33, buffer.length() - 33).c_str());
					maxId = maxId >= bID ? maxId : bID;

					if (buffer.find(md) != std::string::npos)
					{
						id = bID;
						flag = true;
					}
				}
			}
			if (flag == false)
			{
				id = maxId >= id ? maxId+1 : id;
				ofstream out(rootPaths[ip.c_str()] + TreeFile, ios::app);
				if (out.is_open())
				{
					out << md << " " << id << endl;
					out.close();
				}
			}
		}
	}
	else
	{
		ofstream out(rootPaths[ip.c_str()] + TreeFile, ios::app);
		if (out.is_open())
		{
			out << md << " " << id << endl;
			out.close();
		}
	}
}

bool server::newTree(const std::string & str, const std::string & ip)
{
	std::string jsonStr = str.substr(8, str.length() - 8);

	dllInterfacemyjsonInterface j;
	j.Init();
	j.c->InitJson(jsonStr);

	std::string oneStr = "";
	DATA d1(j.c->Get("pf",oneStr), j.c->Get("pm", oneStr), j.c->Get("f", oneStr), j.c->Get("m", oneStr), j.c->Get("refer", oneStr),j.c->Get("logical",oneStr),j.c->Get("fmd",oneStr),0);

	cout << d1.pf << "  " << d1.pm << "  " << d1.logical<<"  " << d1.f << "  " << d1.m << "  " << d1.refer << endl;
		
	if (tree == nullptr)
	{
		tree = new CTree(d1);
		if (tree == NULL)
			return false;

		tree->pRoot->data.id = tree->maxid;

		tree->pRoot->data.md = tree->setParentPathToMD5(tree->pRoot);
		treeFile(tree->pRoot->data.md, tree->pRoot->data.id,ip);
	}
	else
	{
		pSTreeNode pp = tree->SearchParent(tree->pRoot, d1);
		if (pp == nullptr)
			return false;

		tree->maxid++;
		d1.id = tree->maxid;
		pSTreeNode p = tree->Insert(pp->data, d1);

		p->data.md = tree->setParentPathToMD5(p); 
		treeFile(p->data.md, p->data.id,ip);
	}
	
	return true;
}

std::string server::scaTree()
{
	if (tree != nullptr && tree->pRoot != nullptr)
	{
		pSTreeNode node = tree->getOneSca(tree->pRoot);
		if (node != nullptr)
		{
			TreeDataType data(node->data);

			dllInterfacemyjsonInterface inter;
			inter.Init();

			inter.c->Add("pf", data.pf);
			inter.c->Add("pm", data.pm);
			inter.c->Add("refer", data.refer);
			inter.c->Add("logical", data.logical);
			inter.c->Add("f", data.f);
			inter.c->Add("m", data.m);
			inter.c->Add("id", std::to_string(data.id));
			inter.c->Add("md", data.md);

			return inter.c->ToString();
		}
	}
	return std::string("нч");
}

std::string server::getTree()
{
	if (tree != nullptr && tree->pRoot != nullptr)
	{
		pSTreeNode node = tree->getOneNew(tree->pRoot);
		if (node != nullptr)
		{
			TreeDataType data(node->data);

			dllInterfacemyjsonInterface inter;
			inter.Init();
			inter.c->clear();

			inter.c->Add("pf", data.pf);
			inter.c->Add("pm", data.pm);
			cout << data.f << "  " << data.m <<"  "<<data.logical << endl;
			std::string sum = tree->getSumTransform(node);
			inter.c->Add("refer",sum);
			inter.c->Add("logical", data.logical);
			inter.c->Add("f", data.f);
			inter.c->Add("m", data.m);
			inter.c->Add("id", std::to_string(data.id));
			inter.c->Add("md", data.md);

			return inter.c->ToString();
		}
	}
	return std::string("нч");
}


