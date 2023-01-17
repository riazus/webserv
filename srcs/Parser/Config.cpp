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
	getFullInfo();
}

void Config::getFullInfo()
{
	for (int i=0; i < servers.size(); i++)
	{
		std::cout << "server_name:	" << servers[i]->getServerName() << std::endl;
		std::cout << "port:	";
		for (int i=0; i < servers[i]->getPort().size(); i++)
			std::cout << servers[i]->getPort()[i] << ", ";
		std::cout << std::endl;
		std::cout << "root:	" << servers[i]->getRoot() << std::endl;
		std::cout << "index:	" << servers[i]->getIndex() << std::endl;
		std::cout << "methods:	";
		for (int i=0; i < servers[i]->getMethods().size(); i++)
			std::cout << servers[i]->getMethods()[i] << ", ";
		std::cout << std::endl;
		// std::cout << "cgi:	" << servers[i]->getCgi() << std::endl;
		std::cout << "max_client_body_size:	" << servers[i]->getMaxClientBodySize() << std::endl;
	}
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

