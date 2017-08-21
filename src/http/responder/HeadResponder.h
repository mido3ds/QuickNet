#ifndef __HeadResponder_h__
#define __HeadResponder_h__

#include "IResponder.h"

namespace http { 
namespace responder 
{

class HeadResponder: public IResponder
{
public:
    virtual Response GetResponse(const Request&, const RoutesMap&) noexcept;
};

}}

#endif  /* __HeadResponder_h__ */