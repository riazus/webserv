#include "Server.hpp"

Server::Server()
{
}

Server::Server(const Server &server)
{
	*this = server;
}

Server &Server::operator=(const Server &server)
{
	this->name = server.name;
	return (*this);
}

bool isSpace(unsigned char c) {
	return ( c == '\n' || c == '\r' ||
		c == '\t' || c == '\v' || c == '\f');
}

void Server::setServerName(std::string name)
{
	this->name = name;
}

std::string Server::getServerName()
{
	return (this->name);
}

void Server::setPort(int port)
{
	this->port = port;
}

int Server::getPort()
{
	return (this->port);
}

void Server::setRoot(std::string root)
{
	this->root = root;
}

std::string Server::getRoot()
{
	return (this->root);
}

void Server::setIndex(std::string index)
{
	this->index = index;
}

std::string Server::getIndex()
{
	return (this->index);
}

void Server::setMethods(std::string methods)
{
	this->methods.push_back(methods);
}

std::vector<std::string> Server::getMethods()
{
	return (this->methods);
}

void Server::setLocation(Location location)
{
	this->location.push_back(location);
}

std::vector<Location> Server::getLocation()
{
	return (this->location);
}

void Server::setErrorPage(int num, std::string page)
{
	this->error_page[num] = page;
}

std::map<int, std::string> Server::getErrorPage()
{
	return (this->error_page);
}

void Server::setCgi(std::string name, std::string path)
{
	this->cgi[name] = path;
}

std::map<std::string, std::string> Server::getCgi()
{
	return (this->cgi);
}

void Server::setMaxClientBodySize(long long size)
{
	this->max_client_body_size = size;
}

void Server::setHostAddr(in_addr_t addr)
{
	this->host.s_addr = addr;
}

void Server::setHostName(std::string name)
{
	this->hostName = name;
}

long long Server::getMaxClientBodySize()
{
	return (this->max_client_body_size);
}

std::string Server::getHostName()
{
    return this->hostName;
}

Server::~Server()
{
}