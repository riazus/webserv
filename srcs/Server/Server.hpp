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

		stringVector                        names;
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
		std::string                         locationName;
		std::string                         alias;
		pairString                          ret;
		std::string                         cgiPass;
		bool                                extension;

		void                                checkForDefaultFields();

	public:

		Server();
		Server(const Server &Server);
		Server& operator=(const Server &Server);
		bool operator==(const Server &rhs);

		void 	setServerName(std::string name);
		void 	setPort(int port);
		void 	setRoot(std::string root);
		void 	setIndex(std::string index);
		void 	setMethods(std::string methods);
		void 	setLocation(Server location);
		void 	setErrorPage(int num, std::string page);
		void 	setCgi(std::string name, std::string path);
		void 	setMaxClientBodySize(long long size);
		void 	setHostAddr(in_addr_t addr);
		void 	setHostName(std::string name);
		void 	setIpAddress(std::string ip_adress);
		void 	setAutoindex(bool var);
		void 	setIsExtension(bool);
		void 	setAlias(std::string alias);
		void 	setReturn(std::string code, std::string url);
		void	setLocationName(std::string str);
		void	setCgiPass(std::string pass);


		stringVector        &getServerName();
		int                 &getPort();
		std::string         &getIpAdress();
		std::string 		getRoot() const;
		std::string 		&getIndex();
		stringVector 		&getMethods();
		std::vector<Server> &getLocations();
		mapError 			&getErrorPage();
		mapString 			&getCgi();
		long long 			&getMaxClientBodySize();
		std::string 		&getHostName();
		in_addr_t 			getHostAddr();
		bool 				&getAutoindex();
		std::string 		&getAlias();
		pairString 			&getReturn();
		std::string 		getLocationName() const;
		bool 				&getIsExtension();
		std::string 		&getCgiPass();


		void parse_server(std::vector<std::string> config, int *line_count, int locLevel);
		void server_error(std::string error);

		~Server();

		class InvalidServerException : public std::exception
		{
		public:

			virtual const char* what() const throw();
		};
};

#endif