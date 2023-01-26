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

void Request::ResetHeaders()
{
	_version = "";
	_method = "";
	_code = 200;
	_body = "";
	_port = 8080;
	_path = "";
	_query = "";
	_headers.clear();
	_headers["Accept-Charsets"] = "";
	_headers["Accept-Language"] = "";
	_headers["Allow"] = "";
	_headers["Auth-Scheme"] = "";
	_headers["Authorization"] = "";
	_headers["Content-Language"] = "";
	_headers["Content-Length"] = "";
	_headers["Content-Location"] = "";
	_headers["Content-Type"] = "";
	_headers["Date"] = "";
	_headers["Host"] = "";
	_headers["Last-Modified"] = "";
	_headers["Location"] = "";
	_headers["Referer"] = "";
	_headers["Retry-After"] = "";
	_headers["Server"] = "";
	_headers["Transfer-Encoding"] = "";
	_headers["User-Agent"] = "";
	_headers["Cookie"] = "";
	_headers["Www-Authenticate"] = "";
	_headers["Connection"] = "Keep-Alive";
}

int Request::getCode() const
{
	return this->_code;
}

void Request::setCode(int code)
{
	this->_code = code;
}

std::string Request::getHeader(std::string header)
{
    if (this->_headers.count(header))
		return this->_headers[header];
	else
		return "";
}

std::string Request::getMethod() const
{
    return this->_method;
}

mapString Request::getHeaders() const
{
    return this->_headers;
}

void Request::setBody(std::string body)
{
	this->_body = body;
}

void Request::setMethod(std::string method)
{
	this->_method = method;
}

void Request::setPath(std::string path)
{
	this->_path = path;
}

void Request::setVersion(std::string version)
{
	this->_version = version;
}

void Request::setHeader(std::string token, std::string value)
{
	this->_headers[token] = value;
}

void Request::setEnvForCgi(std::string token, std::string value)
{
	std::transform(token.begin(), token.end(), token.begin(), ::toupper);
	for (size_t i = 0 ; i < token.size() ; i++)
		if (token[i] == '-')
			token[i] = '_';
	this->_env_for_cgi["HTTP_" + token] = value;
}

void Request::setNetwork(std::string ip)
{
	std::string	host;
	size_t		colons = ip.find(":");

	host = ip.substr(0, colons);
	if (host == "localhost")
		this->_server.setHostAddr(inet_addr("127.0.0.1"));
	else if (host.find_first_not_of("0123456789.") != std::string::npos)
		this->_server.setHostName(host.c_str());
	else
		this->_server.setHostAddr(inet_addr(host.c_str()));
	this->_server.setPort(std::atoi(ip.substr(++colons).c_str()));
}
