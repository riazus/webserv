#include "Client.hpp"

Client::Client(void) : hadResponse(false), UserId("")
{
	gettimeofday(&this->lastRequest, NULL);
}

Client::Client(Client const & src)
{
	*this = src;
}

Client &Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
		this->request = rhs.request;
		this->lastRequest = rhs.lastRequest;
		this->hadResponse = rhs.hadResponse;
		this->_socket = rhs._socket;
		this->_server = rhs._server;
		this->UserId = rhs.UserId;
	}
	return *this;
}

Client::~Client(void)
{
}

void Client::SetSocket(int socket)
{
	this->_socket = socket;
}

void Client::setServer(Server server)
{
	this->_server = server;
}

Server &Client::getServer()
{
	return this->_server;
}

int Client::getSocket()
{
    return this->_socket;
}