#ifndef CLIENT_H
# define CLIENT_H

# include "../includes.hpp"
# include "../Request/Request.hpp"
# include "../Config/Config.hpp"
# include "../Response/Response.hpp"
# include "../Response/ResponseBody.hpp"

class Client
{
	public:
		Client(void);
		Client(Client const & src);
		Client &operator=(const Client &rhs);
		~Client(void);

		bool	hadResponse;
		Request request;
		timeval	lastRequest;

		Response 		*response;
		ResponseBody	*responseBody;

		void	setServer(Server &server);
		void	SetSocket(int socket);
		Server	getServer() const;

	private:
		int		_socket;
		Server	_server;

};

#endif