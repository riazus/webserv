#ifndef RESPONSE_H
# define RESPONSE_H

# include "../includes.hpp"
# include "ResponseBody.hpp"

class Response
{
	public:
		Response(void);
		Response(Response const & src);
		Response &operator=(const Response &rhs);
		~Response(void);

		std::string		getResponse() const;
		bool			getIsValid() const;

		void			setResponse(std::string response);

		void			getMethod();
		void			postMethod();
		void			deleteMethod();

		void			resetResponse(ResponseBody responseBody);
		void			createHeader();
		void			initErrorMap();
		void			initDirectories();
		void			initResponseProcess();
		std::string		readFile(int code);
		std::string		readFile(std::string path);

		std::string		UserId;

	private:
		std::string			getErrorFileBody(int code);
		static methodMap	initMethods(void);
		static methodMap	_method;
		ResponseBody	*_responseBody;
		mapError		_status;
		int				_code;
		mapString		_directives;
		std::string		_header;
		std::string		_body;
		std::string		_response;
		bool			_isValid;
};

static std::string	getDate(void);
static std::string	getLastMod(const std::string& path);
static std::string	ft_itoa(int nb);
static std::string	gen_random(const int len);


#endif