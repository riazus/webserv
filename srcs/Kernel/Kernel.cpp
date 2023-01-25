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
    for(intVector::iterator it = _serverFds.begin(); it != _serverFds.end(); it++)
    {
        close(*it);
    }
    close(_epollFd);
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
	Client newClient;

	new_socket = accept(eventPollFd, NULL, NULL);
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
	for (intVector::iterator it = _serverFds.begin(); it != _serverFds.end(); it++)
		if (*it == eventPollFd)
			return true;
    return false;
}

void Kernel::InitEpoll()
{
	std::memset((struct epoll_event *)&this->_event, 0, sizeof(this->_event));
	for (intVector::iterator it = _serverFds.begin(); it != _serverFds.end(); it++)
	{
		this->_event.data.fd = *it;
		this->_event.events = EPOLLIN;
		epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, *it, &this->_event);
	}
}

// Returns prepared socket's fd for accept connection
int Kernel::CreateSocket(Server *server)
{
	const int opt = 1;
	int listen_fd;
	struct sockaddr_in servaddr;
	std::cout << "SOCKET()" << std::endl;
	sleep(2);
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		CloseSockets();
		throw std::logic_error("Error: socket() failed");
	}
	std::cout << "SETSOCKOPT()" << std::endl;
	sleep(2);
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &opt, sizeof(opt)))
	{
		CloseSockets();
		throw std::logic_error("Error: setsockopt() failed");
	}

	/*if (fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		CloseSockets();
		throw std::logic_error("Error: fcntl() failed");
	}*/

	servaddr.sin_family = AF_INET;
	//NB! Need to FIX
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	std::cout << "BIND NEW PORT: " << server->getPort() << std::endl;
	servaddr.sin_port = server->getPort();

	std::memset(servaddr.sin_zero, '\0', sizeof(servaddr.sin_zero));

	if (servaddr.sin_addr.s_addr == static_cast<in_addr_t>(-1))
	{
		CloseSockets();
		throw std::logic_error("Error: inet_addr: Invalid IP");
	}
	std::cout << "BIND()" << std::endl;
	sleep(2);
	if (bind(listen_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
		CloseSockets();
		throw std::logic_error("Error: bind() failed");
	}
	std::cout << "LISTEN()" << std::endl;
	sleep(2);
	if (listen(listen_fd, MAX_CLIENTS) < 0)
	{
		CloseSockets();
		throw std::logic_error("Error: listen() failed");
	}
	return listen_fd;
}

void Kernel::InitSocket()
{
	for(serverVector::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
	{
		this->_serverFds.push_back(this->CreateSocket(*it));
	}
	for(intVector::iterator it = this->_serverFds.begin(); it != this->_serverFds.end(); it++)
	{
		std::cout << "init server socket: " << *it << std::endl; 
	}
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
	this->_servers = this->_config->getServers();
	
	this->CreateEpoll();
	this->InitSocket();
	this->InitEpoll();
}

void Kernel::Run()
{
	std::string wait[] = {".   ", "..  ", "... ", "....", "   .", "  ..", " ...", "...."};
	int frames = 0;
	int nfds = 0;

	std::cout << "Servers initialization..." << std::endl;
    this->LoadKernel();
	while(true)
	{
		errno = 0;
		//std::cout << "EPOLL FD: " << this->_epollFd << std::endl;
		nfds = epoll_wait(this->_epollFd, this->_eventsArray, MAX_EV, APP_TIMEOUT);
		//std::cout << "NFDS COUNT: " << nfds << std::endl;
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
			std::cout << "Start handling..." << std::endl;
			if (this->_eventsArray[i].events & EPOLLERR ||  this->_eventsArray[i].events & EPOLLHUP)
			{
				throw std::logic_error("There is a bad event in events array");
			}
			else if (this->_eventsArray[i].events & EPOLLIN && this->fdIsServer(this->_eventsArray[i].data.fd))
			{
				std::cout << "Accept new client..." << std::endl;
				this->AcceptNewClient(this->_eventsArray[i].data.fd);
			}
			else if (this->_eventsArray[i].events & EPOLLIN)
			{
				std::cout << "Start read client's request..." << std::endl;
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

void Kernel::SetConfig(Config *config)
{
	this->_config = config;
}
