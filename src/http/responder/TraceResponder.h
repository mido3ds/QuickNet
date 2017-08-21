#ifndef __TraceResponder_h__
#define __TraceResponder_h__

#include "IResponder.h"

namespace http { 
namespace responder 
{

class TraceResponder: public IResponder
{
public:
    virtual Response GetResponse(const Request&) noexcept;
};

}}

#endif  /* __TraceResponder_h__ */