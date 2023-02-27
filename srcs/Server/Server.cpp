#include "Server.hpp"

Server::Server() : port(0), ip_adress(""), hostName(""), root(""), index(""), max_client_body_size(0)
{
}

Server::Server(const Server &server)
{
	*this = server;
}

Server &Server::operator=(const Server &server)
{

	this->names = server.names;
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
	this->autoindex = server.autoindex;
	this->locationName = server.locationName;
	this->alias = server.alias;
	this->ret = server.ret;
	this->extension = server.extension;
	this->cgiPass = server.cgiPass;

	return (*this);
};

bool Server::operator==(const Server & rhs)
{
	std::string firstIp(inet_ntoa(this->host));
	std::string secondIp(inet_ntoa(rhs.host));

	return ((firstIp == secondIp || (this->host.s_addr == 0 || rhs.host.s_addr == 0)) && (this->port == rhs.port));
}

void Server::setServerName(std::string name)
{
	this->names.push_back(name);
}

stringVector &Server::getServerName()
{
	return (this->names);
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

std::string Server::getRoot() const
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

void Server::setLocation(Server location)
{
	this->location.push_back(location);
}

std::vector<Server> &Server::getLocations()
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

void Server::setAlias(std::string alias)
{
	this->alias = alias;
}

std::string &Server::getAlias()
{
    return this->alias;
}

void Server::setIsExtension(bool val)
{
	this->extension = val;
}

bool &Server::getIsExtension()
{
    return this->extension;
}

void Server::setReturn(std::string code, std::string url)
{
	this->ret = std::make_pair(code, url);
}

void Server::setLocationName(std::string str)
{
	this->locationName = str;
}

std::string Server::getLocationName() const
{
	return (this->locationName);
}

pairString &Server::getReturn()
{
	return (this->ret);
}

void Server::setCgiPass(std::string pass)
{
	this->cgiPass = pass;
}

std::string &Server::getCgiPass()
{
	return (this->cgiPass);
}



void Server::parse_server(std::vector<std::string> config, int *line_count, int locLevel)
{
	if (locLevel > 2)
		config_error("overflow location levels");
	std::vector<std::string>::const_iterator it = config.begin() + *line_count;
	if (locLevel == 1 && location.size() < 1)
	{
		(*line_count)++;
		it++;
	}
	if(locLevel == 1 || locLevel == 2)
	{
		std::vector<std::string> line = ft_split(config[*line_count], CHARTOSKIP);
		// std::cout << "LINE:	" << *line_count << "	" << line[0] << " " << line[1] << std::endl;
		if (line.size() != 3)
			config_error("expected a directory and '{' after location");
			//std::cout << "SET LOCAT NAME: " <<  << std::endl;
		this->setLocationName(line[1]);
		if (line[2] != "{")
			config_error("missing '{'");
		(*line_count)++;
		it++;
	}
	else
	{
		std::vector<std::string> line = ft_split(config[0], CHARTOSKIP);

		if (line[1] != "{")
			config_error("expected '{' after server");
		if (it == config.end())
			config_error("");
		it++;
	}
	while (it != config.end())
	{
		std::vector<std::string> line = ft_split(*it, CHARTOSKIP);
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
				Server location;
				// std::cout << "LINE:	" << *line_count << "	" << line[0] << " " << line[1] << std::endl;
				location.parse_server(config, line_count, locLevel + 1);
				this->setLocation(location);
				it += *line_count - tmp_count - 1;
				if (locLevel < 1)
					(*line_count)--;
		}
		else if (line[0] == "server_name")
		{
			if (locLevel != 0)
				config_error("invalid attribute");
			else 
			{
				if (line.size() != 2)
					config_error("expected 1 argument after server_name");
				for(int i = 1; i < line.size(); i++)
				{
					this->setServerName(line[1]);
				}
			}
		}
		else if (line[0] == "error_page")
		{
			if (line.size() != 3)
				config_error("expected 2 arguments after error_page");
			this->setErrorPage(atoi(line[1].c_str()), line[2]);
		}
		else if (line[0] == "listen")
		{
			if (locLevel != 0)
				config_error("invalid attribute");
			else
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
			if (line.size() < 2)
				config_error("expected min 1 argument after allow_methods");
			for(int i = 1; i < line.size(); i++){
				this->setMethods(line[i]);
			}
		}
		else if (line[0] == "cgi_pass")
		{
			if (line.size() < 2)
				config_error("expected min 1 argument after cgi_param");
			this->setCgiPass(line[1]);
		}
		else if (line[0] == "autoindex")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after autoindex");
			if (line[1] != "1" && line[1] != "0")
				config_error("expected bool var in autoindex");
			this->setAutoindex(line[1] == "1"? true : false);
		}
		else if (line[0] == "alias")
		{
			if (locLevel == 0)
				config_error("invalid attribute");
			if (line.size() != 2)
				config_error("expected 1 argument after alias");
			this->setAlias(line[1]);
		}
		else if (line[0] == "return")
		{
			if (line.size() != 3)
				config_error("expected 2 arguments after return");
			this->setReturn(line[1], line[2]);
		}
		it++;
		(*line_count)++;
	}
	this->checkForDefaultFields();
	std::vector<std::string> end = ft_split(*it, CHARTOSKIP);
	while (!end.size() || end[0] != "}")
	{
		(*line_count)++;
		end = ft_split(config[*line_count], CHARTOSKIP);
	}
	if (end.size() && end[0] != "}")
		config_error("expected '}'");
}


void Server::checkForDefaultFields()
{
	std::string content = getFile(DEFAULT);
	stringVector lines = ft_split(content, "\n");
	for(int i = 0; i < lines.size(); i++)
	{
		stringVector tmp = ft_split(lines[i], CHARTOSKIP);
		if (tmp[0] == "error_page")
		{
			if (this->getErrorPage().find(std::atoi(tmp[1].c_str())) == this->getErrorPage().end())
				this->setErrorPage(std::atoi(tmp[1].c_str()), tmp[2]);
		}
		else if (tmp[0] == "max_client_body_size")
		{
			if (this->getMaxClientBodySize() == 0)
				this->setMaxClientBodySize(std::atoi(tmp[1].c_str()));
		}
	}
}

const char* Server::InvalidServerException::what() const throw()
{
	return "InvalidServerException: Invalid server configuration!";
}

Server::~Server()
{
}