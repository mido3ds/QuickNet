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
    delete socket;
}

///////////////////////////////////////////////////////////////////////////

void QuickNetApp::Run() noexcept
{
    while (true)
    {
        socket->Listen(20); // TODO: discuss number of connections
        Socket client = socket->Accept();

        /*pseudocode:
            new_thread = new Thread()
            // in the new_thread
            while is_connected and connection_not_timed_out():
                req = receive_req()
                respnse = handle_req(req)
                send_response(response)
        */
    }
}

///////////////////////////////////////////////////////////////////////////
