#ifndef RESPONSEBODY_H
# define RESPONSEBODY_H

# include "../includes.hpp"
# include "../Request/Request.hpp"

class ResponseBody
{
	public:
		ResponseBody(void);
		ResponseBody(ResponseBody const & src);
		ResponseBody &operator=(const ResponseBody &rhs);
		~ResponseBody(void);

		void				setRequest(Request &request);
		void				setServer(Server server);
		void				setLocation(Server location);
		void				setLocationPath(std::string path);
		void				setLocationFile(std::string path);
		void				setErrorMap(mapError map);
		void				setClientBodyBufferSize(size_t size);
		void				setCgiPass(std::string cgiPass);
		void				setAllowMethod(stringVector allow);
		void				setLanguage(std::string lang);
		void				setAutoIndex(bool Index);
		void				setIndex(std::string index);
		void				setCode(int code);
		void				setCookies(mapString cookies);
		void				setContentLocation(std::string content);
		void				setContent(std::string content);

		mapString			getCookies(void) const;
		std::string			getCookie(std::string key) const;
		int					getCode() const;
		Request				getRequest() const;
		Server				getServer(void) const;
		Server				getLocation() const;
		std::string			getLocationPath(void) const;
		std::string			getLocationFile(void) const;
		mapError			getErrorMap(void) const;
		std::string			getErrorPath(int code) const;
		size_t				getClientBodyBufferSize(void) const;
		std::string			getCgiPass(void) const;
		stringSet			getAllowMethod() const;
		stringVector		getAllow() const;
		std::string			getLanguage() const;
		bool				getAutoIndex(void) const;
		std::string			getIndex(void) const;
		std::string			getContentLocation() const;
		std::string			getContent() const;

	private:
		Request			_request;
		Server			_server;
		Server			_location;
		std::string		_locationPath;
		std::string		_locationFile;
		mapError		_errorPage;
		size_t			_clientBodyBufferSize;
		std::string		_cgiPass;
		stringSet		_allowMethod;
		stringVector	_allow;
		std::string		_acceptLanguage;
		bool			_autoIndex;
		std::string		_index;
		std::string		_contentLocation;
		std::string		_content;
		int				_code;
		mapString		_cookies;		
};


#endif