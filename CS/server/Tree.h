#pragma once
#include <iostream>
#include "../../Polyfacetool/mycpp.h"
using namespace std;

enum MyFlag
{
	newTree = 0,  //新增
	scaTree,      //已扫描模型
	getTree       //已提取数据
};

class DATA
{
public:
	int id;
	std::string pf;
	std::string pm;
	std::string f;
	std::string m;
	std::string refer;
	std::string logical;
	std::string fmd;
	std::string md;
	MyFlag flag;

	DATA() {};
	DATA(const std::string& pf,const std::string& pm,const std::string& f, const std::string& m, const std::string& refer, const std::string& logical, const std::string& fmd,int maxid)
	{
		this->pf = pf; this->pm = pm; this->refer = refer; this->f = f; this->m = m; this->logical = logical; this->fmd = fmd; this->flag = MyFlag::newTree; id = maxid++;
	};
	//DATA(const DATA& d, int maxid)
	//{
	//	pf = d.pf; pm = d.pm; refer = d.refer; f = d.f; m = d.m; logical = d.logical; flag = MyFlag::newTree; id = maxid++;
	//};
	~DATA() {};

	//DATA& operator = (const DATA& d)
	//{
	//	pf = d.pf; pm = d.pm; refer = d.refer; f = d.f; m = d.m; logical = d.logical; id = d.id;
	//}

	bool compareData(const DATA& d)
	{
		if (md == d.md)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
};

typedef struct STreeNode* pSTreeNode;
typedef DATA TreeDataType;

struct STreeNode
{
	TreeDataType data;
	pSTreeNode pFirstChild;
	pSTreeNode pNextBrother;
	pSTreeNode pFather;

	STreeNode(TreeDataType Value)
	{
		data = Value;
		pFirstChild = NULL;
		pNextBrother = NULL;
		pFather = NULL;
	}
};


class CTree
{
public:
	CTree();
	CTree(TreeDataType Value);
	~CTree();
public:
	pSTreeNode Insert(TreeDataType parentValue, TreeDataType Value);    //  parentValue：该节点的父结点；Value：该节点的值
	pSTreeNode InsertBrother(pSTreeNode pParentNode, TreeDataType Value);

	pSTreeNode getOneSca(pSTreeNode pNode);
	pSTreeNode getOneNew(pSTreeNode pNode);

	pSTreeNode Search(pSTreeNode pNode, TreeDataType Value);

	pSTreeNode SearchParent(pSTreeNode pNode, TreeDataType Value);

	std::string getSumTransform(pSTreeNode pNode);

	std::string setParentPathToMD5(pSTreeNode pNode);

	void Preorder(pSTreeNode pNode);      //  前序遍历
	void Inorder(pSTreeNode pNode);       //  中序遍历
	void postorder(pSTreeNode pNode);     //  后续遍历

	void FreeMemory(pSTreeNode pNode);    //  释放内存

public:
	pSTreeNode pRoot;
	int maxid;
};

