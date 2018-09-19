#pragma once
#include <iostream>
using namespace std;

enum MyFlag
{
	newTree = 0,
	getTree
};

class DATA
{
public:
	std::string pf;
	std::string pm;
	std::string refer;
	std::string f;
	std::string m;
	MyFlag flag;

	DATA() {};
	DATA(const std::string& pf,const std::string& pm,const std::string& refer,const std::string& f,const std::string& m)
	{
		this->pf = pf;this->pm = pm;this->refer = refer;this->f = f;this->m = m;
	};
	DATA(const DATA& d)
	{
		pf = d.pf;pm = d.pm;refer = d.refer;f = d.f;m = d.m;
	};
	~DATA() {};

	DATA& operator = (const DATA& d)
	{
		pf = d.pf; pm = d.pm; refer = d.refer; f = d.f; m = d.m;
	}

	bool compareData(const DATA& d)
	{
		if (pf == d.pf&&pm == d.pm&&refer == d.refer&&f == d.f&&m == d.m)
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

	STreeNode(TreeDataType Value)
	{
		data = Value;
		pFirstChild = NULL;
		pNextBrother = NULL;
	}
};


class CTree
{
public:
	CTree();
	CTree(TreeDataType Value);
	~CTree();


public:
	void Insert(TreeDataType parentValue, TreeDataType Value);    //  parentValue���ýڵ�ĸ���㣻Value���ýڵ��ֵ
	void InsertBrother(pSTreeNode pParentNode, TreeDataType Value);

	pSTreeNode Search(pSTreeNode pNode, TreeDataType Value);

	void Preorder(pSTreeNode pNode);      //  ǰ�����
	void Inorder(pSTreeNode pNode);       //  �������
	void postorder(pSTreeNode pNode);     //  ��������

	void FreeMemory(pSTreeNode pNode);    //  �ͷ��ڴ�

public:
	pSTreeNode pRoot;
};

