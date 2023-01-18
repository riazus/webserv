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

        std::string name;
        std::string buffer;
        std::vector<int> listen;
        std::string root;
        std::string index;
        std::vector<std::string> methods;
        std::vector<Location> location;

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


// void Server::keyCheck(std::string key, std::string line)
// {
// 	const std::string instructions[] = {
// 		"server_name",
// 		"listen",
// 		"root",
// 		"index",
// 		"allow_methods",
// 		"location",
// 		"}",
// 		""
// 	};

// 	if (key == instructions[0])
// 	{
// 		setServerName(key);
// 	}
// 	if (key == instructions[1])
// 	{
// 		setListen(key);
// 	}
// }

// int Server::getServerInfo(std::string configFile)
// {
// 	std::string line, key;
// 	this->buffer = configFile;
// 	int size;

// 	this->braceCounter = 1;
// 	size = buffer.find("\n") + 1;
// 	line = buffer.substr(0, size);
// 	buffer.erase(0, size);
// 	if (line != "server {\n")
// 		throw Server::ConfigFileContentException();
// 	line = " ";
// 	while (line.size() > 0)
// 	{
// 		size = buffer.find("\n") + 1;
// 		line = buffer.substr(0, size);
// 		key = line;
// 		key.erase(std::remove_if(key.begin(), key.end(), isSpace), key.end());
// 		if (key.size() > 2)
// 		{
// 			key.erase(key.find(" "), key.size() - key.find(" "));
// 			keyCheck(key, line);
// 		}
// 		buffer.erase(0, size);
// 		// std::cout << "LINE = '" << line << "'";
// 		std::cout << "KEY = '" << key << "'" << std::endl;

// 	}
// 	return 0;
// }


const char* Server::ConfigFileContentException::what() const throw()
{
	return "ConfigFileContentException : Invalid configuration file content!";
}

Server::~Server()
{
}