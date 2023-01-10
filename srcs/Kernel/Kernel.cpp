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
	//std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 142\n\n<html>\n<body>\n<center>\n<h1>Hi! This is webserv written by Riyaz and Matvey, enjoy!</h1>\n<center>\n</body>\n</html>";
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
	
	std::string readyResponse = Kernel::getResponse(buffer);

	std::cout << buffer << std::endl;
	write(new_socket, readyResponse.c_str(), readyResponse.length());
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

//Parsing http request and return ready string to send response
std::string Kernel::getResponse(std::string buffer)
{
	//TODO: Matvey
	/*
    1.. Need validate path from buffer
	2. Validate any cases of denied access(cannot open file, permission, etc.)
	And many many more... :)
	*/
	std::string body;
	//body = "<b>Pick your favorite color</b><br>\n<form method="POST" action="http://www.whizkidtech.redprince.net/cgi-bin/c">\n<input type="RADIO" name="color" value="red"> Red<br>\n<input type="RADIO" name="color" value="green"> Green<br>\n<input type="RADIO" checked name="color" value="blue"> Blue<br>\n<input type="RADIO" name="color" value="cyan"> Cyan<br>\n<input type="RADIO" name="color" value="magenta"> Magenta<br>\n<input type="RADIO" name="color" value="yellow"> Yellow<br>\n<br><b>On the scale 1 - 3, how favorite is it?</b><br><br>\n<select name="scale" size=1>\n<option>1\n<option selected>2\n<option>3\n</select>\n<br>\n<input type="HIDDEN" name="favorite color" size="32">\n<input type="Submit" value="I'm learning" name="Attentive student">\n<input type="Submit" value="Give me a break!" name="Overachiever">\n<input type="Reset" name="Reset">\n</form>";
	body = "<html>\n<body>\n<center>\n<h1>Hi! This is webserv written by Riyaz and Matvey, enjoy!</h1>\n<center>\n</body>\n</html>";
	//body = "<!DOCTYPE html>\n<html>\n<title>404 Not Found</title>\n<body>\n<div>\n<H1>404 Not Found</H1>\n</div>\n</body>\n</html>";
    return std::string("HTTP/1.1 404 \nContent-Type: text/html\nContent-Length: 142\n\n" + body);
}
