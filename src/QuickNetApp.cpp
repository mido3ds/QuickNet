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
    socket = new Socket(pack.host, pack.port, TCP, toServe);
}

QuickNetApp::~QuickNetApp()
{
    socket->Close();
    delete socket;
}

///////////////////////////////////////////////////////////////////////////

void QuickNetApp::Run() noexcept
{
    while (true)
    {
        // TODO: discuss number of connections
        socket->Listen(20);
        
        // TODO: make it in new thread
        Server server(socket->Accept());
        server.Serve();
    }
}

///////////////////////////////////////////////////////////////////////////
