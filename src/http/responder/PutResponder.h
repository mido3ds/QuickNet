#ifndef __PutResponder_h__
#define __PutResponder_h__

#include "IResponder.h"

namespace http { 
namespace responder 
{

class PutResponder: public IResponder
{
public:
    virtual Response GetResponse(const Request&, const RoutesMap&) noexcept;
};

}}

#endif  /* __PutResponder_h__ */