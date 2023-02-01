#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "../includes.hpp"
#include "../Config/Config.hpp"

class Server;

class Location
{
    private:

        std::string path;
        std::string name;
        std::vector<std::string> methods;
        std::string root;
        std::string index;
        int autoindex;
        int client_body_buffer_size;

    public:

        Location();
        Location(const Location &Location);
        Location& operator= (const Location &Location);


        void setPath(std::string path);
        void setRoot(std::string root);
        void setIndex(std::string index);
        void setAutoindex(int autoindex);
        void setMethods(std::string method);

        std::string getPath();
        std::string getRoot();
        std::string getIndex();
        int getAutoindex();
        std::vector<std::string> getMethods();

        void location_error(std::string error);
        void is_valid();

        ~Location();

        class InvalidLocationException : public std::exception
    	{
	    public:

		    virtual const char* what() const throw();
	    };

        static Location *parse_location(std::vector<std::string> &config, int *line_count, Server* server);
};

#endif