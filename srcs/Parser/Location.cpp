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



Location::~Location()
{
}