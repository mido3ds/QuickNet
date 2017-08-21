#ifndef __IResponder_h__
#define __IResponder_h__

#include "../Request.h"
#include "../Response.h"
#include "../StatusCode.h"

namespace http { 
namespace responder 
{

class IResponder
{
public:
    static IResponder* MakeResponder(Method);
    virtual Response GetResponse(const Request&) noexcept = 0;
    virtual ~IResponder();
};

}}

#endif  /* __IResponder_h__ */