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
	this->_responseBody = rhs._responseBody;
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

int isCgi(std::string path)
{
	if (path.find(".py") != std::string::npos)
		return 1;
	if (path.find(".php") != std::string::npos)
		return 2;
	if (path.find(".sh") != std::string::npos)
		return 3;
	else
		return 0;
}

std::string Response::execCgi(std::string path, int file_format)
{
	std::string format;
	pid_t pid;
	char	buffer[2];
	FILE	*tmpIn = tmpfile();
	FILE	*tmpOut = tmpfile();
	int		in = fileno(tmpIn);
	int		out = fileno(tmpOut);
	int		status;

	lseek(in, 0, SEEK_SET);

	if (file_format == 1)
		format = "python3";
	if (file_format == 2)
		format = "php";
	if (file_format == 3)
		format = "sh";
	pid = fork();
	if (pid == 0)
	{
		dup2(in, STDIN_FILENO);
		dup2(out, STDOUT_FILENO);

		execlp(format.c_str(), format.c_str(), path.c_str(), (char*) NULL);
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) != 0)
	{
		this->_code = 500;
		this->_directives["Content-Type"] = "text/html";
		this->_body = this->getErrorFileBody(500);
		_directives["Content-Length"] = ft_itoa(_body.size());
		return ft_itoa(_body.size());
	}
	lseek(out, 0, SEEK_SET);
	int ret = 1;
	std::string buffer_str;
	while (ret > 0)
	{
		ret = read(out, buffer, 1);
		buffer[ret] = '\0';
		buffer_str += buffer;
	}
	fclose(tmpIn);
	fclose(tmpOut);
	close(in);
	close(out);
	this->_body = buffer_str;
	return (ft_itoa(_body.size()));
}


void Response::getMethod()
{
	if(checkPath(this->_responseBody.getContentLocation()) == IS_A_DIRECTORY &&
		this->_responseBody.getContentLocation() == (this->_responseBody.getLocation().getRoot() + this->_responseBody.getLocation().getAlias()))
	{
		this->_responseBody.setContent(this->_responseBody.getContentLocation() + "/" + this->_responseBody.getIndex());
		if (this->_responseBody.getIndex() != "")
			this->_responseBody.setAutoIndex(false);
	}
	else
		this->_responseBody.setContent(this->_responseBody.getContentLocation());
	int file_format = isCgi(this->_responseBody.getContent());
	if(file_format != 0)
	{
		struct stat buffer; 
		if (stat (this->_responseBody.getContent().c_str(), &buffer) == 0)
			_directives["Content-Length"] = execCgi(this->_responseBody.getContent(), file_format);
		else
		{
			this->_code = 404;
			this->_directives["Content-Type"] = "text/html";
			this->_body = this->getErrorFileBody(404);
			_directives["Content-Length"] = ft_itoa(_body.size());
		}
	}
	else
		_directives["Content-Length"] = readFile(this->_responseBody.getContent());
	
	this->createHeader();
}

void Response::postMethod()
{
	std::cout << "POST HERE " << std::endl;	
	if (checkPath(this->_responseBody.getContentLocation()) == IS_A_DIRECTORY &&
	this->_responseBody.getContentLocation() == (this->_responseBody.getLocation().getRoot() + this->_responseBody.getLocation().getAlias()))
		this->_responseBody.setContent(this->_responseBody.getContentLocation() + "/" + this->_responseBody.getIndex());
	else
		this->_responseBody.setContent(this->_responseBody.getContentLocation());
	if (!this->_responseBody.getCgiPass().empty())
	{
		Cgi cgi;
		std::string tmpBody;
		cgi.initCgiData(this->_responseBody);
		cgi.setEnv();
		tmpBody = cgi.execute();
		parseCgiBody(tmpBody);
	}
	else
		this->_code = 204;

	if (this->_code == 200)
	{
		std::string host = this->_responseBody.getServer().getIpAdress();
		std::string port = ft_itoa(this->_responseBody.getServer().getPort());
		std::string location = host + ":" + port + "/cgi-bin/tmp";
		_code = 201;
		_directives["Location"] = location;
	}
	this->createHeader();
}

