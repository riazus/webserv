#ifndef RESPONSE_H
# define RESPONSE_H

# include "../Request/Request.hpp"

class Response
{
	public:
		Response(void);
		Response(Response const & src);
		Response &operator=(const Response &rhs);
		~Response(void);

	private:
		
};


#endif