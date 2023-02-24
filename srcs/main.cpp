#include "includes.hpp"
#include "Config/Config.hpp"
#include "Kernel/Kernel.hpp"

extern volatile bool g_run;

void signal_handler(int signum)
{
	(void)signum;
	g_run = false;
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        Config config;
        Kernel kernel;
        signal(SIGINT, signal_handler);
        try
        {
            config.Parsing(argv[1]);
            config.Parsing(DEFAULT);
            kernel.SetConfig(config);
            kernel.Run();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
        std::cout << "Usage: ./webserv configs/<config name> " << std::endl;

    return (EXIT_SUCCESS);
}