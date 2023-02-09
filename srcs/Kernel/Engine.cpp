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
void Engine::Execute(int argc, char **argv)
{
    Config config;
    Kernel kernel;
    signal(SIGINT, signal_handler);
    try
    {
        //In this place need parse all things from config

        //for example Parser *parser = new Parser();
        //parser->Parse();
        config.Parsing("./test_1server.conf");
        //config.getFullInfo();
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
