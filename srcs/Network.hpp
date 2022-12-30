#ifndef NETWORK_H
# define NETWORK_H

# include "Kernel.hpp"

class Network
{
	public:
		Network(void);
		Network(Network const & src);
		Network &operator=(const Network &rhs);
		~Network(void);

		bool operator==(Network const &rhs);
		bool operator!=(Network const &rhs);
		friend std::ostream	&operator<<(std::ostream &out, const Network &net);

		struct in_addr	host;
		int				port;
		std::string		hostName; // for request only

	private:
		
};


#endif