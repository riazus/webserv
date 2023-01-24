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

        /*std::string name;
        std::string buffer;
        std::vector<int> listen;
        std::string root;
        std::string index;
        std::vector<std::string> methods;
        std::vector<Location> location;*/

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
	this->listen.push_back(port);
}

std::vector<int> Server::getPort()
{
	return (this->listen);
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

long long Server::getMaxClientBodySize()
{
	return (this->max_client_body_size);
}

Server::~Server()
{
}