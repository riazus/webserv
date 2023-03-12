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

