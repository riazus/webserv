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

bool Response::getIsValid() const
{
    return this->_isValid;
}

void Response::setResponse(std::string response)
{
	this->_response = response;
}

void Response::getMethod()
{

}

void Response::postMethod()
{

}

void Response::deleteMethod()
{
	
}

void Response::resetResponse(ResponseBody &responseBody)
{
	this->_code = responseBody.getCode();
	this->_header = "";
	this->_body = "";
	this->initErrorMap();
	this->initDirectories();
	this->_responseBody = &responseBody;
	this->_isValid = false;
}
// Creat and append header with body
void Response::createHeader()
{
	stringVector tmp(this->_responseBody->getAllow());

	if (this->_code == 405)
	{
		for(int i = 0; i < tmp.size(); i++)
		{
			if (i+1 == tmp.size())
				this->_directives["Allow"] += tmp[i];
			else
				this->_directives["Allow"] += tmp[i] + " ";
		}
	}
	this->_directives["Content-Language"] = this->_responseBody->getLanguage();
	if (this->_code != 404)
		this->_directives["Content-Location"] = this->_responseBody->getContentLocation();
	this->_directives["Date"] = getDate();
	this->_directives["Last-Modified"] = getLastMod(this->_responseBody->getContentLocation());
	if (_code == 503 || _code == 429 || _code == 301)
		this->_directives["Retry-After"] = "2";
	if (_code == 401)
		this->_directives["WWW-Authenticate"] = "Basic realm=\"Access requires authentification\" charset=\"UTF-8\"";
	this->_header += "HTTP 1.1 " + ft_itoa(_code);
	this->_header += " " + this->_status[_code] + "\r\n";
	for (mapString::const_iterator i = this->_directives.begin(); i != this->_directives.end(); i++)
		if (i->second != "")
			this->_header+= i->first + ": " + i->second + "\r\n";
	this->_response = this->_header + "\r\n" + this->_body;
	this->_isValid = true;
}

void Response::initErrorMap()
{
	this->_status[100] = "Continue";
	this->_status[200] = "OK";
	this->_status[201] = "Created";
	this->_status[204] = "No Content";
	this->_status[301] = "Moved Permanently";
	this->_status[308] = "Permanent Redirect";
	this->_status[400] = "Bad Request";
	this->_status[403] = "Forbidden";
	this->_status[404] = "Not Found";
	this->_status[405] = "Method Not Allowed";
	this->_status[408] = "Request Timeout";
	this->_status[413] = "Payload Too Large";
	this->_status[500] = "Internal Server Error";
}

void Response::initDirectories()
{
	this->_directives["Allow"] = "";
	this->_directives["Content-Language"] = "";
	this->_directives["Content-Length"] = "";
	this->_directives["Content-Location"] = "";
	this->_directives["Content-Type"] = "";
	this->_directives["Date"] = "";
	this->_directives["Last-Modified"] = "";
	this->_directives["Connection"] = "";
	this->_directives["Location"] = "";
	this->_directives["Retry-After"] = "";
	this->_directives["Server"] = "Webserv";
	this->_directives["Transfer-Encoding"] = "identity";
	this->_directives["WwwAuthenticate"] = "";
	this->_directives["Set-Cookie"] = "";
}

void Response::initResponseProcess()
{
	stringSet tmp(this->_responseBody->getAllowMethod());

	if (this->_responseBody->getCookie("user_id") == "")
	{
		std::cout << this->_responseBody->getCookie("user_id") << std::endl;
		this->UserId = gen_random(32);
		this->_directives["Set-Cookie"] = "user_id=" + this->UserId;
	}

	/*if (this->_responseBody->gettLocation().getReturn().first != "")
	{
		if(_config.getRequest().getMethod() != "POST")
			_code = std::atoi(_config.getLocation().getReturn().first.c_str());
		else
			_code = 308;
		_directives["Location"] = _config.getLocation().getReturn().second;
		createHeader();
		return ;
	}*/
	
	if (tmp.find(this->_responseBody->getRequest()->getMethod()) == tmp.end()) // this method doesn't exists
		this->_code = 405;
	else if (this->_responseBody->getClientBodyBufferSize() < this->_responseBody->getRequest()->bodySize)
		this->_code = 413;
	
	if (this->_code == 408)
	{
		this->_directives["Connection"] = "close";
		this->createHeader();
	}
	else if (this->_code != 200)
	{
		this->_directives["Content-Length"] = this->readFile(this->_code);
		this->createHeader();
	}
	else if (this->_method.find(this->_responseBody->getRequest()->getMethod()) != this->_method.end())
		(this->*Response::_method[this->_responseBody->getRequest()->getMethod()])();
}

static int		checkPath(const std::string &path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
			return IS_A_DIRECTORY;
		else if (s.st_mode & S_IFREG)
			return IS_A_FILE;
		else
			return IS_SOMETHING_ELSE;
	}
	else
		return IS_SOMETHING_ELSE;
}

std::string Response::readFile(int code)
{
	std::string	path(this->_responseBody->getErrorPath(code));
	std::ofstream		file;
	std::stringstream	buffer;

	if (checkPath(path) == IS_A_FILE)
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			this->_code = 404;
			this->_directives["Content-Type"] = "text/html";
			this->_body = this->getErrorFileBody(404);
			return (ft_itoa(_body.size()));
		}
		buffer << file.rdbuf();
		file.close();
		this->_directives["Content-Type"] = "text/html";
		this->_body = buffer.str();
		//return (ft_itoa(_body.size()));
	}
	else
	{
		this->_code = 404;
		this->_directives["Content-Type"] = "text/html";
		this->_body = this->getErrorFileBody(404);
		//return (ft_itoa(_body.size()));
	}
	return (ft_itoa(_body.size()));
}

std::string Response::readFile(std::string path)
{
	return std::string();
}

std::string Response::getErrorFileBody(int code)
{
    std::string	path("./website/error_pages/" + ft_itoa(code) + ".html");
	std::ofstream		file;
	std::stringstream	buffer;

	file.open(path.c_str(), std::ifstream::in);
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

methodMap Response::initMethods(void)
{
    methodMap map;

	map["GET"] = &Response::getMethod;
	map["DELETE"] = &Response::deleteMethod;
	map["POST"] = &Response::postMethod;

	return map;
}

methodMap Response::_method = Response::initMethods();

static std::string	getDate(void)
{
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	return (std::string(buffer));
}

static std::string	getLastMod(const std::string& path)
{
	char			buffer[100];
	struct stat		stats;
	struct tm		*tm;

	if (stat(path.c_str(), &stats) == 0)
	{
		tm = gmtime(&stats.st_mtime);
		strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
		return (std::string(buffer));
	}
	return ("");
}

static std::string	ft_itoa(int nb)
{
	std::string s;
	std::stringstream out;
	out << nb;
	s = out.str();
	return s;
}

static std::string gen_random(const int len)
{
	static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string tmp_s;
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i)
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

	return tmp_s;
}