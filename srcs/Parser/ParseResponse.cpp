
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

std::tuple<bool,Location> ParseMsg::FindLocation(Server &server, std::string &locationName)
{
    std::list<Location> locations(server.getLocations());
	if (*locationName.end() == '/')
		locationName.resize(locationName.size() - 1);

	if (locationName.empty())
		return std::make_tuple(false, server.getLocations().back());
	if (locations.empty())
		return std::make_tuple(false, server.getLocations().back());

	for (std::list<Location>::const_iterator i = locations.begin(); i != locations.end(); i++)
		if (i->getPath()  != "*")
		{
			for (std::string tmp = locationName; !tmp.empty(); tmp.resize(tmp.size() - 1))
				if (tmp == i->getPath())
				{
					locationName = tmp;
					return std::make_tuple(true,*i);
				}
		}
		else
		{
			std::string suffix(i->getPath().substr(1));
			if (locationName.size() > suffix.size() && !locationName.compare(locationName.size() - suffix.size(), suffix.size(), suffix))
			{
				Location ret(*i);
				return std::make_tuple(true,ret);
				ret.setIsExtension(true);
			}
		}
	//TODO fix it
	return std::make_tuple(false, server.getLocations().back());
}

std::string ParseMsg::CheckContentLocation(std::string content)
{
	const char	*tmp = content.c_str();
	std::string	ret;
	int			i = 0;
	bool		was = false;

	while(tmp[i])
	{
		if (tmp[i] == '/')
		{
			if (was == false && tmp[i + 1])
				ret.push_back(tmp[i]);
			was = true;
		}
		else
		{
			ret.push_back(tmp[i]);
			was = false;
		}
		i++;
	}
	return (ret);
}

void ParseMsg::ParseCookies(ResponseBody& responseBody, Request& request)
{

}

void ParseMsg::ParseResponse(ResponseBody &responseBody, Request &request, Server &server)
{
	std::string	locationName(request.getPath());
	Location location;
	bool isLocationExists;
	std::tie(isLocationExists, location) = FindLocation(server, locationName);
	std::string	content;

	//parseCookies(responseBody, request);
	responseBody.setCode(request.getCode());
	responseBody.setRequest(request);
	responseBody.setServer(server);
	responseBody.setLocationFile(locationName);
	responseBody.setLocationPath(request.getPath());
	responseBody.setLocation(location);
	responseBody.setErrorMap(server.getErrorPage());
	responseBody.setClientBodyBufferSize(server.getMaxClientBodySize());
	//responseBody.setCgiParam(location.getCgiParam());
	//responseBody.setCgiPass(location.getCgiPass());
	responseBody.setAllowMethod(server.getMethods());
	responseBody.setLanguage(setLanguage(request.getHeader("Accept-Language")));
	responseBody.setAutoIndex(server.getAutoindex());
	responseBody.setIndex(server.getIndex());

	std::string root = isLocationExists? location.getRoot() : server.getRoot();
	std::string alias = isLocationExists? location.getAlias() : server.getAlias();
	std::cout << "ROOT: " << root << std::endl;
	std::cout << "ALIAS: " << root << std::endl;
	if (!server.getAlias().empty() && location.getIsExtension() == false)
		content = root + alias + request.getPath().substr(locationName.size());
	else if (!location.getAlias().empty() && location.getIsExtension())
		content = root + alias + locationName;
	else
		content = root + request.getPath();
	content = CheckContentLocation(content);
	responseBody.setContentLocation(content);
}
