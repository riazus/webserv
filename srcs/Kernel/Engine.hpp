#include <iostream>
#include "Kernel.hpp"

class Engine
{
    public :
        Engine();
        Engine(Engine &copy);
        Engine &operator=(Engine &copy);
        ~Engine();
        
        static void Validate(int argc, char **argv);
        static void Execute(int argc, char **argv);
        static void Exit();
};