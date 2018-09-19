
#include <boost/asio.hpp>
#include <iostream>
#include "server.h"

using namespace boost::asio;
int main()
{
	try
	{
		io_service io;
		ip::tcp::endpoint ep(ip::tcp::v4(), 6688);
		ip::tcp::acceptor acceptor(io, ep);
		server s;
		char str[10240] = "\0";
		while (1)
		{
			ip::tcp::socket sock(io);
			acceptor.accept(sock);
			//std::cout << "client:" << sock.remote_endpoint().address() << std::endl;

			memset(str, '\0', 10240);
			sock.read_some(buffer(str));

			sock.write_some(buffer(s.analyze((std::string)str, sock.remote_endpoint().address().to_string())));
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}

