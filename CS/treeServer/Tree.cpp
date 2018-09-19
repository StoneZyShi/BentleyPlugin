#include "Tree.h"


CTree::CTree()
{
	pRoot = NULL;
}

CTree::CTree(TreeDataType Value)
{
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

void CTree::Insert(TreeDataType parentValue, TreeDataType Value)
{
	if (pRoot == NULL)
		return;

	pSTreeNode pFindNode = Search(pRoot, parentValue);
	if (pFindNode == NULL)
		return;

	if (pFindNode->pFirstChild == NULL)
	{
		pFindNode->pFirstChild = new STreeNode(Value);
		return;
	}
	else
	{
		InsertBrother(pFindNode->pFirstChild, Value);
		return;
	}
}

void CTree::InsertBrother(pSTreeNode pBrotherNode, TreeDataType Value)
{
	if (pBrotherNode->pNextBrother != NULL)
		InsertBrother(pBrotherNode->pNextBrother, Value);
	else
	{
		pBrotherNode->pNextBrother = new STreeNode(Value);
		return;
	}

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

void CTree::Preorder(pSTreeNode pNode)
{
	if (pNode == NULL)
		return;
	cout << " " << pNode->data.m.c_str() << " ";

	Preorder(pNode->pFirstChild);
	Preorder(pNode->pNextBrother);
}

void CTree::Inorder(pSTreeNode pNode)
{
	if (pNode == NULL)
		return;

	Inorder(pNode->pFirstChild);
	cout << " " << pNode->data.m.c_str() << " ";
	Inorder(pNode->pNextBrother);
}

void CTree::postorder(pSTreeNode pNode)
{
	if (pNode == NULL)
		return;

	postorder(pNode->pFirstChild);
	postorder(pNode->pNextBrother);
	cout << " " << pNode->data.m.c_str() << " ";
}

