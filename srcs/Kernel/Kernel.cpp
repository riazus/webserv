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

void Kernel::CloseSockets()
{
	//TODO:
    /*for(intVector::iterator it = _socketFd.begin(); it != _socketFd.end(); it++)
    {
        close(*it);
    }*/
	close(_socketFd);
    //close(_epollFd);
}

void Kernel::DeleteClient(int socketFd)
{
	this->_clients.erase(socketFd);
	epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, socketFd, NULL);
	close(socketFd);
}

// Parsing http request and return ready string to send response
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
    return std::string("HTTP/1.1 200 OK \nContent-Type: text/html\nContent-Length: 142\n\n" + body);
}

bool Kernel::WriteClientRequest(int socketFd)
{

    return false;
}

void Kernel::ClientWrite(int eventPollFd)
{
	throw std::logic_error("Non implemented ClientWrite");
	
	std::string response;

	std::cout << "CLIENT WRITE TO SERVER" << std::endl;
	std::cout << _clients[eventPollFd].request.requestLine << std::endl;

	//this->getRightsServer(this->_clients[eventPollFd]);

	//response = this->_clients[eventPollFd].
}

bool Kernel::ReadClientRequest(int clientSocket)
{
	char buffer[BUFFER_SIZE + 1];
	std::string body("");

	ssize_t requestLen = read(clientSocket, buffer, BUFFER_SIZE);

	std::cout << buffer << std::endl;
	std::cout << "LOL" << std::endl;
	if (requestLen == -1)
	{
		this->DeleteClient(clientSocket);
		//throw std::logic_error("Error: read Client's request failed");
		return false;
	}
	else if (requestLen == 0)
	{
		this->DeleteClient(clientSocket);
		return false;
	}
	else
	{
		//TODO check for len after a lot of manipulation
		this->_clients[clientSocket].request.contetnSize += requestLen;
		//timestamp
		this->_clients[clientSocket].request.requestLine.append(buffer, requestLen);
	}

	//TODO Matvey - parse buffer content
	if (this->_clients[clientSocket].request.requestLine.find("\r\n\r\n") != std::string::npos
		&& this->_clients[clientSocket].request.headerReady == false)
	{
		//this->_parser.parseHeader(this->_clients[clientSocket].request) ???
		this->_clients[clientSocket].request.headerReady = true;

	}
	if (this->_clients[clientSocket].request.headerReady == true)
	{
		body = this->_clients[clientSocket].request.requestLine.substr(this->_clients[clientSocket].request.requestLine.find("\r\n\r\n") + 4);
		std::string header = this->_clients[clientSocket].request.requestLine.substr(0, this->_clients[clientSocket].request.requestLine.find("\r\n\r\n") + 4);

		std::cout << body << std::endl;
		std::cout << header << std::endl;
	}

	//Afterwork need change
    return false;
}

void Kernel::ClientRead(int eventPollFd)
{
	if (this->_clients[eventPollFd].hadResponse == true)
	{
		this->_clients[eventPollFd].hadResponse = false;
	}
	if (this->ReadClientRequest(eventPollFd) == false)
	{
		return;
	}
	if (this->_clients[eventPollFd].request.bodyReady == true)
	{
		this->_event.events = EPOLLOUT;
		this->_event.data.fd = eventPollFd;
		epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, eventPollFd, &this->_event);
	}
}

//Accept event's fd to the new client
void Kernel::AcceptNewClient(int eventPollFd)
{
	int new_socket;
	int addlen = sizeof(_servaddr);
	Client newClient;

	new_socket = accept(eventPollFd, (struct sockaddr *)&_servaddr, (socklen_t*)&addlen);
	if (new_socket < 0 && errno != EWOULDBLOCK)
	{
		throw std::logic_error("Error: accept() failed");
	}

	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		throw std::logic_error("Error: fcntl() failed");
	}

	this->_event.data.fd = new_socket;
	this->_event.events = EPOLLIN;
	epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, new_socket, &this->_event);

	newClient.SetSocket(new_socket);
	this->_clients[new_socket] = newClient;
}

bool Kernel::fdIsServer(int eventPollFd)
{

    return false;
}

void Kernel::InitEpoll()
{
	std::memset((struct epoll_event *)&this->_event, 0, sizeof(this->_event));
	//TODO: NEED IN CYCLE! 
	this->_event.data.fd = this->_socketFd;
	this->_event.events = EPOLLIN;
	epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, this->_socketFd, &this->_event);
}

// Returns prepared socket's fd for accept connection
int Kernel::CreateSocket()
{
	const int opt = 1;

	if ((this->_socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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

void Kernel::CreateEpoll()
{
	this->_epollFd = epoll_create1(0);
	if (this->_epollFd == -1)
	{
		throw std::logic_error("Error: epoll_create1() couldn't init");
	}
}

void Kernel::LoadKernel()
{
	this->CreateEpoll();
	this->CreateSocket();
	this->InitEpoll();
}

void Kernel::Run()
{
	std::string wait[] = {".   ", "..  ", "... ", "....", "   .", "  ..", " ...", "...."};
	int frames = 0;
	int nfds = 0;

    this->LoadKernel();
	while(true)
	{
		errno = 0;
		nfds = epoll_wait(this->_epollFd, this->_eventsArray, MAX_EV, APP_TIMEOUT);
		if (errno == EINVAL || errno == EFAULT || errno == EBADFD)
		{
			throw std::logic_error("Error: epoll_wait() failed");
		}
		else if (errno == EINTR)
		{
			break;
		}

		//TODO: Main logic ->
		for (int i = 0; i < nfds; i++)
		{
			if (this->_eventsArray[i].events & EPOLLERR || this->_eventsArray[i].events & EPOLLHUP)
			{
				throw std::logic_error("There is a bad event in events array");
			}
			else if (this->_eventsArray[i].events & EPOLLIN && this->fdIsServer(this->_eventsArray[i].data.fd))
			{
				this->AcceptNewClient(this->_eventsArray[i].data.fd);
			}
			else if (this->_eventsArray[i].events & EPOLLIN)
			{
				this->ClientRead(this->_eventsArray[i].data.fd);
			}
			else if (this->_eventsArray[i].events & EPOLLOUT)
			{
				//TODO:
				this->ClientWrite(this->_eventsArray[i].data.fd);
			}
		}

		if (nfds == 0)
		{
			std::cout << "\r" << wait[(frames++ % 8)] << " Waiting for new connections " << std::flush;
			sleep(1);
		}
	}
	this->CloseSockets();
	std::cout << "Shutting down server..." << std::endl;
}