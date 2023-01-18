#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Includes.hpp"


class Location
{

    private:

        std::string path;    	class ConfigFileContentException : public std::exception
	{
	    public:

		    virtual const char* what() const throw();
	};
        std::string name;
        std::vector<std::string> methods;
        std::string root;
        std::string index;
        std::string autoindex;
        int client_body_buffer_size;

    public:

        Location();
        Location(const Location &Location);
        Location& operator= (const Location &Location);


        void setPath(std::string path);
        void setName(std::string name);
        void setRoot(std::string root);
        void setIndex(std::string index);
        void setAutoindex(std::string autoindex);
        void setMethods(std::string method);

        std::string getPath();
        std::string getName();
        std::string getRoot();
        std::string getIndex();
        std::string getAutoindex();
        std::vector<std::string> getMethods();


        ~Location();

};

#endif