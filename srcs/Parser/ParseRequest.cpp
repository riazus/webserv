#include "ParseMsg.hpp"

stringVector		ParseMsg::InitMethods()
{
	stringVector	methods;

	methods.push_back("GET");
	methods.push_back("POST");
	methods.push_back("DELETE");

	return methods;
}

stringVector	ParseMsg::methods = ParseMsg::InitMethods();

void ParseMsg::ParseBody(Request &request)
{
    std::string		body;
	size_t			body_start(0);

	body_start = request.requestLine.find("\r\n\r\n");
	if ((body_start = request.requestLine.find("\r\n\r\n")) == std::string::npos)
		std::cout << "ERROR in parse Body" << std::endl;
	if (request.getCode() == 400)
		return ;

	body = request.requestLine.substr(body_start + 4); // +4 to skip the "\r\n\r\n"
	request.bodySize = request.contentSize - body_start - 4;
	if ((unsigned long)std::atol(request.getHeader("Content-Length").c_str()) != request.contentSize - body_start - 4)
	{
		request.setCode(400);
		std::cerr << "Error parsing request: content length different from content real size" << std::endl;
		return ;
	}
	else
		request.setBody(body);
}

void ParseMsg::ParseHeader(Request &request)
{
    std::string	token;
	std::string	value;

	request.ResetHeaders();

	SplitHeader(request);
	ParseFirstLine(request);

	for (stringVector::iterator i = request.header.begin() + 1; i != request.header.end(); i++)
	{
		token = findtoken(*i);
		value = findvalue(*i);
		if (request.getHeaders().count(token))
		{
			request.setHeader(token, value);
			request.setEnvForCgi(token, value);
		}
	}
	
	request.setNetwork(request.getHeader("Host"));
}

static std::string	findtoken(std::string line)
{
	return (line.substr(0, line.find_first_of(':')));
}

static std::string	findvalue(std::string line)
{
	return (line.substr(line.find_first_of(' ') + 1));
}

void ParseMsg::SplitHeader(Request &request)
{
    std::string tmp;
	size_t		start(0);
	size_t		end;

	while((end = request.requestLine.find_first_of('\n', start)) != std::string::npos)
	{
		tmp = request.requestLine.substr(start, end - start);
		if (tmp == "")
			break;
		if (tmp[tmp.size() - 1] == '\r' && tmp.size())
			tmp.resize(tmp.size() - 1);
		request.header.push_back(tmp);
		start = (end == std::string::npos) ? end : end + 1;
	}
	return ;
}

void ParseMsg::ParseFirstLine(Request &request)
{
    std::string	first(request.header.front());
	size_t		end;
	size_t		start;

	if ((end = first.find_first_of(' ')) == std::string::npos)
	{
		request.setCode(400);
		return ;
	}
	request.setMethod(first.substr(0, end));

	if (std::find(methods.begin(), methods.end(), request.getMethod()) == methods.end())
	{
		request.setCode(400);
		return ;
	}

	if ((start = first.find_first_not_of(' ', end))  == std::string::npos)
	{
		request.setCode(400);
		return ;
	}

	if ((end = first.find_first_of(' ', start)) == std::string::npos)
	{
		request.setCode(400);
		return ;
	}
	request.setPath(first.substr(start, end - start));

	if ((end = first.find_first_not_of(' ', end)) == std::string::npos)
	{
		request.setCode(400);
		return ;
	}

	std::string version;

	if (first.substr(end, 5).compare("HTTP/") == 0)
		request.setVersion((version = first.substr(end + 5, 3)));
	else
		return;

	if (version != "1.0" && version != "1.1")
	{
		request.setCode(400);
		std::cerr << "Error parsing request: HTTP/" << version << " find in request header"  << std::endl;
		return ;
	}
}