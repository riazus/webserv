#ifndef CONFIG_H
# define CONFIG_H

#include <fstream>
#include <string>
#include "Server.hpp"
#include "Location.hpp"

class Config
{
	private:

	std::vector<Server> servers;
	std::string configFile;

	public:
		Config(void);
		Config(Config const & src);
		Config &operator=(const Config &rhs);
		void getFile(std::string filename);
		void getServers();
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


#endif