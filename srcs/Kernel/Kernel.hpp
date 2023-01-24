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

    private :
        //intVector   _socketFd;
        int                 _socketFd;
        struct sockaddr_in  _servaddr;

        //epoll vars
        int                 _epollFd;
        struct epoll_event  _event;
        struct epoll_event  _eventsArray[MAX_EV];

        // Now we use only ONE server:
        intVector   _serverFd;
        
        mapClient   _clients;


        void    LoadKernel();
        void    CreateEpoll();
        void    InitEpoll();
        int     CreateSocket();
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