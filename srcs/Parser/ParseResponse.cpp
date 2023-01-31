
#include "ParseMsg.hpp"

Server FindLocation(Server &server, std::string &locationName)
{

}

std::string ParseMsg::CheckContentLocation(std::string contentLocation)
{

}

void ParseMsg::ParseCookies(ResponseBody& responseBody, Request& request)
{

}

/*void ParseMsg::ParseResponse(ResponseBody &responseBody, Request &request, Server &server)
{
	std::string	locationName(request.getPath());
	Config		location(findLocation(server, locationName));
	std::string	content;

	parseCookies(responseBody, request);
	responseBody.setCode(request.getCode());
	responseBody.setRequest(request);
	responseBody.setServer(server);
	responseBody.setLocationFile(request.getPath());
	responseBody.setLocationPath(locationName);
	responseBody.setLocation(location);
	responseBody.setErrorMap(location.getErrorPage());
	responseBody.setClientBodyBufferSize(location.getClientBodyBufferSize());
	responseBody.setCgiParam(location.getCgiParam());
	responseBody.setCgiPass(location.getCgiPass());
	responseBody.setAllowMethod(location.getAllowedMethods());
	responseBody.setLanguage(setLanguage(request.getHeader("Accept-Language")));
	responseBody.setAutoIndex(location.getAutoIndex());
	responseBody.setIndex(location.getIndex());

	if (!location.getAlias().empty() && location.extension == false)
		content = location.getRoot() + location.getAlias() + request.getPath().substr(locationName.size());
	else if (!location.getAlias().empty() && location.extension)
		content = location.getRoot() + location.getAlias() + locationName;
	else
		content = location.getRoot() + request.getPath();

	content = checkContentLocation(content);

	responseBody.setContentLocation(content);
}*/
