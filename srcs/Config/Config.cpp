#include "Config.hpp"

Config::Config(void)
{
}

Config::Config(Config const & src)
{
	*this = src;
}

Config &Config::operator=(Config const &rhs)
{
	this->servers = rhs.servers;
	this->configFile = rhs.configFile;
	return *this;
}

std::string getFile(std::string filename)
{
	std::ifstream file;
	std::string s;

	file.open(filename.c_str());
	if (!file.is_open())
		throw Config::ConfigFileOpenException();
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return content;
}


void Config::Parsing(std::string filename)
{
	std::string config_file = getFile(filename);
	configFile.append("\0");


	if (!config_file.length())
		throw Config::ConfigFileContentException();
	stringVector config = ft_split(config_file, "\n");
	if (!config.size())
		throw Config::ConfigFileContentException();
	int line_count = 0;
	while (line_count < config.size())
	{
		Server server;
		stringVector line = ft_split(config[line_count], CHARTOSKIP);
		if (line[0] == "server")
		{
			server.parse_server(config, &line_count, 0);
			this->servers.push_back(server);
		}
		line_count++;
	}
	std::vector<Server>tmp = servers;
	std::vector<Server>::iterator end = tmp.end();
	int i = 0;
	for(std::vector<Server>::iterator it = tmp.begin(); it != end; it++)
	{
		i = 0;
		for(std::vector<Server>::iterator tmp_it = tmp.begin(); tmp_it != end; tmp_it++)
		{
			if ((tmp_it)->getPort() == (it)->getPort())
				i++;
		}
		if (i > 1)
			config_error("same port specified twice");
	}
	//getFullInfo();
}

void Config::getFullInfo()
{
	std::cout << "--------------------CONFIG FILE INFO--------------------" << std::endl;
	for (int i=0; i < servers.size(); i++)
	{
		std::cout << "===================Server " << i << "===================" << std::endl << std::endl;
		std::cout << "server_name:	" << servers[i].getServerName() << std::endl;
		std::cout << "port:	" << servers[i].getPort() << std::endl;
		std::cout << "ip_adress:	" << servers[i].getIpAdress() << std::endl;
		std::cout << "root:	" << servers[i].getRoot() << std::endl;
		std::cout << "index:	" << servers[i].getIndex() << std::endl;
		std::map<std::string, std::string> map_tmp = servers[i].getCgi();
		std::map<std::string, std::string>::iterator it(map_tmp.begin());
		std::map<std::string, std::string>::iterator end(map_tmp.end());
		while (it != end)
		{
			std::cout << "cgi:	" << it->first <<  "  " << it->second << std::endl;
			it++;
		}
		std::map<int, std::string> map_tmp2 = servers[i].getErrorPage();
		std::map<int, std::string>::iterator it2(map_tmp2.begin());
		std::map<int, std::string>::iterator end2(map_tmp2.end());
		while (it2 != end2)
		{
			std::cout << "error_pages:	" << it2->first <<  "  " << it2->second << std::endl;
			it2++;
		}
		std::cout << "max_client_body_size:	" << servers[i].getMaxClientBodySize() << std::endl;
		std::vector<Server> loc_tmp = servers[i].getLocations();
		std::vector<Server>::iterator loc_end = loc_tmp.end();
		for (std::vector<Server>::iterator loc = loc_tmp.begin(); loc != loc_end; loc++)
		{
			std::cout << "+++++++++++++++++++Location+++++++++++++++++++" << std::endl << std::endl;
			std::cout << "root:	" << (loc)->getPath() << std::endl;
			std::cout << "index:	" << (loc)->getIndex() << std::endl;
			std::cout << "autoindex:	" << (loc)->getAutoindex() << std::endl;
		}
		std::cout << std::endl;
	}
}

serverVector Config::getServers()
{
    return this->servers;
}

const char* Config::ConfigFileOpenException::what() const throw()
{
	return "ConfigFileOpenException : File open error!";
}

const char* Config::ConfigFileContentException::what() const throw()
{
	return "ConfigFileContentException : Invalid configuration file content!";
}

Config::~Config(void)
{
}

