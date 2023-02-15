#ifndef INCLUDES_HPP
# define INCLUDES_HPP

//SYSTEM
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
# include <stdlib.h>

// SHORTCUT
# define BUFFER_SIZE 4096
# define MAX_EV 4096
# define MAX_CLIENTS 1000
# define CLIENT_TIMEOUT 10
# define APP_TIMEOUT 200
# define DEFAULT "./configs/error_pages.conf"

# define IS_A_DIRECTORY 2
# define IS_A_FILE 1
# define IS_SOMETHING_ELSE 0

// For more comfort
# define stringVector	    std::vector<std::string>
# define intVector          std::vector<int>
# define serverVector       std::vector<Server>
# define mapClient          std::map<int,Client>
# define mapString          std::map<std::string,std::string>
# define mapStringVectorInt std::map<std::string,std::vector<int>>
# define mapError           std::map<int,std::string>
# define stringSet          std::set<std::string>
# define methodMap			std::map<std::string, void (Response::*)(void)>
# define pairString         std::pair<std::string,std::string>


# define CHARTOSKIP "; \t"


#endif