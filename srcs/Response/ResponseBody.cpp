#include "ResponseBody.hpp"

ResponseBody::ResponseBody(void)
{
}

ResponseBody::ResponseBody(ResponseBody const & src)
{
	*this = src;
}

ResponseBody &ResponseBody::operator=(ResponseBody const &rhs)
{
	return *this;
}

ResponseBody::~ResponseBody(void)
{
}

void ResponseBody::setContentLocation(std::string contentLocation)
{
	this->_contentLocation = contentLocation;
}

void ResponseBody::setContent(std::string content)
{
	this->_content = content;
}

std::string ResponseBody::getContent() const
{
	return this->_content;
}