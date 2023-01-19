#include "Includes.hpp"

void config_error(std::string error)
{
	if (error != "")
		std::cout << error << std::endl;
	throw Config::ConfigFileContentException();
}

Server *parse_server(std::vector<std::string> config, int *line_count)
{
	std::vector<std::string> line = ft_split(config[0], CHARTOSKIP);

	if (line[1] != "{")
		config_error("expected '{' after server");
	Server *server = new Server();
	std::vector<std::string>::const_iterator it = config.begin() + *line_count;

	if (it == config.end())
		config_error("");
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
			std::cout << *line_count << std::endl;
			if (line.size() != 3)
						config_error("expected 2 arguments after location");
			int tmp_count = *line_count;
			server->setLocation(parse_location(config, line_count, server));
			it += *line_count - tmp_count;
		}
		else if (line[0] == "server_name")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after server_name");
			server->setServerName(line[1]);
		}
		else if (line[0] == "error_page")
		{
			if (line.size() != 3)
				config_error("expected 2 arguments after error_page");
			server->setErrorPage(atoi(line[1].c_str()), line[2]);
		}
		else if (line[0] == "listen")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after listen");
			for (int i=0; i < line[1].size(); i++)
			{
				if (!isdigit(line[1][i]) || i > 4)
									config_error("expected integer after listen");
			}
			server->setPort(atoi(line[1].c_str()));
		}
		else if (line[0] == "cgi")
		{
			if (line.size() != 3)
				config_error("expected 2 arguments after cgi");
			server->setCgi(line[1], line[2]);
		}
		else if (line[0] == "root")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after root");
			server->setRoot(line[1]);
		}
		else if (line[0] == "max_client_body_size")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after max_client_body_size");
			if (!is_integer(line[1]))
				config_error("expected integer after max_client_body_size");
			server->setMaxClientBodySize(atoll(line[1].c_str()));
		}
		else if (line[0] == "index")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after index");
			server->setIndex(line[1]);
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

Location parse_location(std::vector<std::string> &config, int *line_count, Server* server)
{
	Location location;

	(*line_count)++;
	std::vector<std::string> line = ft_split(config[*line_count], CHARTOSKIP);
	if (line.size()!= 3)
		config_error("expected a directory and '{' after location");
	location.setPath(line[1]);
	if (line[2] != "{")
		config_error("missing '{'");
	(*line_count)++;
	std::vector<std::string>::const_iterator it = config.begin() + *line_count;
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
		else if (line[0] == "allow_methods")
		{
			if (line.size() < 2)
				config_error("expected 1 argument min after allow_methods");
			for (std::vector<std::string>::const_iterator it = line.begin() + 1; it != line.end(); it++)
			{
				location.setMethods(*it);
			}
		}
		else if (line[0] == "root")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after root");
			location.setRoot(line[1]);
		}
		else if (line[0] == "index")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after index");
			location.setIndex(line[1]);
		}
		else if (line[0] == "autoindex")
		{
			std::cout << line.size() << " " << line[0] << line[1] << std::endl;
			if (line.size() != 2)
				config_error("expected 1 argument after autoindex");
			location.setAutoindex(atoi(line[1].c_str()));
		}
		it++;
		(*line_count)++;
	}
	std::vector<std::string> end = ft_split(*it, CHARTOSKIP);
	if (end[0] != "}")
		config_error("missing '}'");
	if (location.getRoot().size() == 0)
		location.setRoot(location.getPath());
	return location;
}