void Response::deleteMethod()
{
	std::string path(this->_responseBody.getContentLocation());
	if (checkPath(path) != IS_SOMETHING_ELSE)
	{
		if (checkWritePermission(path) && remove(path.c_str()) == 0)
			this->_code = 204;
		else
			this->_code = 403;
	}
	else
		this->_code = 404;
	if (this->_code != 204)
		this->_directives["Content-Length"] = readFile(this->_code);
	createHeader();
}

void Response::resetResponse(ResponseBody &responseBody)
{
	this->_code = responseBody.getCode();
	this->_header = "";
	this->_body = "";
	this->initErrorMap();
	this->initDirectories();
	this->_responseBody = responseBody;
	this->_isValid = false;
}
// Creat and append header with body
void Response::createHeader()
{
	stringVector tmp(this->_responseBody.getAllow());

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
	this->_directives["Content-Language"] = this->_responseBody.getLanguage();
	if (this->_code != 404)
		this->_directives["Content-Location"] = this->_responseBody.getContentLocation();
	this->_directives["Date"] = getDate();
	this->_directives["Last-Modified"] = getLastMod(this->_responseBody.getContentLocation());
	if (_code == 503 || _code == 429 || _code == 301)
		this->_directives["Retry-After"] = "2";
	if (_code == 401)
		this->_directives["WWW-Authenticate"] = "Basic realm=\"Access requires authentification\" charset=\"UTF-8\"";
	this->_header += "HTTP/1.1 " + ft_itoa(_code);
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
	stringSet tmp(this->_responseBody.getAllowMethod());

	if (this->_responseBody.getCookie("user_id") == "")
	{
		std::cout << this->_responseBody.getCookie("user_id") << std::endl;
		this->UserId = gen_random(32);
		this->_directives["Set-Cookie"] = "user_id=" + this->UserId;
	}

	if (this->_responseBody.getLocation().getReturn().first != "")
	{
		std::cout << "LOL" << std::endl;
		if(this->_responseBody.getRequest().getMethod() != "POST")
			_code = std::atoi(this->_responseBody.getLocation().getReturn().first.c_str());
		else
			_code = 308;
		_directives["Location"] = this->_responseBody.getLocation().getReturn().second;
		std::cout << "CODE: " << _code << std::endl;
		createHeader();
		return ;
	}
	if (tmp.find(this->_responseBody.getRequest().getMethod()) == tmp.end()) // this method doesn't exists
		this->_code = 405;
	else if (this->_responseBody.getClientBodyBufferSize() < this->_responseBody.getRequest().bodySize)
	{
		std::cout << this->_responseBody.getClientBodyBufferSize() << " :serverMaxBuff < requestBuff: " << this->_responseBody.getRequest().bodySize << std::endl;
		this->_code = 413;
	}
	
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
	else if (this->_method.find(this->_responseBody.getRequest().getMethod()) != this->_method.end())
		(this->*Response::_method[this->_responseBody.getRequest().getMethod()])(); //start execute found method
}

std::string Response::readFile(int code)
{
	std::string	path(this->_responseBody.getErrorPath(code));
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
	}
	else
	{
		this->_code = 404;
		this->_directives["Content-Type"] = "text/html";
		this->_body = this->getErrorFileBody(404);
	}
	return (ft_itoa(_body.size()));
}

std::string Response::readFile(std::string path)
{
	std::ofstream		file;
	std::stringstream	buffer;
	if (checkPath(this->_responseBody.getContentLocation()) == IS_A_DIRECTORY && this->_responseBody.getAutoIndex())
		_body = createAutoindexPage(this->_responseBody.getContentLocation(), this->_responseBody.getRequest().getPath());
	else if (checkPath(path) == IS_A_FILE)
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			_code = 404;
			return (readFile(_code));
		}
		buffer << file.rdbuf();
		file.close();
		_directives["Content-Type"] = findType(path);
		if (checkReadPermission(path) == 0)
		{
			_code = 403;
			return (readFile(_code));
		}
		_body = buffer.str();
	}
	else
	{
		_code = 404;
		readFile(_code);
	}
	return (ft_itoa(_body.size()));
}

