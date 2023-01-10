#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <iostream>
#include <vector>


class Location
{

    private:

        std::string name;
        std::string method;
        std::string root;
        std::string alias;
        std::string index;
        std::string cgi_pass;
        int client_body_buffer_size;

    public:

        Location();
        Location(const Location &Location);
        Location& operator= (const Location &Location);
        ~Location();

};

#endif