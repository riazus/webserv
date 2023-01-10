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

void Config::getServers()
{
	Server tmp;
	int i = 0;

	tmp.setName("serv1");
	std::cout << tmp.getName() << std::endl;

	tmp.getServerInfo(configFile);

	// if (servers.size() == 0)
	// 	throw Config::ConfigFileContentException();
}

void Config::getFile(std::string filename)
{
	std::ifstream file;
	std::string s;

	file.open(filename);
	if (!file.is_open())
		throw Config::ConfigFileOpenException();
	while(getline(file, s))
	{
		configFile.append(s);
		configFile.append("\n");
	}
	configFile.append("\0");
	getServers();
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

