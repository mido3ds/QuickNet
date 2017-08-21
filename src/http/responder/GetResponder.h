#ifndef __GetResponder_h__
#define __GetResponder_h__

#include "IResponder.h"

namespace http { 
namespace responder 
{

class GetResponder: public IResponder
{
public:
    virtual Response GetResponse(const Request&, const RoutesMap&) noexcept;
};

}}

#endif  /* __GetResponder_h__ */