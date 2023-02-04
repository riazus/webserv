#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes.hpp"
#include "../Parser/Location.hpp"
#include "../Config/Config.hpp"

class Config;
class Location;


class Server
{

    private:

        std::string                         name;
        int                                 port;
        std::string                         ip_adress;
        struct in_addr	                    host;
        std::string                         hostName;
        std::string                         root;
        std::string                         index;
        std::vector<std::string>            methods;
        std::map<int, std::string>          error_page;
        std::map<std::string, std::string>  cgi;
        std::list<Location *>               location;
        long long                           max_client_body_size;


    public:

        Server();
        Server(const Server &Server);
        Server& operator= (const Server &Server);


        void setServerName(std::string name);
        void setPort(int port);
        void setRoot(std::string root);
        void setIndex(std::string index);
        void setMethods(std::string methods);
        void setLocation(Location *location);
        void setErrorPage(int num, std::string page);
        void setCgi(std::string name, std::string path);
        void setMaxClientBodySize(long long size);
        void setHostAddr(in_addr_t addr);
        void setHostName(std::string name);
        void setIpAddress(std::string ip_adress);

        std::string getServerName();
        int getPort();
        std::string getIpAdress();
        std::string getRoot();
        std::string getIndex();
        std::vector<std::string> getMethods();
        Location *getLocation();
        std::map<int, std::string> getErrorPage();
        std::map<std::string, std::string> getCgi();
        long long getMaxClientBodySize();
        std::string getHostName();
        in_addr_t getHostAddr();
        static Server *parse_server(std::vector<std::string> config, int *line_count);

        void server_error(std::string error);
        void is_valid();

        ~Server();

        class InvalidServerException : public std::exception
    	{
	    public:

		    virtual const char* what() const throw();
	    };
};

#endif