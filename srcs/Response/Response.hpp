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
		void			setResponse(std::string response);
		bool			isReady;

		void			getMethod();
		void			postMethod();
		void			deleteMethod();

	private:
		ResponseBody	*_responseBody;
		mapError		_status;
		int				_code;
		mapString		_directives;
		std::string		_header;
		std::string		_body;
		std::string		_response;
};


#endif