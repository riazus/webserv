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
	this->_request = rhs._request;
	this->_acceptLanguage = rhs._acceptLanguage;
	this->_allow = rhs._allow;
	this->_allowMethod = rhs._allowMethod;
	this->_autoIndex = rhs._autoIndex;
	this->_cgiPass = rhs._cgiPass;
	this->_clientBodyBufferSize = rhs._clientBodyBufferSize;
	this->_code = rhs._code;
	this->_content = rhs._content;
	this->_contentLocation = rhs._contentLocation;
	this->_cookies = rhs._cookies;
	this->_errorPage = rhs._errorPage;
	this->_index = rhs._index;
	this->_location = rhs._location;
	this->_locationFile = rhs._locationFile;
	this->_locationPath = rhs._locationPath;
	this->_request = rhs._request;
	this->_server = rhs._server;
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

void ResponseBody::setLocation(Server location)
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

void ResponseBody::setErrorMap(mapError map)
{
	this->_errorPage = map;
}

void ResponseBody::setClientBodyBufferSize(size_t size)
{
	this->_clientBodyBufferSize = size;
}

void ResponseBody::setCgiPass(std::string cgiPass)
{
	this->_cgiPass = cgiPass;
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

void ResponseBody::setCookies(mapString cookies)
{
	this->_cookies = cookies;
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

mapString ResponseBody::getCookies(void) const
{
    return this->_cookies;
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

Server ResponseBody::getServer(void) const
{
    return this->_server;
}

Server ResponseBody::getLocation(void) const
{
    return this->_location;
}

std::string	ResponseBody::getLocationPath(void) const
{
	return this->_locationPath;
}

std::string	ResponseBody::getLocationFile(void) const
{
	return this->_locationFile;
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

std::string ResponseBody::getCgiPass() const
{
	return this->_cgiPass;
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
