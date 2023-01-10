#ifndef SERVER_HPP
#define SERVER_HPP

#include "./Location.hpp"
// #include "./Config.hpp"
#include <string.h>

class Server
{

    private:

        int braceCounter;
        std::string name;
        std::string buffer;
        std::vector<std::string> listen;
        std::string root;
        std::string index;
        std::vector<std::string> methods;
        std::vector<Location> location;

    public:

        Server();
        Server(const std::string name);
        Server(const Server &Server);
        Server& operator= (const Server &Server);
        int getServerInfo(std::string configFile);
        void setName(std::string name);
        std::string getName();
        std::string myGetLine(std::string *buffer);
        ~Server();

    	class ConfigFileContentException : public std::exception
	{
	    public:

		    virtual const char* what() const throw();
	};
};

#endif