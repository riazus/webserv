#include "Cgi.hpp"

Cgi::Cgi(void)
{
	_body = "";
	_method = "";
	_contentSize = "";
	_contentType = "";
	_contentPath = "";
	_contentFile = "";
	_contentPathFile = "";
	_toExe = "";
	_query = "";
	_port = "";
	_host = "";
	_authorisation = "";
	_protocol = "";
	_serverName = "";
}

Cgi::Cgi(Cgi const & src)
{
	*this = src;
}

Cgi::~Cgi(void) {}

Cgi &	Cgi::operator=(const Cgi &rhs)
{
	_body = rhs._body;
	_method = rhs._method;
	_contentSize = rhs._contentSize;
	_contentType = rhs._contentType;
	_contentPath = rhs._contentPath;
	_contentFile = rhs._contentFile;
	_contentPathFile = rhs._contentPathFile;
	_toExe = rhs._toExe;
	_query = rhs._query;
	_port = rhs._port;
	_host = rhs._host;
	_authorisation = rhs._authorisation;
	_protocol = rhs._protocol;
	_serverName = rhs._serverName;
	return (*this);
}

static std::string	ft_itoa(int nb)
{
	std::string s;
	std::stringstream out;
	out << nb;
	s = out.str();
	return s;
}

void			Cgi::initCgiData(ResponseBody& config)
{
	mapString	headers(config.getRequest().getHeaders());

	_body = config.getRequest().getBody();
	_cgiPass = config.getLocation().getCgiPass();
	_method = config.getRequest().getMethod();
	_contentSize = ft_itoa(config.getRequest().bodySize);
	_contentType = headers["Content-Type"];
	_contentFile = config.getLocationPath();
	_contentPath = config.getLocationFile();
	_toExe = config.getContent();
	_query = config.getRequest().getQuery();
	_contentPathFile = config.getContentLocation();
	_port = ft_itoa(config.getServer().getPort());
	_host = config.getServer().getIpAdress();
	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		_authorisation = headers["Authorization"];
	_protocol = "HTTP/1.1";
	_serverName = "Webserv";
	_interface	= "CGI/1.1";
	_status = "200";
}

void			Cgi::setEnv(void)
{
	_env["AUTH_TYPE"] = _authorisation;
	_env["REDIRECT_STATUS"] = _status;
	_env["GATEWAY_INTERFACE"] = _interface;
	_env["SCRIPT_NAME"] = _toExe;
	_env["SCRIPT_FILENAME"] = _toExe;
	_env["REQUEST_METHOD"] = _method;
	_env["CONTENT_LENGTH"] = _contentSize;
	_env["CONTENT_TYPE"] = _contentType;
	_env["PATH_INFO"] = _contentPath;
	_env["PATH_TRANSLATED"] = _contentPath;
	_env["QUERY_STRING"] = _query;
	_env["REMOTEaddr"] = _host;
	_env["REMOTE_IDENT"] = _authorisation;
	_env["REMOTE_USER"] = _authorisation;
	_env["REQUEST_URI"] = _contentPath;
	_env["SERVER_NAME"] = _host;
	_env["SERVER_PORT"] = _port;
	_env["SERVER_PROTOCOL"] = _protocol;
	_env["SERVER_SOFTWARE"] = _serverName;
}

char **Cgi::mapToTab(void)
{
	char **CgiEnv = new char *[_env.size() + 1];
	std::string tempStr;
	int i = 0;

	for (mapString::iterator it = _env.begin(); it != _env.end(); it++, i++)
	{
		tempStr = it->first + "=" + it->second;
		CgiEnv[i] = new char[tempStr.size() + 1];
		CgiEnv[i] = std::strcpy(CgiEnv[i], tempStr.c_str());
	}
	CgiEnv[i] = NULL;
	return CgiEnv;
}

std::string		Cgi::execute(void)
{
	char		**CgiEnv = mapToTab();
	int			saveStdin = dup(STDIN_FILENO);
	int			saveStdout = dup(STDOUT_FILENO);
	FILE		*tmpIn = tmpfile();
	FILE		*tmpOut = tmpfile();
	int			In = fileno(tmpIn);
	int			Out = fileno(tmpOut);
	std::string	body;
	pid_t		pid;

	write(In, _body.c_str(), std::atoi(_contentSize.c_str()));
	lseek(In, 0, SEEK_SET);

	std::cout << "QUERY_STRING: " << this->_query << std::endl;
	if ((pid = fork()) == -1)
		return ("Status: 500\r\n");
	else if (pid == 0)
	{
		char * const * nll = NULL;

		std::cout << "cgi_pass :" << _cgiPass << " path : " << _toExe << std::endl;
		dup2(In, STDIN_FILENO);
		dup2(Out, STDOUT_FILENO);
		execve(_cgiPass.c_str(), nll, CgiEnv);
		write(STDOUT_FILENO, "Status: 500\r\n", 15);
	}
	else
	{
		char	buffer[BUFFER_SIZE + 1];

		waitpid(0, NULL, 0);
		lseek(Out, 0, SEEK_SET);

		int ret = 1;
		while (ret > 0)
		{
			ret = read(Out, buffer, BUFFER_SIZE);
			buffer[ret] = '\0';
			body += buffer;
		}
	}
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(tmpIn);
	fclose(tmpOut);
	close(In);
	close(Out);
	close(saveStdin);
	close(saveStdout);

	if (pid == 0)
		exit(0);
	for (int i = 0;  CgiEnv[i]; i++)
		delete CgiEnv[i];
	// std::cout << "BODY: " << body <<"----------------------------------------------<" << std::endl;
	delete [] CgiEnv;
	return body;
}
