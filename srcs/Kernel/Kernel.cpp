#include "Kernel.hpp"

Kernel::Kernel()
{
}

Kernel::Kernel(Kernel &copy)
{
	*this = copy;
}

Kernel &Kernel::operator=(Kernel &copy)
{
    return *this;
}

Kernel::~Kernel()
{
}

//Returns prepared socket's fd for accept connection
int Kernel::CreateSocket()
{
	const int opt = 1;

	if ((_socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		CloseSockets();
		throw std::logic_error("Error: socket() failed");
	}
	
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &opt, sizeof(opt)))
	{
		CloseSockets();
		throw std::logic_error("Error: setsockopt() failed");
	}

	/*if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) < 0)
	{
		CloseSockets();
		throw std::logic_error("Error: fcntl() failed");
	}*/

	_servaddr.sin_family = AF_INET;
	//NB! Need to FIX
	_servaddr.sin_addr.s_addr = INADDR_ANY;
	_servaddr.sin_port = htons(8080);

	std::memset(_servaddr.sin_zero, '\0', sizeof(_servaddr.sin_zero));

	if (_servaddr.sin_addr.s_addr == static_cast<in_addr_t>(-1))
	{
		CloseSockets();
		throw std::logic_error("Error: inet_addr: Invalid IP");
	}

	if (bind(_socketFd, (struct sockaddr*) &_servaddr, sizeof(_servaddr)) < 0)
	{
		CloseSockets();
		throw std::logic_error("Error: bind() failed");
	}

	if (listen(_socketFd, MAX_CLIENTS) < 0)
	{
		CloseSockets();
		throw std::logic_error("Error: listen() failed");
	}
	//return listen_fd;
	return 0;
}

void Kernel::ListenConnections()
{
	int new_socket;
	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	int addlen = sizeof(_servaddr);

	std::cout << "++++++++++++++++Waiting for new connections+++++++++++++++++++++" << std::endl;
	std::cout << "_socketFd = " << _socketFd << std::endl;
	new_socket = accept(_socketFd, (struct sockaddr *)&_servaddr, (socklen_t*)&addlen);
	if (new_socket < 0)
	{
		perror("In accept");
        exit(EXIT_FAILURE);
		throw std::logic_error("Error: accept() failed");
	}
	char buffer[BUFFER_SIZE] = {0};
	read(new_socket, buffer, BUFFER_SIZE);
	
	//std::string openFile = Kernel::getPath(buffer);



	std::cout << buffer << std::endl;
	write(new_socket, hello.c_str(), hello.length());
	std::cout << "---------------Hello message sent---------------------" << std::endl;
	close(new_socket);
}

void Kernel::CloseSockets()
{
    /*for(intVector::iterator it = _socketFd.begin(); it != _socketFd.end(); it++)
    {
        close(*it);
    }*/
	close(_socketFd);
    //close(_epollFd);
}

//Parsing http request and return ready path to open file
std::string Kernel::getPath(std::string buffer)
{
	//TODO: Matvey
    return std::string("index.html");
}
