#ifndef KERNEL_H
# define KERNEL_H

# include "../includes.hpp"
# include "../Server/Server.hpp"
# include "../Client/Client.hpp"
# include "../Parser/ParseMsg.hpp"

class Kernel
{
    public :
        Kernel();
        Kernel(Kernel &copy);
        Kernel &operator=(Kernel &copy);
        ~Kernel();

        void    Run();
        void    SetConfig(Config &config);

    private :
        //epoll vars
        int                 _epollFd;
        struct epoll_event  _event;
        struct epoll_event  _eventsArray[MAX_EV];

        intVector   _serverFds;
        serverVector _servers;
        
        mapClient   _clients;

        Config      _config;
        ParseMsg    _parserMsg;

        void    LoadKernel();
        void    CreateEpoll();
        void    InitEpoll();
        void    InitSocket();
        int     CreateSocket(Server server);
        void    CloseSockets();
        void    HandleEpollError(int eventPollFd);
        void    AcceptNewClient(int eventPollFd);
        bool    fdIsServer(int eventPollFd);
        void    HandleClientTimeout();
        bool    CheckTimeout(struct timeval last);

        //Handle read/write methods
        void    ClientWrite(int eventPollFd);
        void    ClientRead(int eventPollFd);
        bool    ReadClientRequest(int socketFd);
        void    DeleteClient(int socketFd);
        void    getServerForClient(Client &client);

        std::string getResponse();

};
void        displayClientInfo(Client &client);
void        displayServerInfo(Client &client, Response &response);
std::string	get_time_diff(struct timeval *last);
#endif