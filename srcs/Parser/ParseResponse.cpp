
#include "ParseMsg.hpp"

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
    std::vector<Server> locations(server.getLocations());
	if (*locationName.end() == '/')
		locationName.resize(locationName.size() - 1);

	if (locationName.empty())
		return server;
	if (locations.empty())
		return server;

	for (std::vector<Server>::const_iterator i = locations.begin(); i != locations.end(); i++)
		if (i->getLocationName()  != "*")
		{
			for (std::string tmp = locationName; !tmp.empty(); tmp.resize(tmp.size() - 1))
			{
			//	std::cout << i->getLocationName() << " :LocationName == TMP: " << tmp << std::endl;

				if (tmp == i->getLocationName())
				{
					//std::cout << "LOCATION FOUND" << std::endl;
					locationName = tmp;
					return *i;
				}
			}
		}
		else
		{
			std::string suffix(i->getLocationName().substr(1));
			if (locationName.size() > suffix.size() && !locationName.compare(locationName.size() - suffix.size(), suffix.size(), suffix))
			{
				Server ret(*i);
				ret.setIsExtension(true);
				return ret;
			}
		}

	return server;
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
	//std::cout << "AFTER CHECK: " << ret << std::endl;
	return (ret);
}

void ParseMsg::ParseCookies(ResponseBody& responseBody, Request& request)
{
	std::map<std::string, std::string>	cookiesToSet;
	std::string							charSet(" ;");
	std::string							cookies(request.getHeader("Cookie"));

	cookies += " ";

	size_t start = cookies.find_first_not_of(charSet, 0);
	size_t end = 0;

	while ((end = cookies.find_first_of(charSet, start)) != std::string::npos)
	{
		std::string	tmp = cookies.substr(start, end - start);

		std::string token;
		std::string value;

		token.assign(tmp, 0, tmp.find_first_of('='));
		value.assign(tmp, tmp.find_first_of('=') + 1, tmp.size());

		cookiesToSet[token] = value;

		if ((start = cookies.find_first_not_of(charSet, end)) == std::string::npos)
			break ;
	}
	responseBody.setCookies(cookiesToSet);
}

void ParseMsg::ParseResponse(ResponseBody &responseBody, Request &request, Server &server)
{
	std::string	locationName(request.getPath());
	Server location(FindLocation(server, locationName));
	std::string	content;
	std::vector<Server> locations(server.getLocations());
	
	ParseCookies(responseBody, request);
	responseBody.setCode(request.getCode());
	responseBody.setRequest(request);
	responseBody.setServer(server);
	responseBody.setLocationFile(request.getPath());
	responseBody.setLocationPath(locationName);
	responseBody.setLocation(location);
	responseBody.setErrorMap(location.getErrorPage());
	responseBody.setClientBodyBufferSize(location.getMaxClientBodySize());
	responseBody.setCgiPass(location.getCgiPass());
	responseBody.setAllowMethod(location.getMethods());
	responseBody.setLanguage(setLanguage(request.getHeader("Accept-Language")));
	responseBody.setAutoIndex(location.getAutoindex());
	responseBody.setIndex(location.getIndex());

	if (!location.getAlias().empty() && location.getIsExtension() == false)
	{
		//std::cout << "YOY" << std::endl;
		content = location.getRoot() + location.getAlias() + request.getPath().substr(locationName.size());
	}
	else if (!location.getAlias().empty() && location.getIsExtension())
		content = location.getRoot() + location.getAlias() + locationName;
	else
		content = location.getRoot() + request.getPath();
	//std::cout << "BEFORE CHECK: " << content << std::endl;
	content = CheckContentLocation(content);
	responseBody.setContentLocation(content);
	//std::cout << "PATH OF CONTENT LOC: " << responseBody.getContentLocation() << std::endl;
}
