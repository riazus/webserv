#include "../includes.hpp"

bool	ft_in_charset(char const c, const std::string &charset)
{
	int	i_charset;

	i_charset = 0;
	while (charset[i_charset])
	{
		if (c == charset[i_charset++])
			return true;
	}
	return false;
}

std::vector<std::string> ft_split(const std::string &str, const std::string &charset)
{
	std::vector<std::string> res;
	std::string			tmp;
	int					i;

	i = 0;
	while (i < str.length())
	{
		while (i < str.length() && ft_in_charset(str[i], charset))
			i++;
		if (i < str.length())
		{
			tmp = "";
			while (i < str.length() && !ft_in_charset(str[i], charset))
				tmp += str[i++];
			res.push_back(tmp);
		}
	}
	return res;
}

bool is_integer(const std::string &s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
		return false;
	char *p;
	strtol(s.c_str(), &p, 10);
	return (*p == 0);
}