mapString Response::initType()
{
	mapString tmp;

	tmp["acc"] = "audio/aac";
	tmp["abw"] = "application/x-abiword";
	tmp["avi"] = "video/x-msvideo";
	tmp["bin"] = "application/octet-stream";
	tmp["bmp"] = "image/bmp";
	tmp["bz"] = "application/x-bzip";
	tmp["bz2"] = "application/x-bzip2";
	tmp["csh"] = "application/x-csh";
	tmp["css"] = "text/css";
	tmp["csv"] = "text/csv";
	tmp["doc"] = "application/msword";
	tmp["gif"] = "image/gif";
	tmp["htm"] = "text/html";
	tmp["html"] = "text/html";
	tmp["ico"] = "image/x-icon";
	tmp["ics"] = "text/calendar";
	tmp["jar"] = "application/java-archive";
	tmp["jpeg"] = "image/jpeg";
	tmp["jpg"] = "image/jpeg";
	tmp["js"] = "application/javascript";
	tmp["json"] = "application/json";
	tmp["mid"] = "audio/midi";
	tmp["midi"] = "audio/midi";
	tmp["mpeg"] = "video/mpeg";
	tmp["oga"] = "audio/ogg";
	tmp["ogv"] = "video/ogg";
	tmp["ogx"] = "application/ogg";
	tmp["otf"] = "font/otf";
	tmp["png"] = "image/png";
	tmp["pdf"] = "application/pdf";
	tmp["rar"] = "application/x-rar-compressed";
	tmp["rtf"] = "application/rtf";
	tmp["sh"] = "application/x-sh";
	tmp["svg"] = "image/svg+xml";
	tmp["tif"] = "image/tiff";
	tmp["tiff"] = "image/tiff";
	tmp["ts"] = "application/typescript";
	tmp["ttf"] = "font/ttf";
	tmp["vsd"] = "application/vnd.visio";
	tmp["wav"] = "audio/x-wav";
	tmp["weba"] = "audio/webm";
	tmp["webm"] = "video/webm";
	tmp["webp"] = "image/webm";
	tmp["woff"] = "font/woff";
	tmp["woff2"] = "font/woff2";
	tmp["xhtml"] = "application/xhtml+xml";
	tmp["xml"] = "application/xml";
	tmp["zip"] = "application/zip";

	return (tmp);
}

mapString Response::_typeMap = Response::initType();

std::string	Response::findType(std::string contentlocation)
{
	std::string type(contentlocation.substr(contentlocation.rfind(".") + 1, contentlocation.size() - contentlocation.rfind(".")));
	if (_typeMap.find(type) == _typeMap.end())
		return "text/plain";
	return _typeMap[type];
}

void Response::parseCgiBody(std::string body)
{
	size_t	start;
	size_t	startBody = 0;
	size_t sepSize = 0;

	if((start = body.find("Status: ")) != std::string::npos)
	{
		this->_code = std::atoi(body.substr(start + 8, 3).c_str());
		this->_directives["Content-Length"] = readFile(this->_code);
	}
	else if ((start = body.find("Content-type: ")) != std::string::npos)
	{
		if ((startBody = body.find("\r\n\r\n", start)) != std::string::npos)
			sepSize = 4;
		else if((startBody = body.find("\n\n", start)) != std::string::npos)
			sepSize = 2;

		std::string line = body.substr(start, startBody - start);

		this->_body = body.substr(startBody + sepSize);
		this->_directives["Content-Type"] = line.substr(14);
		this->_directives["Content-Length"] = ft_itoa(this->_body.size());
	}
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

static int checkReadPermission(const std::string &path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
		if(s.st_mode & S_IROTH)
			return (1);
	return (0);
}

static int checkWritePermission(const std::string &path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
		if(s.st_mode & S_IWOTH)
			return (1);
	return (0);
}