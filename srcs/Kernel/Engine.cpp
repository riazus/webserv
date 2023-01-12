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
        //In this place need parse all things from config

        //for example Parser *parser = new Parser();
        //parser->Parse();

        Kernel *kernel = new Kernel();
        /*std::string wait[] = {"⠋", "⠙", "⠸", "⠴", "⠦", "⠇"};
        int timeout = 200;
        int nfds = 0;
        std::string	request;
        kernel->CreateSocket();

        //kernel->init();        
        while(true)
        {
            errno = 0;

            kernel->_nfds = epoll_wait(kernel->);
            kernel->ListenConnections();
        }*/
        kernel->Run();

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
