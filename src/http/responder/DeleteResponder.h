#ifndef __DeleteResponder_h__
#define __DeleteResponder_h__

#include "IResponder.h"

namespace http { 
namespace responder 
{

class DeleteResponder: public IResponder
{
public:
    virtual Response GetResponse(const Request&) noexcept;
};

}}

#endif  /* __DeleteResponder_h__ */