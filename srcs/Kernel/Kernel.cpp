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

void Kernel::HandleEpollError(int eventPollFd)
{
	if (fdIsServer(eventPollFd))
	{
		this->CloseSockets();
		throw std::logic_error("There is a bad event in events array");
	}
	epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, eventPollFd, NULL);
	close(eventPollFd);
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

	for (serverVector::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (!(client.request.getServer().getHostName().empty()))
		{
			stringVector serverNames = it->getServerName();
			for(int i = 0; i < serverNames.size(); i++)
			{
				if (client.request.getServer().getHostName() == serverNames[i] && it->getPort() == client.request.getServer().getPort())
				{
					foundAConf = true;
					rightServer = *it;
					break ;
				}
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
		//std::cout << "RIGHT SERVER NOT FOUND!!" << std::endl;
	}
	client.setServer(rightServer);
}

void displayClientInfo(Client &client)
{
	char buffer[100];
	struct tm *tm = gmtime(&client.lastRequest.tv_sec);
	strftime(buffer, 100, "%F - %T", tm);
	
	std::cout << std::endl << "CLIENT: " << buffer <<" | "<< client.request.getMethod()+" " << client.request.getPath()+" ";
	std::cout << client.request.getDomainName()+" " << client.getServer().getPort() << std::endl;
	//std::cout << client.request.requestLine << std::endl;
}

void displayServerInfo(Client &client, Response &response)
{
	char			buffer[100];
	struct tm		*tm;
	std::string	resp = response.getResponse();
	std::string firstLine = resp.substr(0, resp.find_first_of('\n') - 1);

	tm = gmtime(&client.lastRequest.tv_sec);
	strftime(buffer, 100, "%F - %T", tm);

	std::cout << "SERVER: " << buffer << " (" << get_time_diff(&client.lastRequest)+")" << " | " << firstLine+" ";
	std::cout << client.request.getDomainName()+" " << client.getServer().getPort() << std::endl;
	//std::cout << response.getResponse() << std::endl;
}

std::string	get_time_diff(struct timeval *last)
{
	if (last == 0)
		return "        - ";
	struct timeval end;
	gettimeofday(&end, NULL);

	size_t	usec = end.tv_usec - last->tv_usec;
	size_t	sec = end.tv_sec - last->tv_sec;
	bool	is_micro = false;
	std::stringstream ss;

	if (sec > 0)
	{
		ss << sec << " s";
	}
	else
	{
		if (usec >= 1000)
		{
			ss << usec / 1000 << " ms";
		}
		else
		{
			ss << usec << " μs";
			is_micro = true;
		}
	}

	std::string str = ss.str();
	if (str.size() > 10)
		return "eternity..";
	return str;
}

void Kernel::ClientWrite(int eventPollFd)
{
	std::string response;

	this->getServerForClient(this->_clients[eventPollFd]);
	this->_parserMsg.ParseResponse(this->_clients[eventPollFd].responseBody, this->_clients[eventPollFd].request, this->_clients[eventPollFd].getServer());
	this->_clients[eventPollFd].response.resetResponse(this->_clients[eventPollFd].responseBody);

	this->_clients[eventPollFd].response.initResponseProcess();

	if (this->_clients[eventPollFd].UserId == "")
		this->_clients[eventPollFd].UserId = this->_clients[eventPollFd].response.UserId;

	if (this->_clients[eventPollFd].response.getIsValid() == false)
		return ;
	
	response = this->_clients[eventPollFd].response.getResponse();
	
	if (write(eventPollFd, response.c_str(), response.size()) < 0)
		this->DeleteClient(eventPollFd);
	this->_clients[eventPollFd].hadResponse = true;

	displayClientInfo(this->_clients[eventPollFd]);
	displayServerInfo(this->_clients[eventPollFd], this->_clients[eventPollFd].response);

	this->_event.events = EPOLLIN;
	this->_event.data.fd = eventPollFd;
	epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, eventPollFd, &this->_event);
	this->_clients[eventPollFd].request.ResetRequest();
}

bool Kernel::ReadClientRequest(int clientSocket)
{
	char buffer[BUFFER_SIZE + 1];
	std::string body("");
	ssize_t requestLen = read(clientSocket, buffer, BUFFER_SIZE);

	//std::cout << buffer << std::endl;
	if (requestLen == -1)
	{
		this->DeleteClient(clientSocket);
		throw std::logic_error("Error: read Client's request failed");
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
		this->_clients[clientSocket].request.contentSize += requestLen;
		if (this->_clients[clientSocket].request.requestLine.empty())
			gettimeofday(&this->_clients[clientSocket].lastRequest, NULL);
		this->_clients[clientSocket].request.requestLine.append(buffer, requestLen);
	}

	if (this->_clients[clientSocket].request.requestLine.find("\r\n\r\n") != std::string::npos
		&& this->_clients[clientSocket].request.headerReady == false)
	{
		this->_parserMsg.ParseHeader(this->_clients[clientSocket].request);
		this->_clients[clientSocket].request.headerReady = true;
		if (std::atoi(this->_clients[clientSocket].request.getHeader("Content-Length").c_str()) == 0)
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
	if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, new_socket, &this->_event) == -1)
		throw std::logic_error("Error: epoll_ctl() failed");

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

void Kernel::HandleClientTimeout()
{
	for(mapClient::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (CheckTimeout((*it).second.lastRequest))
		{
			if ((*it).second.request.requestLine.empty() || (*it).second.hadResponse)
			{
				std::cout << "REMOVE CLIENT" << std::endl;
				this->DeleteClient((*it).first);
				return this->HandleClientTimeout();
			}
			else
			{
				(*it).second.request.setCode(408);
				this->_event.events = EPOLLOUT;
				this->_event.data.fd = (*it).first;
				epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, (*it).first, &this->_event);
			}
		}
	}
}

bool Kernel::CheckTimeout(timeval last)
{
	struct timeval now;
	gettimeofday(&now, NULL);

	if ((now.tv_sec - last.tv_sec) >= CLIENT_TIMEOUT)
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

void Kernel::SetConfig(Config &config)
{
	this->_config = config;
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

		for (int i = 0; i < nfds; i++)
		{
			if (this->_eventsArray[i].events & EPOLLERR ||  this->_eventsArray[i].events & EPOLLHUP)
				this->HandleEpollError(this->_eventsArray[i].data.fd);
			else if (this->_eventsArray[i].events & EPOLLIN && this->fdIsServer(this->_eventsArray[i].data.fd))
				this->AcceptNewClient(this->_eventsArray[i].data.fd);
			else if (this->_eventsArray[i].events & EPOLLIN)
				this->ClientRead(this->_eventsArray[i].data.fd);
			else if (this->_eventsArray[i].events & EPOLLOUT)
				this->ClientWrite(this->_eventsArray[i].data.fd);
		}

		if (nfds == 0)
		{
			std::cout << "\r" << wait[(frames++ % 8)] << " Waiting for new connections " << std::flush;
			this->HandleClientTimeout();
		}
	}
	this->CloseSockets();
	std::cout << std::endl << "Shutting down server..." << std::endl;
}