#include "includes.hpp"
#include "Config/Config.hpp"
#include "Kernel/Engine.hpp"

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        // Engine::Validate(argc, argv);
        Engine::Execute(argv[1]);
        // Engine::Exit();
    }
    else
        std::cout << "Usage: ./webserv configs/<config name> " << std::endl;

    return (EXIT_SUCCESS);
}