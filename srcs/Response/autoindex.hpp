#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "../includes.hpp"

std::string		setLink(struct stat &buf, std::string dname, std::string url);
std::string		setSize(struct stat &buf);
std::string		setCreationTime(struct stat &buf);
std::string		setModificationTime(struct stat &buf);
std::string 	createDirList(std::string path, std::string dname, std::string url);
std::string		createFirstPart(std::string &path);

std::string		createAutoindexPage(std::string path, std::string url);

#endif
