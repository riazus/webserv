#include "Engine.hpp"

Engine::Engine()
{
}

Engine::Engine(Engine &copy)
{
    *this = copy;
}

Engine &Engine::operator=(Engine &copy)
{
    return *this;
}

Engine::~Engine()
{
}

//Validate/Parse app's config
void Engine::Validate(int argc, char **argv)
{
    //TODO: Matvey
    
}

//Main executable method
void Engine::Execute(int argc, char **argv)
{
    try
    {
        Kernel *kernel = new Kernel();
        kernel->CreateSocket();
        
        while(true)
        {
            kernel->ListenConnections();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

//Close any FDs and deallocate memory
void Engine::Exit()
{

}
