#include "Server.hpp"

Server::Server(): name(""), port(8080), ip_adress(""), hostName(""), root(""), index(""), max_client_body_size(0)
{
}

Server::Server(const Server &server)
{
	*this = server;
}

Server &Server::operator=(const Server &server)
{

	this->name = server.name;
	this->port = server.port;
	this->ip_adress = server.ip_adress;
	this->host = server.host;
	this->hostName = server.hostName;
	this->root = server.root;
	this->index = server.index;
	this->methods = server.methods;
	this->error_page = server.error_page;
	this->cgi = server.cgi;
	this->location = server.location;
	this->max_client_body_size = server.max_client_body_size;
	return (*this);
}

bool Server::operator==(const Server & rhs)
{
	std::string firstIp(inet_ntoa(this->host));
	std::string secondIp(inet_ntoa(rhs.host));

	return ((firstIp == secondIp || (this->host.s_addr == 0 || rhs.host.s_addr == 0)) && (this->port == rhs.port));
}

void Server::setServerName(std::string name)
{
	this->name = name;
}

std::string &Server::getServerName()
{
	return (this->name);
}

void Server::setPort(int port)
{
	this->port = port;
}

int &Server::getPort()
{
	return (this->port);
}

void Server::setIpAddress(std::string ip_adress)
{
	this->ip_adress = ip_adress;
}

void Server::setAutoindex(bool var)
{
	this->autoindex = var;
}

std::string &Server::getIpAdress()
{
	return (this->ip_adress);
}

void Server::setRoot(std::string root)
{
	this->root = root;
}

std::string &Server::getRoot()
{
	return (this->root);
}

void Server::setIndex(std::string index)
{
	this->index = index;
}

std::string &Server::getIndex()
{
	return (this->index);
}

void Server::setMethods(std::string methods)
{
	this->methods.push_back(methods);
}

std::vector<std::string> &Server::getMethods()
{
	return (this->methods);
}

void Server::setLocation(Location location)
{
	this->location.push_back(location);
}

std::list<Location> &Server::getLocations()
{
	return this->location;
}

void Server::setErrorPage(int num, std::string page)
{
	this->error_page[num] = page;
}

mapError &Server::getErrorPage()
{
	return (this->error_page);
}

void Server::setCgi(std::string name, std::string path)
{
	this->cgi[name] = path;
}

std::map<std::string, std::string> &Server::getCgi()
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

long long &Server::getMaxClientBodySize()
{
	return (this->max_client_body_size);
}

std::string &Server::getHostName()
{
    return this->hostName;
}
void Server::server_error(std::string error)
{
	if (error != "")
		std::cout << "Error: " << error << std::endl;
	throw Server::InvalidServerException();
}

in_addr_t Server::getHostAddr()
{
	return inet_addr(getIpAdress().c_str());
}

bool &Server::getAutoindex()
{
    return this->autoindex;
}

std::string Server::getAlias()
{
    return "";
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
	/*std::list<Location *> locations = getLocation();
	if (!locations.size())
		server_error("location is not set");
	for (std::list<Location*>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		(*it)->is_valid();
	}*/
}


void Server::parse_server(std::vector<std::string> config, int *line_count)
{
	std::vector<std::string> line = ft_split(config[0], CHARTOSKIP);

	if (line[1] != "{")
		config_error("expected '{' after server");
	//Server server;
	std::vector<std::string>::const_iterator it = config.begin() + *line_count;

	if (it == config.end())
		config_error("");
	it++;
	while (it != config.end())
	{
		std::vector<std::string> line = ft_split(*it, CHARTOSKIP);
		// std::cout << *line_count << "  " << line[0] << std::endl;
		if (!line.size() || !line[0].size())
		{
			it++;
			(*line_count)++;
			continue;
		}
		if (line[0][0] == '#')
		{
			it++;
			(*line_count)++;
			continue;
		}
		if (line[0] == "}")
			break;
		else if (line[0] == "location")
		{
			if (line.size() != 3)
						config_error("expected 2 arguments after location");
			int tmp_count = *line_count;
			Location location;
			this->setLocation(location.parse_location(config, line_count));
			it += *line_count - tmp_count - 1;
		}
		else if (line[0] == "server_name")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after server_name");
			this->setServerName(line[1]);
		}
		else if (line[0] == "error_page")
		{
			if (line.size() != 3)
				config_error("expected 2 arguments after error_page");
			this->setErrorPage(atoi(line[1].c_str()), line[2]);
		}
		else if (line[0] == "listen")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after listen");
			std::vector<std::string> listen = ft_split(line[1], ":");
			if (listen.size() != 1 && listen.size() != 2)
				config_error("expected port or address:port after listen");
			std::string address;
			std::string port;
			address = listen[0];
			if (listen.size() == 1)
			{
				address = "0.0.0.0";
				port = listen[0];
			}
			else
				port = listen[1];
			if (address == "localhost")
				address = "127.0.0.1";
			this->setIpAddress(address);
			for (int i=0; i < port.size(); i++)
			{
				if (!isdigit(port[i]) || i > 4)
									config_error("expected integer after listen");
			}
			this->setPort(atoi(port.c_str()));
		}
		else if (line[0] == "cgi")
		{
			if (line.size() != 3)
				config_error("expected 2 arguments after cgi");
			this->setCgi(line[1], line[2]);
		}
		else if (line[0] == "root")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after root");
			this->setRoot(line[1]);
		}
		else if (line[0] == "max_client_body_size")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after max_client_body_size");
			if (!is_integer(line[1]))
				config_error("expected integer after max_client_body_size");
			this->setMaxClientBodySize(atoll(line[1].c_str()));
		}
		else if (line[0] == "index")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after index");
			this->setIndex(line[1]);
		}
		else if (line[0] == "allow_methods")
		{
			if (line.size() != 2)
				config_error("expected min 1 argument allow methods");
			for(int i = 1; i < line.size(); i++){
				this->setMethods(line[i]);
			}
		}
		else if (line[0] == "autoindex")
		{
			if (line.size() != 2)
				config_error("expected min 2 argument in autoindex");
			if (line[1] != "1" && line[1] != "0")
				config_error("expected bool var in autoindex");
			this->setAutoindex(line[1] == "1"? true : false);
		}
		it++;
		(*line_count)++;
	}
	std::vector<std::string> end = ft_split(*it, CHARTOSKIP);
	while (!end.size() || end[0] != "}")
	{
		(*line_count)++;
		end = ft_split(config[*line_count], CHARTOSKIP);
	}
	if (end.size() && end[0] != "}")
		config_error("expected '}'");
	//this->is_valid();
}

const char* Server::InvalidServerException::what() const throw()
{
	return "InvalidServerException: Invalid server configuration!";
}

Server::~Server()
{
}