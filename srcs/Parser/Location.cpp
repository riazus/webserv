#include "Location.hpp"

Location::Location()
{
}

Location::Location(const Location &location)
{
	*this = location;
}

Location &Location::operator=(const Location &location)
{
	this->name = location.name;
	return (*this);
}

void Location::setPath(std::string path)
{
	this->path = path;
}

std::string Location::getPath()
{
	return (this->path);
}

void Location::setRoot(std::string root)
{
	this->root = root;
}

std::string Location::getRoot()
{
	return (this->root);
}

void Location::setIndex(std::string index)
{
	this->index = index;
}

std::string Location::getIndex()
{
	return (this->index);
}

void Location::setAutoindex(int autoindex)
{
	this->autoindex = autoindex;
}

int Location::getAutoindex()
{
	return (this->autoindex);
}

void Location::setMethods(std::string method)
{
	this->methods.push_back(method);
}

std::vector<std::string> Location::getMethods()
{
	return (this->methods);
}

void Location::location_error(std::string error)
{
	if (error != "")
		std::cout << "Error: " << error << std::endl;
	throw Location::InvalidLocationException();
}

void	Location::is_valid()
{
	std::string error;
	if (getPath() == "")
	{
		error = (getPath() + " path is not set");
		location_error(error);
	}
	std::vector<std::string> methods = getMethods();
	if (methods.size() == 0)
	{
		error = (getPath() + " allowed methods are not set");
		location_error(error);
	}
	if (getRoot() == "")
	{
		error = (getPath() + " location_root is not set");
		location_error(error);
	}
}

const char* Location::InvalidLocationException::what() const throw()
{
	return "InvalidLocationException: Invalid location configuration!";
}

Location::~Location()
{
}