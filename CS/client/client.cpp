#include "client.h"

#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
using namespace boost::asio;



CLIENTDLL_EXPORTS void messageS(const char* send,Fun pFun, const char* ip, const char* port)
{
	char writeStr[MaxLen] = "\0";
	strcpy_s(writeStr, send);

	char readStr[MaxLen] = "\0";
	try
	{
		io_service io;
		ip::tcp::endpoint ep(ip::address::from_string(ip), atoi(port));
		ip::tcp::socket sock(io);
		sock.connect(ep);

		sock.write_some(buffer(writeStr));

		sock.read_some(buffer(readStr));
		//std::cout << "receive from" << sock.remote_endpoint().address() << std::endl;
		//std::cout << "receive from server:" << readStr << std::endl;

		pFun(readStr);
	}
	catch (std::exception& e)
	{
		std::cout << "Òì³£:" <<e.what() << std::endl;
		//pFun("Ô¶³ÌÖÐ¶Ï");
		char err[512] = "\0";
		strcpy_s(err, e.what());
		pFun(err);
		Sleep(50);
		messageS(send, pFun, ip, port);
	}

}

