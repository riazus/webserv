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

void Client::setServer(Server &server)
{
	this->_server = server;
}

Server Client::getServer() const
{
	return this->_server;
}

Server* Client::getServerAddr()
{
	return &this->_server;
}
