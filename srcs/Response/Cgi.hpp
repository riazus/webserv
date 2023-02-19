#ifndef Cgi_H
# define Cgi_H

# include "ResponseBody.hpp"

class Cgi
{
	public:
		Cgi(void);
		Cgi(Cgi const & src);
		~Cgi(void);

		Cgi		&operator=(const Cgi &rhs);

		std::string		execute(void);
		char 			**mapToTab(void);
		void			setEnv(void);
		void			initCgiData(ResponseBody& config);


	private:
		mapString		_env;
		std::string		_body;
		std::string		_cgiPass;
		std::string		_method;
		std::string		_contentSize;
		std::string		_toExe;
		std::string		_contentType;
		std::string		_contentPath;
		std::string		_contentFile;
		std::string		_contentPathFile;
		std::string		_query;
		std::string		_port;
		std::string		_host;
		std::string		_authorisation;
		std::string		_protocol;
		std::string		_serverName;
		std::string		_interface;
		std::string		_status;
};


#endif
