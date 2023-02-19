#include "includes.hpp"
#include "Config/Config.hpp"
#include "Kernel/Engine.hpp"

int main(int argc, char **argv)
{
    // Engine::Validate(argc, argv);
    Engine::Execute(argc, argv);
    // Engine::Exit();

    return (EXIT_SUCCESS);
}