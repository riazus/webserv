#ifndef CONFIG_H
# define CONFIG_H

#include "../Server/Server.hpp"
#include "../Parser/Location.hpp"

class Config
{
	private:

	std::vector<Server *> servers;
	std::string configFile;

	public:
		Config(void);
		Config(Config const & src);
		Config &operator=(const Config &rhs);

		std::string getFile(std::string filename);
		void Parsing(std::string filename);
		void getFullInfo();
		serverVector getServers();

		~Config(void);
	
	class ConfigFileOpenException : public std::exception
	{
	    public:

		    virtual const char* what() const throw();
	};

	class ConfigFileContentException : public std::exception
	{
	    public:

		    virtual const char* what() const throw();
	};
};

std::vector<std::string> ft_split(const std::string &str, const std::string &charset);	
Server *parse_server(std::vector<std::string> config, int *line_count);
Location parse_location(std::vector<std::string> &config, int *line_count, Server* server);
bool is_integer(const std::string &s);
void config_error(std::string error);

#endif