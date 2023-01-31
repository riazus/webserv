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

void Server::setLocation(Location *location)
{
	this->location.push_back(location);
}

std::list<Location *> Server::getLocation()
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
void Server::server_error(std::string error)
{
	if (error != "")
		std::cout << "Error: " << error << std::endl;
	throw Server::InvalidServerException();
}

void Server::is_valid()
{
	if (getPort() == 0)
		server_error("port is not set");
	if (getServerName() == "")
		server_error("server name is not set");
	// if (get_ip_address() == "")
	// 	server_error("ip_address is not set");
	if (getRoot() == "")
		server_error("root is not set");
	if (getIndex() == "")
		server_error("index is not set");
	// if (getMaxClientBodySize() == 0)
	// 	server_error("max_client_body_size is not set");

	int fd;
	std::string error;
	// std::map<std::string, std::string> cgis = getCgi();
	// for (std::map<std::string, std::string>::iterator cgi = cgis.begin(); cgi != cgis.end(); cgi++)
	// {
	// 	fd = ::open(cgi->second.c_str(), O_RDONLY);
	// 	if (fd <= 0)
	// 	{
	// 		error = "cgi " + cgi->second + " is unavailable";
	// 		server_error(error);
	// 	}
	// 	close(fd);
	// }
	std::map<int, std::string> error_pages = getErrorPage();
	for (std::map<int, std::string>::iterator page = error_pages.begin(); page != error_pages.end(); page++)
	{
		fd = ::open((getRoot() + page->second).c_str(), O_RDONLY);
		if (fd <= 0)
		{
			error = "error_page " + getRoot() + page->second + " is unavailable";
			server_error(error);
		}
		close(fd);
	}
	std::list<Location *> locations = getLocation();
	if (!locations.size())
		server_error("location is not set");
	for (std::list<Location*>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		(*it)->is_valid();
	}
}

const char* Server::InvalidServerException::what() const throw()
{
	return "InvalidServerException: Invalid server configuration!";
}

Server::~Server()
{
}