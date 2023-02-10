#include "Location.hpp"

Location::Location(): path(""), name(""), root(""), index(""), autoindex(0), client_body_buffer_size(0), extension(false)
{
}

Location::Location(const Location &location)
{
	*this = location;
}

Location &Location::operator=(const Location &location)
{
	this->path = location.path;
    this->name = location.name;
    this->methods = location.methods;
    this->root = location.root;
    this->index = location.index;
    this->autoindex = location.autoindex;
    this->client_body_buffer_size = location.client_body_buffer_size;
	this->extension = location.extension;
	return (*this);
}

void Location::setPath(std::string path)
{
	this->path = path;
}

std::string Location::getPath() const
{
	return (this->path);
}

void Location::setRoot(std::string root)
{
	this->root = root;
}

std::string Location::getRoot()
{
	return (this->root);
}

void Location::setIndex(std::string index)
{
	this->index = index;
}

std::string Location::getIndex()
{
	return (this->index);
}

void Location::setAutoindex(int autoindex)
{
	this->autoindex = autoindex;
}

int Location::getAutoindex()
{
	return (this->autoindex);
}

void Location::setMethods(std::string method)
{
	this->methods.push_back(method);
}

void Location::setIsExtension(bool val)
{
	this->extension = val;
}

std::vector<std::string> Location::getMethods()
{
	return (this->methods);
}

bool Location::getIsExtension()
{
    return this->extension;
}

std::string Location::getAlias()
{
    return "";
}

void Location::location_error(std::string error)
{
	if (error != "")
		std::cout << "Error: " << error << std::endl;
	throw Location::InvalidLocationException();
}

void	Location::is_valid()
{
	std::string error;
	if (getPath() == "")
	{
		error = (getPath() + " path is not set");
		location_error(error);
	}
	std::vector<std::string> methods = getMethods();
	if (methods.size() == 0)
	{
		error = (getPath() + " allowed methods are not set");
		location_error(error);
	}
	if (getRoot() == "")
	{
		error = (getPath() + " location_root is not set");
		location_error(error);
	}
}

Location Location::parse_location(std::vector<std::string> &config, int *line_count)
{
	(*line_count)++;
	std::vector<std::string> line = ft_split(config[*line_count], CHARTOSKIP);
	if (line.size()!= 3)
		config_error("expected a directory and '{' after location");
	this->setPath(line[1]);
	if (line[2] != "{")
		config_error("missing '{'");
	(*line_count)++;
	std::vector<std::string>::const_iterator it = config.begin() + *line_count;
	while (it != config.end())
	{
		std::vector<std::string> line = ft_split(*it, CHARTOSKIP);
		if (!line.size() || !line[0].size())
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
				this->setMethods(*it);
			}
		}
		else if (line[0] == "root")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after root");
			this->setRoot(line[1]);
		}
		else if (line[0] == "index")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after index");
			this->setIndex(line[1]);
		}
		else if (line[0] == "autoindex")
		{
			if (line.size() != 2)
				config_error("expected 1 argument after autoindex");
			this->setAutoindex(atoi(line[1].c_str()));
		}
		it++;
		(*line_count)++;
	}
	std::vector<std::string> end = ft_split(*it, CHARTOSKIP);
	if (end[0] != "}")
		config_error("missing '}'");
	if (this->getRoot().size() == 0)
		this->setRoot(this->getPath());
	return *this;
}

const char* Location::InvalidLocationException::what() const throw()
{
	return "InvalidLocationException: Invalid location configuration!";
}

Location::~Location()
{
}