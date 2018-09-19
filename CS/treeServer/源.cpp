#include <iostream>
using namespace std;

#include "Tree.h"

int main()
{
	DATA d1("pf1", "pm1", "refer1", "f1", "m1");
	DATA d2("pf2", "pm2", "refer2", "f2", "m2");
	DATA d3("pf3", "pm3", "refer3", "f3", "m3");
	DATA d4("pf4", "pm4", "refer4", "f4", "m4");
	DATA d5("pf5", "pm5", "refer5", "f5", "m5");
	DATA d6("pf6", "pm6", "refer6", "f6", "m6");
	DATA d7("pf7", "pm7", "refer7", "f7", "m7");
	DATA d8("pf8", "pm8", "refer8", "f8", "m8");
	DATA d9("pf9", "pm9", "refer9", "f9", "m9");
	DATA d10("pf10", "pm10", "refer10", "f10", "m10");
	DATA d11("pf11", "pm11", "refer11", "f11", "m11");
	DATA d12("pf12", "pm12", "refer12", "f12", "m12");
	DATA d13("pf13", "pm13", "refer13", "f13", "m13");
	DATA d14("pf14", "pm14", "refer14", "f14", "m14");
	DATA d15("pf15", "pm15", "refer15", "f15", "m15");

	CTree* pTree = new CTree(d1);
	if (pTree == NULL)
		return 0;

	pTree->Insert(d1, d2);
	pTree->Insert(d1, d3);
	pTree->Insert(d1, d4);
	pTree->Insert(d1, d5);
	pTree->Insert(d1, d6);
	pTree->Insert(d1, d7);
	pTree->Insert(d4,d8);
	pTree->Insert(d5, d9);
	pTree->Insert(d5, d10);
	pTree->Insert(d6, d11);
	pTree->Insert(d6, d12);
	pTree->Insert(d6, d13);
	pTree->Insert(d10, d14);
	pTree->Insert(d10, d15);

	cout << "前序遍历：" << endl;
	pTree->Preorder(pTree->pRoot);
	cout << endl;

	cout << "中序遍历：" << endl;
	pTree->Inorder(pTree->pRoot);
	cout << endl;

	cout << "后序遍历：" << endl;
	pTree->postorder(pTree->pRoot);
	cout << endl;

	delete pTree;
	pTree = NULL;
	system("pause");
	return 1;
}

