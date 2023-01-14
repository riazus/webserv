#include "Client.hpp"

Client::Client(void)
{
}

Client::Client(Client const & src)
{
	*this = src;
}

Client &Client::operator=(Client const &rhs)
{
	return *this;
}

Client::~Client(void)
{
}

void Client::SetSocket(int socket)
{
	this->_socket = socket;
}