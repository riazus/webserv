#ifndef PARSEMSG_HPP
#define PARSEMSG_HPP

#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../includes.hpp"

class ParseMsg
{
    public:
		ParseMsg(void);
		ParseMsg(ParseMsg const & src);
		ParseMsg &operator=(const ParseMsg &rhs);
		~ParseMsg(void);
    
    /*REQUEST*/
    public:
        void            ParseBody(Request &request);
        void            ParseHeader(Request &request);
    private:
        stringVector    methods;
        stringVector    InitMethods();
        void            SplitHeader(Request &request);
        void            ParseFirstLine(Request &request);

    /*RESPONSE*/
    /*public:
        static void ParseResponse(Response &response);
    private:
        static Config FindLocation(Server &server, std::string &locationName);
        static std::string CheckContentLocation(std::string contentLocation);*/

};

static std::string	findtoken(std::string line);
static std::string	findvalue(std::string line);

#endif