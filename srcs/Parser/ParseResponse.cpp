
#include "ParseMsg.hpp"

/*ParseMsg::ParseMsg(ParseMsg const & src)
{

}
ParseMsg &ParseMsg::operator=(const ParseMsg &rhs)
{
	return *this;
}
ParseMsg::~ParseMsg(void)
{

}*/

std::string	ParseMsg::setLanguage(std::string acceptLanguage)
{
	if (acceptLanguage == "*")
		return ("en");
	else if (acceptLanguage.size() <= 3)
		return (acceptLanguage);
	else
		return (acceptLanguage.substr(0, acceptLanguage.find_first_of('-')));
}

Server ParseMsg::FindLocation(Server &server, std::string &locationName)
{
    /*std::list<Location *> locations(server.getLocation());
	if (*locationName.end() == '/')
		locationName.resize(locationName.size() - 1);

	if (locationName.empty())
		return server;
	if (locations.empty())
		return server;*/
	//TODO fix it
	return server;
}

std::string ParseMsg::CheckContentLocation(std::string contentLocation)
{
	return contentLocation;
}

void ParseMsg::ParseCookies(ResponseBody& responseBody, Request& request)
{

}

void ParseMsg::ParseResponse(ResponseBody &responseBody, Request &request, Server &server)
{
	std::string	locationName(request.getPath());
	Server		location(FindLocation(server, locationName));
	std::string	content;

	//parseCookies(responseBody, request);
	responseBody.setCode(request.getCode());
	std::cout << "IN BEGIN Parse Response: " << &request << std::endl;
	responseBody.setRequest(&request);
	responseBody.setServer(server);
	responseBody.setLocationFile(locationName);
	responseBody.setLocationPath(request.getPath());
	responseBody.setLocation(server.getLocation());
	//responseBody.setErrorMap(location.getErrorPage());
	responseBody.setClientBodyBufferSize(server.getMaxClientBodySize());
	//responseBody.setCgiParam(location.getCgiParam());
	//responseBody.setCgiPass(location.getCgiPass());
	
	responseBody.setAllowMethod(server.getMethods());
	responseBody.setLanguage(setLanguage(request.getHeader("Accept-Language")));
	//responseBody.setAutoIndex(server.getIndex());
	responseBody.setIndex(server.getIndex());

	/*if (!server.getAlias().empty() && location.extension == false)
		content = location.getRoot() + location.getAlias() + request.getPath().substr(locationName.size());
	else if (!location.getAlias().empty() && location.extension)
		content = location.getRoot() + location.getAlias() + locationName;
	else
		content = location.getRoot() + request.getPath();*/
	
	content = server.getRoot() + request.getPath();

	//content = "HTTP/1.1 200 OK \nContent-Type: text/html\nContent-Length: 142\n\n<html>\n<body>\n<center>\n<h1>Hi! This is webserv written by Riyaz and Matvey, enjoy!</h1>\n<center>\n</body>\n</html>";//checkContentLocation(content);

	responseBody.setContent(content);
}
