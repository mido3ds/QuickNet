#ifndef __PostResponder_h__
#define __PostResponder_h__

#include "IResponder.h"

namespace http { 
namespace responder 
{

class PostResponder: public IResponder
{
public:
    virtual Response GetResponse(const Request&, const RoutesMap&) noexcept;
};

}}

#endif  /* __PostResponder_h__ */