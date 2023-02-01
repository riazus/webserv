
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

Server FindLocation(Server &server, std::string &locationName)
{
	return server;
}

std::string ParseMsg::CheckContentLocation(std::string contentLocation)
{
	return contentLocation;
}

void ParseMsg::ParseCookies(ResponseBody& responseBody, Request& request)
{

}

void ParseMsg::ParseResponse(ResponseBody &responseBody, Request &request, Server const &server)
{
	// std::string	locationName(request.getPath());
	// Config		location(findLocation(server, locationName));
	std::string	content;

	// //parseCookies(responseBody, request);
	// responseBody.setCode(request.getCode());
	// responseBody.setRequest(request);
	// responseBody.setServer(server);
	// responseBody.setLocationFile(request.getPath());
	// responseBody.setLocationPath(locationName);
	// responseBody.setLocation(location);
	// responseBody.setErrorMap(location.getErrorPage());
	// responseBody.setClientBodyBufferSize(location.getClientBodyBufferSize());
	// responseBody.setCgiParam(location.getCgiParam());
	// responseBody.setCgiPass(location.getCgiPass());
	// responseBody.setAllowMethod(location.getAllowedMethods());
	// responseBody.setLanguage(setLanguage(request.getHeader("Accept-Language")));
	// responseBody.setAutoIndex(location.getAutoIndex());
	// responseBody.setIndex(location.getIndex());

	// if (!location.getAlias().empty() && location.extension == false)
	// 	content = location.getRoot() + location.getAlias() + request.getPath().substr(locationName.size());
	// else if (!location.getAlias().empty() && location.extension)
	// 	content = location.getRoot() + location.getAlias() + locationName;
	// else
	//content = location.getRoot() + request.getPath();

	content = "HTTP/1.1 200 OK \nContent-Type: text/html\nContent-Length: 142\n\n<html>\n<body>\n<center>\n<h1>Hi! This is webserv written by Riyaz and Matvey, enjoy!</h1>\n<center>\n</body>\n</html>";//checkContentLocation(content);

	//responseBody.setContent(content);

}
