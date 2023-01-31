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
	return *this;
}

std::string Config::getFile(std::string filename)
{
	std::ifstream file;
	std::string s;

	file.open(filename);
	if (!file.is_open())
		throw Config::ConfigFileOpenException();
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	configFile.append("\0");
	return content;
}

void Config::Parsing(std::string filename)
{
	std::string config_file = getFile(filename);

	if (!config_file.length())
		throw Config::ConfigFileContentException();
	std::vector<std::string> config = ft_split(config_file, "\n");
	if (!config.size())
		throw Config::ConfigFileContentException();
	int line_count = 0;
	while (line_count < config.size())
	{
		std::vector<std::string> line = ft_split(config[line_count], CHARTOSKIP);
		if (line[0] == "server")
		{
			Server *server = parse_server(config, &line_count);
			servers.push_back(server);
		}
		line_count++;
	}
	//getFullInfo();
}

void Config::getFullInfo()
{
	std::cout << "--------------------CONFIG FILE INFO--------------------" << std::endl;
	for (int i=0; i < servers.size(); i++)
	{
		std::cout << "===================Server " << i << "===================" << std::endl << std::endl;
		std::cout << "server_name:	" << servers[i]->getServerName();
		std::cout << " port:	" << servers[i]->getPort() << std::endl;
		std::cout << "root:	" << servers[i]->getRoot() << std::endl;
		std::cout << "index:	" << servers[i]->getIndex() << std::endl;
		std::list<Location *> loc_tmp = servers[i]->getLocation();
		std::list<Location *>::iterator loc_end = loc_tmp.end();
		for (std::list<Location *>::iterator loc = loc_tmp.begin(); loc != loc_end; loc++)
		{
			std::cout << "+++++++++++++++++++Location+++++++++++++++++++" << std::endl << std::endl;
			std::cout << "root:	" << (*loc)->getPath() << std::endl;
			std::cout << "index:	" << (*loc)->getIndex() << std::endl;
			std::cout << "autoindex:	" << (*loc)->getAutoindex() << std::endl;
			std::cout << "autoindex:	" << (*loc)->getAutoindex() << std::endl;
			std::cout << "autoindex:	" << (*loc)->getAutoindex() << std::endl;
		}
		std::map<std::string, std::string> map_tmp = servers[i]->getCgi();
		std::map<std::string, std::string>::iterator it(map_tmp.begin());
		std::map<std::string, std::string>::iterator end(map_tmp.end());
		while (it != end)
		{
			std::cout << "cgi:	" << it->first <<  "  " << it->second << std::endl;
			it++;
		}
		std::map<int, std::string> map_tmp2 = servers[i]->getErrorPage();
		std::map<int, std::string>::iterator it2(map_tmp2.begin());
		std::map<int, std::string>::iterator end2(map_tmp2.end());
		while (it2 != end2)
		{
			std::cout << "error_pages:	" << it2->first <<  "  " << it2->second << std::endl;
			it2++;
		}
		//std::cout << "max_client_body_size:	" << servers[i]->getMaxClientBodySize() << std::endl;
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

