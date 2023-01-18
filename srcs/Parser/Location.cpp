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

void Location::setName(std::string name)
{
	this->name = name;
}

std::string Location::getName()
{
	return (this->name);
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

void Location::setAutoindex(std::string autoindex)
{
	this->autoindex = autoindex;
}

std::string Location::getAutoindex()
{
	return (this->autoindex);
}

void Location::setMethods(std::string method)
{
	this->methods.push_back(method);
}

std::vector<std::string> getMethods()
{
	return (this->methods)
}


Location::~Location()
{
}