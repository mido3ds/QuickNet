#include "IResponder.h"

#include "GetResponder.h"
#include "HeadResponder.h"
#include "PostResponder.h"
#include "PutResponder.h"
#include "DeleteResponder.h"
#include "ConnectResponder.h"
#include "OptionsResponder.h"
#include "TraceResponder.h"

using namespace http;
using namespace responder;

IResponder* IResponder::MakeResponder(Method method)
{
    switch (method)
    {
        case GET:
            return new GetResponder();
        case HEAD:
            return new HeadResponder();
        case POST:
            return new PostResponder();
        case PUT:
            return new PutResponder();
        case DELETE:
            return new DeleteResponder();
        case CONNECT:
            return new ConnectResponder();
        case OPTIONS:
            return new OptionsResponder();
        case TRACE:
            return new TraceResponder();
    }
}

IResponder::~IResponder()
{}