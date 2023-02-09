#include "Kernel.hpp"

volatile bool g_run = true;

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
    for(intVector::iterator it = _serverFds.begin(); it != _serverFds.end(); it++)
        close(*it);
    close(_epollFd);
}

void Kernel::DeleteClient(int socketFd)
{
	this->_clients.erase(socketFd);
	epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, socketFd, NULL);
	close(socketFd);
}

void Kernel::getServerForClient(Client &client)
{
	bool foundAConf = false;
	Server rightServer = (*(this->_servers.begin()));

	/*for (serverVector::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (!(client.request.getServer()->getHostName().empty()))
		{
			std::string serverName = (*it)->getHostName();
			if (client.request.getServer()->getHostName() == serverName && (*it)->getPort() == client.request.getServer()->getPort())
			{
				foundAConf = true;
				rightServer = *it;
				break ;
			}
		}
		else
		{
			if (client.request.getServer() == *it)
			{
				rightServer = *it;
				foundAConf = true;
			}
		}
		if (foundAConf)
			break ;
	}*/
	client.setServer(rightServer);
}

void Kernel::ClientWrite(int eventPollFd)
{
	//throw std::logic_error("Non implemented ClientWrite");
	
	std::string response;
	//std::cout << _clients[eventPollFd].request.requestLine << std::endl;
	this->getServerForClient(this->_clients[eventPollFd]);
	this->_parserMsg.ParseResponse(this->_clients[eventPollFd].responseBody, this->_clients[eventPollFd].request, *this->_clients[eventPollFd].getServerAddr());
	this->_clients[eventPollFd].response.resetResponse(this->_clients[eventPollFd].responseBody);

	this->_clients[eventPollFd].response.initResponseProcess();

	//response = "HTTP/1.1 200 OK \nContent-Type: text/html\nContent-Length: 142\n\n<html>\n<body>\n<center>\n<h1>Hi! This is webserv written by Riyaz and Matvey, enjoy!</h1>\n<center>\n</body>\n</html>";
	
	if (this->_clients[eventPollFd].UserId == "")
		this->_clients[eventPollFd].UserId = this->_clients[eventPollFd].response.UserId;

	if (this->_clients[eventPollFd].response.getIsValid() == false)
		return ;
	std::cout << eventPollFd <<" :EV <-> END REQUEST " << this->_clients[eventPollFd].request.getMethod() << std::endl;
	response = this->_clients[eventPollFd].response.getResponse();
	if (write(eventPollFd, response.c_str(), response.size()))
		this->DeleteClient(eventPollFd);
	this->_clients[eventPollFd].hadResponse = true;

	//request
	char buffer[100];
	struct tm *tm = gmtime(&this->_clients[eventPollFd].lastRequest.tv_sec);
	strftime(buffer, 100, "%F - %T", tm);
	
	std::cout << std::endl << "CLIENT: " << buffer <<" | "<< this->_clients[eventPollFd].request.getMethod()+" " << this->_clients[eventPollFd].request.getPath()+" ";
	std::cout << this->_clients[eventPollFd].request.getServer().getServerName()+" " << this->_clients[eventPollFd].request.getServer().getPort() << std::endl;
	std::cout << std::endl << "CLIENT: " << this->_clients[eventPollFd].request.getMethod()+" " << this->_clients[eventPollFd].request.getPath()+" "<< this->_clients[eventPollFd].request.getServer().getServerName()+" " << this->_clients[eventPollFd].request.getServer().getPort() << std::endl;

	this->_event.events = EPOLLIN;
	this->_event.data.fd = eventPollFd;
	epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, eventPollFd, &this->_event);
	this->_clients[eventPollFd].request.ResetRequest();
}

