#include "Server.hpp"

Server::Server()
{
}

Server::Server(const std::string name)
{
	this->name = name;
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

std::string Server::myGetLine(std::string *buffer)
{
	std::string ret;
	

	return ret; 
}

int Server::getServerInfo(std::string configFile)
{
	std::string line, key;
	this->buffer = configFile;
	int size;
	const std::string instructions[] = {
		"server_name",
		"listen",
		"root",
		"index",
		"allow_methods",
		"location",
		"}",
		""
	};

	// if (myGetLine() != "server {")
	// 	throw Server::ConfigFileContentException();
	this->braceCounter = 1;

	size = buffer.find("\n") + 1;
	line = buffer.substr(0, size);
	buffer.erase(0, size);
	if (line != "server {\n")
		throw Server::ConfigFileContentException();
	while (line.size() > 0)
	{
		size = buffer.find("\n") + 1;
		line = buffer.substr(0, size);
		// if (line.size() > 1 && line.find(" "))
		// {
		// 	// if(!line.find(" "))
		// 	// {
		// 	// 	//check instructions
		// 	// }
		// 	// else
		// 		line.erase(line.find(" "), line.size() - line.find(" ") - 1);
		// }
		buffer.erase(0, size);
		std::cout << "LINE = '" << line << "'";
	}
	return 0;
}

void Server::setName(std::string name)
{
	this->name = name;
	return ;
}

std::string Server::getName()
{
	return (this->name);
}

const char* Server::ConfigFileContentException::what() const throw()
{
	return "ConfigFileContentException : Invalid configuration file content!";
}

Server::~Server()
{
}