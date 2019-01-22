/*
   Copyright (c) 2011 Daniel Bakkelund.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
   THE POSSIBILITY OF SUCH DAMAGE.
*/



#ifndef CPUNIT_GLOBMATCHER_HPP
#define CPUNIT_GLOBMATCHER_HPP

#include <vector>
#include <string>

namespace cpunit {

  class GlobMatcher {
    static const char wc;
    const std::string pattern;

    /**
     * Contracts consecutive sequences of wildcards to one wildcard.
     */
    static std::string contract_wildcards(const std::string &s);

    /**
       Attempt to match the substring 's.substr(si)' with the 
       pattern 'pattern.substring(pi)'
     */
    bool attempt_match(const std::string& s, const std::size_t si, const std::size_t pi) const;

    /**
       s   - the string under concideration
       si  - the index in the string to consider next
       wci - the wildcard index in the pattern
     */
    bool on_wildcard(const std::string& s, const std::size_t si, const std::size_t wci) const;
  public:
    GlobMatcher(const std::string _pattern);
    virtual ~GlobMatcher();

    bool matches(const std::string &s) const;
  };
}

#endif // CPUNIT_GLOBMATCHER_HPP
