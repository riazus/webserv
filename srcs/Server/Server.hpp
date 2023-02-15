#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes.hpp"
// #include "../Parser/Location.hpp"
#include "../Config/Config.hpp"

class Config;
// class Location;


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
        stringVector                        methods;
        mapError                            error_page;
        mapString                           cgi;
        std::vector<Server>                 location;
        long long                           max_client_body_size;
        bool                                autoindex;
        std::string                         locationPath;
        std::string                         alias;
        pairString                          ret;
        stringVector                        cgi_param;


        bool                                extension;

        void                                checkForDefaultFields();

    public:

        Server();
        Server(const Server &Server);
        Server& operator=(const Server &Server);
        bool operator==(const Server &rhs);

        void setServerName(std::string name);
        void setPort(int port);
        void setRoot(std::string root);
        void setIndex(std::string index);
        void setMethods(std::string methods);
        void setLocation(Server location);
        void setErrorPage(int num, std::string page);
        void setCgi(std::string name, std::string path);
        void setMaxClientBodySize(long long size);
        void setHostAddr(in_addr_t addr);
        void setHostName(std::string name);
        void setIpAddress(std::string ip_adress);
        void setAutoindex(bool var);
        void setLocationPath(std::string path);
        void setIsExtension(bool);
        void setAlias(std::string alias);
        void setReturn(std::string code, std::string url);
        void setCgiParam(std::string cgi_param);
        //void setLocationName(std::string str);



        std::string getLocationPath() const;
        std::string &getServerName();
        int &getPort();
        std::string &getIpAdress();
        std::string getRoot() const;
        std::string &getIndex();
        stringVector &getMethods();
        std::vector<Server> &getLocations();
        mapError &getErrorPage();
        std::map<std::string, std::string> &getCgi();
        long long &getMaxClientBodySize();
        std::string &getHostName();
        in_addr_t getHostAddr();
        bool &getAutoindex();
        std::string &getAlias();
        pairString &getReturn();
        stringVector &getCgiParam();
        //std::string getLocationName() const;
        bool &getIsExtension();



        void parse_server(std::vector<std::string> config, int *line_count, bool is_location);
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