#ifndef __OptionsResponder_h__
#define __OptionsResponder_h__

#include "IResponder.h"

namespace http { 
namespace responder 
{

class OptionsResponder: public IResponder
{
public:
    virtual Response GetResponse(const Request&, const RoutesMap&) noexcept;
};

}}

#endif  /* __OptionsResponder_h__ */