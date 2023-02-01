#include "Response.hpp"

Response::Response(void)
{
}

Response::Response(Response const & src)
{
	*this = src;
}

Response &Response::operator=(Response const &rhs)
{
	return *this;
}

Response::~Response(void)
{
}

std::string Response::getResponse() const
{
	return this->_response;
}

void Response::setResponse(std::string response)
{
	this->_response = response;
}