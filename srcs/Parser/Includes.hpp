#ifndef INCLUDES_HPP
# define INCLUDES_HPP

#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>



#include "Server.hpp"
#include "Location.hpp"
#include "Config.hpp"

# define CHARTOSKIP "; \t"

class Server;
class Location;

	std::vector<std::string> ft_split(const std::string &str, const std::string &charset);	
	Server *parse_server(std::vector<std::string> config, int *line_count);
	bool is_integer(const std::string &s);

#endif