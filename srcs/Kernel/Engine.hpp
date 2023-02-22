#ifndef ENGINE_H
# define ENGINE_H

#include "../Kernel/Kernel.hpp"

class Engine
{
    public :
        Engine();
        Engine(Engine &copy);
        Engine &operator=(Engine &copy);
        ~Engine();
        
        static void Validate(int argc, char **argv);
        static void Execute(std::string configName);
        static void Exit();
};
#endif