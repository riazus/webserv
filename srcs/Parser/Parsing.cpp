#include "Includes.hpp"

Server *parse_server(std::vector<std::string> config, int *line_count)
{
	std::vector<std::string> line = ft_split(config[0], CHARTOSKIP);

	if (line[1] != "{")
		throw Config::ConfigFileContentException();

	Server *server = new Server();
	std::vector<std::string>::const_iterator it = config.begin() + *line_count;

	if (it == config.end())
		throw Config::ConfigFileContentException();
	it++;
	while (it != config.end())
	{
		std::vector<std::string> line = ft_split(*it, CHARTOSKIP);
		if (!line.size() && !line[0].size())
		{
			it++;
			(*line_count)++;
			continue;
		}
		if (line[0][0] == '#')
		{
			it++;
			(*line_count)++;
			continue;
		}
		if (line[0] == "}")
			break;
		else if (line[0] == "location")
		{
			if (line.size() != 3)
				throw Config::ConfigFileContentException();
			int tmp_count = *line_count;
			// server.setLocation(parse_location(config, line_count, server)); ---------------------------------------------------------------
			it += *line_count - tmp_count;
		}
		else if (line[0] == "server_name")
		{
			if (line.size() != 2)
				throw Config::ConfigFileContentException();
			server->setServerName(line[1]);
		}
		else if (line[0] == "error_page")
		{
			if (line.size() != 3)
				throw Config::ConfigFileContentException();
			server->setErrorPage(atoi(line[1].c_str()), line[2]);
		}
		else if (line[0] == "cgi")
		{
			if (line.size() != 3)
				throw Config::ConfigFileContentException();
			server->setCgi(line[1], line[2]);
		}
		else if (line[0] == "listen")
		{
			if (line.size() != 2)
				throw Config::ConfigFileContentException();
			for (int i=0; i < line[1].size(); i++)
			{
				if (!isdigit(line[1][i]) || i > 4)
					throw Config::ConfigFileContentException();
			}
			server->setPort(atoi(line[1].c_str()));
		}
		else if (line[0] == "root")
		{
			if (line.size() != 2)
				throw Config::ConfigFileContentException();
			server->setRoot(line[1]);
		}
		else if (line[0] == "index")
		{
			if (line.size() != 2)
				throw Config::ConfigFileContentException();
			server->setIndex(line[1]);
		}
		else if (line[0] == "max_client_body_size")
		{
			if (line.size() != 2)
				throw Config::ConfigFileContentException();
			if (!is_integer(line[1]))
				throw Config::ConfigFileContentException();
			server->setMaxClientBodySize(atoll(line[1].c_str()));
		}
		it++;
		(*line_count)++;
	}
	std::vector<std::string> end = ft_split(*it, CHARTOSKIP);
	while (!end.size() || end[0] != "}")
	{
		(*line_count)++;
		end = ft_split(config[*line_count], CHARTOSKIP);
	}
	if (end.size() && end[0] != "}")
		throw Config::ConfigFileContentException();
	// if (!server->is_valid(error))
	// 	throw Config::ConfigFileContentException(); ------------))
	// 	throw Config::ConfigFileContentException();--------------------------------------
	return server;
}