#include "Request.hpp"

Request::Request(void)
{
}

Request::Request(Request const & src)
{
	*this = src;
}

Request &Request::operator=(Request const &rhs)
{
	return *this;
}

Request::~Request(void)
{
}

