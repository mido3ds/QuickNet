#ifndef __IResponder_h__
#define __IResponder_h__

#include "../Request.h"
#include "../Response.h"
#include "../StatusCode.h"

namespace http { 
namespace responder 
{
    using InputPackage = std::unordered_map<std::string, std::string>;
    using RouteListener = std::function<std::string(InputPackage)>;
    using RoutesMap = std::unordered_map<std::string, RouteListener>;

class IResponder
{
public:
    static IResponder* MakeResponder(Method);
    virtual Response GetResponse(const Request&, const RoutesMap&) noexcept = 0;
    virtual ~IResponder();
};

}}

#endif  /* __IResponder_h__ */