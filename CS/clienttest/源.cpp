
#include <iostream>
#include <string>
#include <Windows.h>
#include "../client/client.h"
#include "../../Polyfacetool/dllInterface.h"

using namespace std;

void main()
{
	dllInterfacemyjsonInterface inter;
	inter.Init();

	//inter.c->Add("pf", "f0");
	//inter.c->Add("pm", "m0");
	//inter.c->Add("f", "f1");
	//inter.c->Add("m", "m1");
	//inter.c->Add("refer", "refer1");
	//inter.c->Add("logical", "logical1");

	//inter.c->Add("pf", "f1");
	//inter.c->Add("pm", "m1");
	//inter.c->Add("f", "f2");
	//inter.c->Add("m", "m2");
	//inter.c->Add("refer", "refer2");
	//inter.c->Add("logical", "logical2");

	//inter.c->Add("pf", "f2");
	//inter.c->Add("pm", "m2");
	//inter.c->Add("f", "f3");
	//inter.c->Add("m", "m3");
	//inter.c->Add("refer", "refer3");
	//inter.c->Add("logical", "logical3");

	inter.c->Add("pf", "f3");
	inter.c->Add("pm", "m3");
	inter.c->Add("f", "f4");
	inter.c->Add("m", "m4");
	inter.c->Add("refer", "refer4");
	inter.c->Add("logical", "logical4");

	//inter.c->Add("pf", "f4");
	//inter.c->Add("pm", "m4");
	//inter.c->Add("f", "f5");
	//inter.c->Add("m", "m5");
	//inter.c->Add("refer", "refer5");
	//inter.c->Add("logical", "logical5");

	//std::string str = "NewTree:" + inter.c->ToString();
	//std::string str = "ScaTree:" + inter.c->ToString();
	//std::string str = "GetTree:" + inter.c->ToString();
	std::string str = "ShowAll:" + inter.c->ToString();

	treeClient t;
	std::string node = "";
	int i = 0;
	while (node != "Œﬁ")
	{
		//t.sendMeg("≤‚ ‘:", "192.168.1.100", "6688");
		t.sendMeg("≤‚ ‘:", "127.0.0.1", "6688");
		node = t.getRead();
		if (node == "ø’")
		{
			std::cout << node << ": "<<i++<< endl;
			Sleep(5);
			continue;
		}
		else if(node.find("‘∂≥Ã"))
		{
			std::cout << "yuancheng" << endl;
		}

	}

	//std::string r = "";
	//while (r != "Œﬁ")
	//{
	//	char c;
	//	c = getchar();
	//	if (c == 's')
	//	{
	//		std::string str = "ShowAll:" + inter.c->ToString();
	//		treeClient t;
	//		t.sendMeg("ShowAll:","127.0.0.1","6688");
	//	}
	//	else if (c == 'g')
	//	{
	//		treeClient t;
	//		t.sendMeg("GetTree:", "127.0.0.1", "6688");
	//		r = t.getRead();
	//	}
	//	else if (c == 'o')
	//	{
	//		break;
	//	}
	//}

	system("pause");
}




