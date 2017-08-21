#ifndef __ConnectResponder_h__
#define __ConnectResponder_h__

#include "IResponder.h"

namespace http { 
namespace responder 
{

class ConnectResponder: public IResponder
{
public:
    virtual Response GetResponse(const Request&, const RoutesMap&) noexcept;
};

}}

#endif  /* __ConnectResponder_h__ */