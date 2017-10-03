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



#include "cpunit_ConformityChecker.hpp"
#include "cpunit_trace.hpp"
#include <sstream>

cpunit::ConformityChecker::ConformityChecker(const std::string &path_pattern, 
					     const std::string &name_pattern, 
					     const std::string &file_pattern) :
  path(path_pattern),
  name(name_pattern),
  file(file_pattern),
  file_eq_path(false),
  extension("*")
{
  CPUNIT_DTRACE("ConformityChecker - instantiated with ["<<path_pattern<<"], ["<<name_pattern<<"], ["<<file_pattern<<"], [false]");
}


cpunit::ConformityChecker::ConformityChecker(const std::string &path_pattern, 
					     const std::string &name_pattern) :
  path(path_pattern),
  name(name_pattern),
  file("*"),
  file_eq_path(true),
  extension("*")
{
  CPUNIT_DTRACE("ConformityChecker - instantiated with ["<<path_pattern<<"], ["<<name_pattern<<"], [*], [true]");
}

cpunit::ConformityChecker::~ConformityChecker()
{}

void 
cpunit::ConformityChecker::set_extension(const std::string &extension_pattern) {
  extension = extension_pattern;
}

bool 
cpunit::ConformityChecker::matches(const RegInfo &ri) const {
  CPUNIT_DTRACE("ConformityChecker - checking ["<<ri.get_path()<<"], ["<<ri.get_name()<<"], ["<<ri.get_file()<<']');
  bool result = false;
  CPUNIT_DTRACE("ConformityChecker - path.matches --> "<<path.matches(ri.get_path()));
  CPUNIT_DTRACE("ConformityChecker - name.matches --> "<<name.matches(ri.get_name()));
  CPUNIT_DTRACE("ConformityChecker - file.matches --> "<<file.matches(ri.get_file()));
  if (path.matches(ri.get_path()) &&
      name.matches(ri.get_name()) &&
      file.matches(ri.get_file())) {
    result = (!file_eq_path) || check_file_path_match(ri);
  }
  CPUNIT_DTRACE("ConformityChecker - was "<<result);
  return result;
}

bool 
cpunit::ConformityChecker::check_file_path_match(const RegInfo &ri) const {  
  const std::string rp = ri.get_path();
  std::ostringstream pattern;
  pattern<<'*';
  for (std::size_t i=0; i<rp.size(); i++) {
    if (rp[i] == ':') {
      pattern<<'_';
      i++;
    } else {
      pattern<<rp[i];
    }
  }
  pattern<<'.'<<extension;
  const GlobMatcher tail_match(pattern.str());
  const bool result = tail_match.matches(ri.get_file());
  CPUNIT_DTRACE("ConformityChecker - path confirmity was "<<result);
  return result;
}
