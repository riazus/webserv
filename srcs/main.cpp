#include "includes.hpp"
#include "Config/Config.hpp"
#include "Kernel/Engine.hpp"

int main(int argc, char **argv)
{
    //just example
    Config config;
    try 
	{
     config.Parsing("./test.conf"); // path to .conf file
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
        return 1;
	}
    // Engine::Validate(argc, argv);
    Engine::Execute(argc, argv);
    // Engine::Exit();

    return (EXIT_SUCCESS);
}