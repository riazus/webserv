#ifndef CLIENT_H
# define CLIENT_H

# include "../includes.hpp"
# include "../Request/Request.hpp"
# include "../Config/Config.hpp"
# include "../Response/Response.hpp"

class Client
{
	public:
		Client(void);
		Client(Client const & src);
		Client &operator=(const Client &rhs);
		~Client(void);

		void	SetSocket(int socket);	
		bool	hadResponse;
		Request request;
		timeval	lastRequest;

	private:
		int	_socket;

};

#endif