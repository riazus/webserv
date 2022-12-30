#ifndef CONFIG_H
# define CONFIG_H

# include <iostream>

class Config
{
	public:
		Config(void);
		Config(Config const & src);
		Config &operator=(const Config &rhs);
		~Config(void);

	private:
		
};


#endif