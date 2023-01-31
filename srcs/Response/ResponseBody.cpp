#include "ResponseBody.hpp"

ResponseBody::ResponseBody(void)
{
}

ResponseBody::ResponseBody(ResponseBody const & src)
{
	*this = src;
}

ResponseBody &ResponseBody::operator=(ResponseBody const &rhs)
{
	return *this;
}

ResponseBody::~ResponseBody(void)
{
}

