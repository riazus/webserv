#ifndef CONFIG_H
# define CONFIG_H

#include "Includes.hpp"

class Server;
class Location;

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