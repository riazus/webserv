#ifndef REQUEST_H
# define REQUEST_H

# include "../includes.hpp"
# include "../Server/Server.hpp"

class Request
{
	public:
		Request(void);
		Request(Request const & src);
		Request &operator=(const Request &rhs);
		~Request(void);
		
		void			ResetHeaders();
		void			ResetRequest();
		
		int				bodySize;
		int				contentSize;
		std::string 	requestLine;
		bool			bodyReady;
		bool			headerReady;
		stringVector	header;

		//GETTERS
		int				getCode() const;
		std::string 	getHeader(std::string header);
		std::string		getMethod() const;
		mapString		getHeaders() const;
		Server			getServer()	const;
		std::string		getPath() const;
		std::string		getBody() const;
		std::string		getQuery() const;
		std::string		getDomainName() const;

		//SETTERS
		void			setCode(int code);
		void			setBody(std::string body);
		void			setMethod(std::string method);
		void			setPath(std::string path);
		void			setVersion(std::string version);
		void			setHeader(std::string token, std::string value);
		void 			setEnvForCgi(std::string token, std::string value);
		void			setNetwork(std::string ip);
		void			setServer(Server server);
		void			setDomainName(std::string name);

	private:
		int				_code;
		mapString		_headers;
		mapString		_env_for_cgi;
		std::string		_body;
		std::string 	_version;
		std::string 	_method;
		std::string 	_path;
		std::string 	_query;
		Server			_server;
		std::string		_domainName;
		
};


#endif