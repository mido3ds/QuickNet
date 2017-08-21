#ifndef __GetResponder_h__
#define __GetResponder_h__

#include "IResponder.h"

namespace http { 
namespace responder 
{

class GetResponder: public IResponder
{
public:
    virtual Response GetResponse(const Request&) noexcept;
};

}}

#endif  /* __GetResponder_h__ */