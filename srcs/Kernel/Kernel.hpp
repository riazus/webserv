#ifndef KERNEL_H
# define KERNEL_H

# include "../Client/Client.hpp"
# include "../Request/Request.hpp"
# include "../Response/Response.hpp"

class Kernel
{
    public :
        Kernel();
        Kernel(Kernel &copy);
        Kernel &operator=(Kernel &copy);
        ~Kernel();

        void    Run();
        void    SetConfig(Config *config);

    private :
        //epoll vars
        int                 _epollFd;
        struct epoll_event  _event;
        struct epoll_event  _eventsArray[MAX_EV];

        intVector   _serverFds;
        serverVector _servers;
        
        mapClient   _clients;

        Config      *_config;


        void    LoadKernel();
        void    CreateEpoll();
        void    InitEpoll();
        void    InitSocket();
        int     CreateSocket(Server *server);
        void    CloseSockets();
        void    AcceptNewClient(int eventPollFd);
        bool    fdIsServer(int eventPollFd);
        void    ClientWrite(int eventPollFd);
        void    ClientRead(int eventPollFd);

        //Handle read/write methods
        bool    ReadClientRequest(int socketFd);
        bool    WriteClientRequest(int socketFd);
        void    DeleteClient(int socketFd);

        std::string getResponse(std::string buffer);

};

#endif