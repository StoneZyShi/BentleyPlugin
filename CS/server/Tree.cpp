#include "Tree.h"
#include <vector>
#include <queue>
#include <stack>
#include "../../Polyfacetool/MD5.h"

CTree::CTree()
{
	pRoot = NULL;
	maxid = 1;
}

CTree::CTree(TreeDataType Value)
{
	maxid = 1;
	pRoot = new STreeNode(Value);
	if (pRoot == NULL)
		return;
}

CTree::~CTree()
{ 
	if (pRoot == NULL)
		return;

	FreeMemory(pRoot);
}

void CTree::FreeMemory(pSTreeNode pNode)
{
	if (pNode == NULL)
		return;

	if (pNode->pFirstChild != NULL)
		FreeMemory(pNode->pFirstChild);

	if (pNode->pNextBrother != NULL)
		FreeMemory(pNode->pNextBrother);

	delete pNode;
	pNode = NULL;
}

pSTreeNode CTree::Insert(TreeDataType parentValue, TreeDataType Value)
{
	if (pRoot == NULL)
		return NULL;

	pSTreeNode pFindNode = Search(pRoot, parentValue);
	if (pFindNode == NULL)
		return NULL;

	if (pFindNode->pFirstChild == NULL)
	{
		pFindNode->pFirstChild = new STreeNode(Value);
		pFindNode->pFirstChild->pFather = pFindNode;
		return pFindNode->pFirstChild;
	}
	else
	{		
		return InsertBrother(pFindNode->pFirstChild, Value);
	}
}

pSTreeNode CTree::InsertBrother(pSTreeNode pBrotherNode, TreeDataType Value)
{
	if (pBrotherNode->pNextBrother != NULL)
		return InsertBrother(pBrotherNode->pNextBrother, Value);
	else
	{
		pBrotherNode->pNextBrother = new STreeNode(Value);
		pBrotherNode->pNextBrother->pFather = pBrotherNode->pFather;
		return pBrotherNode->pNextBrother;
	}

}

pSTreeNode CTree::getOneSca(pSTreeNode pNode)
{
	if (pNode != NULL)
	{
		if (pNode->data.flag == MyFlag::newTree)
		{
			pNode->data.flag = MyFlag::scaTree;
			return pNode;
		}
		else
		{
			pSTreeNode f = getOneSca(pNode->pFirstChild);
			if (f != NULL)
			{
				return f;
			}
			pSTreeNode n = getOneSca(pNode->pNextBrother);
			if (n != NULL)
			{
				return n;
			}
		}
	}
	return NULL;
}

pSTreeNode CTree::getOneNew(pSTreeNode pNode)
{
	if (pNode != NULL)
	{
		if (pNode->data.flag == MyFlag::scaTree)
		{
			pNode->data.flag = MyFlag::getTree;
			return pNode;
		}
		else
		{
			//if (pNode->data.f == "0206" && pNode->data.m == "P43" && pNode->data.logical == "²Î¿¼-76")
			//{
			//	cout << pNode->pNextBrother->data.m << endl;
			//}
			pSTreeNode f = getOneNew(pNode->pFirstChild);
			if (f != NULL)
			{
				return f;
			}
			pSTreeNode n = getOneNew(pNode->pNextBrother);
			if (n != NULL)
			{
				return n;
			}
		}
	}
	return NULL;
}

pSTreeNode CTree::Search(pSTreeNode pNode, TreeDataType Value)
{
	if (pNode == NULL)
		return NULL;

	if (pNode->data.compareData(Value))
		return pNode;

	if (pNode->pFirstChild == NULL && pNode->pNextBrother == NULL)
		return NULL;
	else
	{
		if (pNode->pFirstChild != NULL)
		{
			pSTreeNode pNodeTemp = Search(pNode->pFirstChild, Value);
			if (pNodeTemp != NULL)
				return pNodeTemp;
			else
			{
				return Search(pNode->pNextBrother, Value);
			}
		}
		else
			return Search(pNode->pNextBrother, Value);
	}
}

