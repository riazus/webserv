#ifndef CLIENT_H
# define CLIENT_H

# include "../Request/Request.hpp"

class Client
{
	public:
		Client(void);
		Client(Client const & src);
		Client &operator=(const Client &rhs);
		~Client(void);

		void	SetSocket(int socket);	
		bool	hadResponse;
		Request	request;

	private:
		int	_socket;

};


#endif