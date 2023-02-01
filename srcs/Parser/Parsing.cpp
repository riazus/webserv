#include "../includes.hpp"
#include "../Server/Server.hpp"
#include "../Config/Config.hpp"
#include "../Parser/Location.hpp"

void config_error(std::string error)
{
	if (error != "")
		std::cout << error << std::endl;
	throw Config::ConfigFileContentException();
}