pSTreeNode CTree::SearchParent(pSTreeNode pNode, TreeDataType Value)
{
	if (pNode == NULL)
		return NULL;

	if (pNode->data.md == Value.fmd)
		return pNode;

	if (pNode->pFirstChild == NULL && pNode->pNextBrother == NULL)
		return NULL;
	else
	{
		if (pNode->pFirstChild != NULL)
		{
			pSTreeNode pNodeTemp = SearchParent(pNode->pFirstChild, Value);
			if (pNodeTemp != NULL)
				return pNodeTemp;
			else
			{
				return SearchParent(pNode->pNextBrother, Value);
			}
		}
		else
			return SearchParent(pNode->pNextBrother, Value);
	}
	return NULL;
}


std::string CTree::getSumTransform(pSTreeNode pNode)
{
	try
	{
		std::stack<std::string> sum;
		while (pNode != pRoot)
		{
			std::string s = pNode->data.refer;
			sum.push(s);
			if (pNode->pFather != nullptr)
			{
				pNode = pNode->pFather;
			}
			else
			{
				cout << "pNode->pFather = null";
			}
		}

		double transform[4][4] = { 1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
		double myTransform[4][4] = { 1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
		while (!sum.empty())
		{
			strToTransform(sum.top(), myTransform);
			sum.pop();

			toTransform(transform, myTransform);
		}
		
		//double transform[4][4] = { 1,0,0,0,
		//	0,1,0,0,
		//	0,0,1,0,
		//	0,0,0,1 };
		//while (pNode != pRoot)
		//{
		//	std::string s = pNode->data.refer;
		//	if (pNode->pFather != nullptr)
		//	{
		//		double myTransform[4][4] = { 1,0,0,0,
		//			0,1,0,0,
		//			0,0,1,0,
		//			0,0,0,1 };
		//		strToTransform(s, myTransform);
		//		toTransform(transform, myTransform);

		//		pNode = pNode->pFather;
		//	}
		//	else
		//	{
		//		cout << "pNode->pFather = null";
		//	}
		//}
		std::string rtn = transformToStr(transform);
		cout << "rtn:" << rtn << endl;
		return rtn;
	}
	catch (const std::exception e)
	{
		cout << e.what() << endl;
	}
}

std::string CTree::setParentPathToMD5(pSTreeNode pNode)
{
	std::string rtn = "";
	pSTreeNode p = pNode;
	while (p != pRoot)
	{
		rtn = p->data.logical + p->data.f + p->data.m + rtn;
		p = p->pFather;
	}
	rtn = p->data.f + p->data.m + rtn;
	CMD5 md5;
	rtn = md5.md5(rtn);
	return rtn;
}

void CTree::Preorder(pSTreeNode pNode)
{
	if (pNode == NULL)
		return;
	cout << maxid<< "  " << pNode->data.id
		<< "  " << pNode->data.pf.c_str() << "  " << pNode->data.pm.c_str()
		<< "  " << pNode->data.refer.c_str()
		<< "  " << pNode->data.f.c_str() << "  " << pNode->data.m.c_str()
		<< "  " << pNode->data.flag  << endl;

	Preorder(pNode->pFirstChild);
	Preorder(pNode->pNextBrother);
}

void CTree::Inorder(pSTreeNode pNode)
{
	if (pNode == NULL)
		return;

	Inorder(pNode->pFirstChild);
	cout << "model:" << pNode->data.m.c_str() << "  flag:" << pNode->data.flag << "  id:" << pNode->data.id << endl;
	Inorder(pNode->pNextBrother);
}

void CTree::postorder(pSTreeNode pNode)
{
	if (pNode == NULL)
		return;

	postorder(pNode->pFirstChild);
	postorder(pNode->pNextBrother);
	cout << "model:" << pNode->data.m.c_str() << "  flag:" << pNode->data.flag << "  id:" << pNode->data.id << endl;
}

