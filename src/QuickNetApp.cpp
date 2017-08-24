#include "QuickNetApp.h"

using std::string;
using std::exception;

QuickNetApp::QuickNetApp(string host, string port, bool withDebugging)
{
    OptionsPackage pack = {host = host, port = port};
    pack.debug = withDebugging;

    ConstructorHelper(pack);
}
QuickNetApp::QuickNetApp(OptionsPackage pack)
{
    ConstructorHelper(pack);    
}
inline void QuickNetApp::ConstructorHelper(OptionsPackage pack)
{
    socket = new Socket(pack.host, pack.port, TCP, toBind);
    options = pack;

    if (options.host == "")
        options.host = "localhost";
}

QuickNetApp::~QuickNetApp()
{
    socket->Close();
    delete socket;
}

///////////////////////////////////////////////////////////////////////////

void QuickNetApp::Run() noexcept
{
    socket->Listen(numbOfConnections);

    std::cout << date::GMTDateTime() << ": Server started listening at " 
            << "[" << options.host << ":" << options.port << "]"
            << std::endl;
            
    while (true)
    {
        try 
        {
            Socket newClient = socket->Accept();

            // TODO: make it in new thread
            Server(newClient).Serve();
        }
        // TODO: add better logging and exceptions handling
        catch (...)
        {
            std::cout << date::GMTDateTime() << ": Caught unkown exception #" << errno << ", " << std::strerror(errno) << std::endl;
        }
    }

    // TODO: you know you will never get here, right?
    std::cout << date::GMTDateTime() << ": Server stopped" << std::endl;
}

///////////////////////////////////////////////////////////////////////////
