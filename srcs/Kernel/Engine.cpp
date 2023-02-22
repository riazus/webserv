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

extern volatile bool g_run;

void signal_handler(int signum)
{
	(void)signum;
	g_run = false;
}

//Validate/Parse app's config
void Engine::Validate(int argc, char **argv)
{
    //TODO: Matvey
    
}

//Main executable method
void Engine::Execute(std::string configName)
{
    Config config;
    Kernel kernel;
    signal(SIGINT, signal_handler);
    try
    {
        config.Parsing(configName);
        config.Parsing(DEFAULT);
        kernel.SetConfig(config);
        kernel.Run();
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
