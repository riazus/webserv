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

void ResponseBody::setRequest(Request &request)
{
	this->_request = request;
}

void ResponseBody::setServer(Server server)
{
	this->_server = server;
}

void ResponseBody::setLocation(Location location)
{
	this->_location = location;
}

void ResponseBody::setLocationPath(std::string path)
{
	this->_locationPath = path;
}

void ResponseBody::setLocationFile(std::string path)
{
	this->_locationFile = path;
}

void ResponseBody::setClientBodyBufferSize(size_t size)
{
	this->_clientBodyBufferSize = size;
}

void ResponseBody::setLanguage(std::string lang)
{
	this->_acceptLanguage = lang;
}

void ResponseBody::setIndex(std::string index)
{
	this->_index = index;
}

void ResponseBody::setCode(int code)
{
	this->_code = code;
}

void ResponseBody::setContentLocation(std::string contentLocation)
{
	this->_contentLocation = contentLocation;
}

void ResponseBody::setContent(std::string content)
{
	this->_content = content;
}

void ResponseBody::setAllowMethod(stringVector allow)
{
	for (stringVector::const_iterator i = allow.begin(); i != allow.end(); i++)
		this->_allowMethod.insert(*i);
	this->_allow = allow;
}

void ResponseBody::setAutoIndex(bool autoindex)
{
	this->_autoIndex = autoindex;
}

std::string ResponseBody::getCookie(std::string key) const
{
    if (this->_cookies.count(key))
		return this->_cookies.at(key);
	else
		return "";
}

int ResponseBody::getCode() const
{
    return this->_code;
}

Request ResponseBody::getRequest() const
{
    return this->_request;
}

Location ResponseBody::getLocation(void) const
{
    return this->_location;
}

std::string ResponseBody::getErrorPath(int code) const
{
	if (this->_errorPage.find(code) == this->_errorPage.end())
		return ("");
	return (this->_errorPage.at(code));
}

size_t ResponseBody::getClientBodyBufferSize(void) const
{
    return this->_clientBodyBufferSize;
}

stringSet ResponseBody::getAllowMethod() const
{
    return this->_allowMethod;
}

stringVector ResponseBody::getAllow() const
{
    return this->_allow;
}

std::string ResponseBody::getLanguage() const
{
    return this->_acceptLanguage;
}

std::string ResponseBody::getContentLocation() const
{
    return this->_contentLocation;
}

std::string ResponseBody::getContent() const
{
	return this->_content;
}

bool ResponseBody::getAutoIndex(void) const
{
	return this->_autoIndex;
}

std::string ResponseBody::getIndex(void) const
{
	return this->_index;
}
