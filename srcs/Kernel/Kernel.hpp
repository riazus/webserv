#pragma once 

# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/sysmacros.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/select.h>
# include <sys/poll.h>
# include <sys/epoll.h>
# include <iostream>
# include <iomanip>
# include <sstream>
# include <cstring>
# include <cstdlib>
# include <limits>
# include <cctype>
# include <fstream>
# include <string>
# include <stdexcept>
# include <map>
# include <set>
# include <vector>
# include <algorithm>
# include <iterator>
# include <list>
# include <netinet/in.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <ostream>
// SHORTCUT
# define BUFFER_SIZE 4096
# define MAX_EV 4096
# define MAX_CLIENTS 1000
# define CLIENT_TIMEOUT 10
# define APP_TIMEOUT 200
// For more comfort
# define stringVector	std::vector<std::string>
# define intVector      std::vector<int>

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
        int         _socketFd;
        struct sockaddr_in _servaddr;

        //epoll vars
        int         _epollFd;
        struct epoll_event _event;
        struct epoll_event _eventsArray[MAX_EV];


        void    LoadKernel();
        void    CreateEpoll();
        void    InitEpoll();
        int     CreateSocket();
        void    ListenConnections();
        void    CloseSockets();
        std::string getResponse(std::string buffer);

};