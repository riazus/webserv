#ifndef PARSEMSG_HPP
#define PARSEMSG_HPP

#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../Response/ResponseBody.hpp"
#include "../includes.hpp"

class ParseMsg
{
    public:
    
    /*REQUEST*/
    public:
        void            ParseBody(Request &request);
        void            ParseHeader(Request &request);
    private:
        static stringVector    methods;
        static stringVector    InitMethods();
        void            SplitHeader(Request &request);
        void            ParseFirstLine(Request &request);

    /*RESPONSE*/
    public:
        void            ParseResponse(ResponseBody &responseBody, Request &request, Server &server);
    private:
        Server                      FindLocation(Server &server, std::string &locationName);
        std::string                 CheckContentLocation(std::string contentLocation);
        void		                ParseCookies(ResponseBody& responseBody, Request& request);
        std::string                 setLanguage(std::string acceptLanguage);

};

static std::string	findtoken(std::string line);
static std::string	findvalue(std::string line);

#endif