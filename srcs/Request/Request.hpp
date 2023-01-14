#ifndef REQUEST_H
# define REQUEST_H

# include <iostream>

class Request
{
	public:
		Request(void);
		Request(Request const & src);
		Request &operator=(const Request &rhs);
		~Request(void);
		
		int	contetnSize;
		std::string requestLine;

	private:
		
};


#endif