bool Kernel::ReadClientRequest(int clientSocket)
{
	char buffer[BUFFER_SIZE + 1];
	std::string body("");
	this->_clients[clientSocket].request.setServer(this->_clients[clientSocket].getServer());
	//
	ssize_t requestLen = read(clientSocket, buffer, BUFFER_SIZE);
	//std::cout << buffer << std::endl;
	if (requestLen == -1)
	{
		this->DeleteClient(clientSocket);
		//throw std::logic_error("Error: read Client's request failed");
		return false;
	}
	else if (requestLen == 0) //Closing connection request from clients
	{
		this->DeleteClient(clientSocket);
		return false;
	}
	else
	{
		//TODO check for len after a lot of manipulation
		this->_clients[clientSocket].request.bodySize += requestLen;
		if (this->_clients[clientSocket].request.requestLine.empty())
			gettimeofday(&this->_clients[clientSocket].lastRequest, NULL);
		this->_clients[clientSocket].request.requestLine.append(buffer, requestLen);
	}

	if (this->_clients[clientSocket].request.requestLine.find("\r\n\r\n") != std::string::npos
		&& this->_clients[clientSocket].request.headerReady == false)
	{
		this->_parserMsg.ParseHeader(this->_clients[clientSocket].request);
		this->_clients[clientSocket].request.headerReady = true;
		if (std::atoi(this->_clients[clientSocket].request.getHeader("Content-Lenght").c_str()) == 0)
		{
			this->_clients[clientSocket].request.bodyReady = true;
			return true;
		}
	}
	if (this->_clients[clientSocket].request.headerReady == true)
	{
		body = this->_clients[clientSocket].request.requestLine.substr(this->_clients[clientSocket].request.requestLine.find("\r\n\r\n") + 4);
		std::string header = this->_clients[clientSocket].request.requestLine.substr(0, this->_clients[clientSocket].request.requestLine.find("\r\n\r\n") + 4);
		if ((this->_clients[clientSocket].request.contentSize - header.size()) == (unsigned long)std::atol(_clients[clientSocket].request.getHeader("Content-Length").c_str()))
		{
			std::cout << "CONTENT SIZE = " << this->_clients[clientSocket].request.contentSize - header.size() << " CONTENT LENGTH " << std::atoi(_clients[clientSocket].request.getHeader("Content-Length").c_str()) << '\n';
			this->_parserMsg.ParseBody(_clients[clientSocket].request);
			_clients[clientSocket].request.bodyReady = true;
		}
	}
    return true;
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
		throw std::logic_error("Error: accept() failed");

	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
		throw std::logic_error("Error: fcntl() failed");

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
int Kernel::CreateSocket(Server server)
{
	const int opt = 1;
	int listen_fd;
	struct sockaddr_in servaddr;
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		CloseSockets();
		throw std::logic_error("Error: socket() failed");
	}
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &opt, sizeof(opt)))
	{
		CloseSockets();
		throw std::logic_error("Error: setsockopt() failed");
	}

	if (fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		CloseSockets();
		throw std::logic_error("Error: fcntl() failed");
	}

	servaddr.sin_family = AF_INET;
	//NB! Need to FIX
	servaddr.sin_addr.s_addr = server.getHostAddr();
	servaddr.sin_port = htons(server.getPort());

	std::memset(servaddr.sin_zero, '\0', sizeof(servaddr.sin_zero));

	if (servaddr.sin_addr.s_addr == static_cast<in_addr_t>(-1))
	{
		CloseSockets();
		throw std::logic_error("Error: inet_addr: Invalid IP");
	}
	if (bind(listen_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
		CloseSockets();
		throw std::logic_error("Error: bind() failed");
	}
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
}

void Kernel::CreateEpoll()
{
	this->_epollFd = epoll_create1(0);
	if (this->_epollFd == -1)
		throw std::logic_error("Error: epoll_create1() couldn't init");
}

void Kernel::LoadKernel()
{
	this->_servers = this->_config.getServers();

	this->CreateEpoll();
	this->InitSocket();
	this->InitEpoll();
}

void Kernel::Run()
{
	std::string wait[] = {".   ", "..  ", "... ", "....", "   .", "  ..", " ...", "...."};
	int frames = 0;
	int nfds = 0;

	std::cout << "Start servers initialization..." << std::endl;
    this->LoadKernel();
	while(g_run)
	{
		errno = 0;
		nfds = epoll_wait(this->_epollFd, this->_eventsArray, MAX_EV, APP_TIMEOUT);
		if (errno == EINVAL || errno == EFAULT || errno == EBADFD)
			throw std::logic_error("Error: epoll_wait() failed");
		else if (errno == EINTR) // interrupted by ctrl+c
			g_run = false;

		//TODO: Main logic ->
		for (int i = 0; i < nfds; i++)
		{
			if (this->_eventsArray[i].events & EPOLLERR ||  this->_eventsArray[i].events & EPOLLHUP)
				throw std::logic_error("There is a bad event in events array");
			else if (this->_eventsArray[i].events & EPOLLIN && this->fdIsServer(this->_eventsArray[i].data.fd))
				this->AcceptNewClient(this->_eventsArray[i].data.fd);
			else if (this->_eventsArray[i].events & EPOLLIN)
				this->ClientRead(this->_eventsArray[i].data.fd);
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
	std::cout << std::endl << "Shutting down server..." << std::endl;
}

void Kernel::SetConfig(Config &config)
{
	this->_config = config;
}
