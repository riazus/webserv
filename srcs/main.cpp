#include "Kernel/Engine.hpp"

int main(int argc, char **argv)
{
    //just example
    Engine::Validate(argc, argv);
    Engine::Execute(argc, argv);
    Engine::Exit();

    return (EXIT_SUCCESS);
}