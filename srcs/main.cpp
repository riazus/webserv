#include "Kernel/Engine.hpp"
#include "Parser/Config.hpp"

int main(int argc, char **argv)
{
    //just example
    Config config;
    try 
	{
     config.getFile("./test.conf"); // path to .conf file
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
        return 1;
	}
    Engine::Validate(argc, argv);
    Engine::Execute(argc, argv);
    Engine::Exit();

    return (EXIT_SUCCESS);
}