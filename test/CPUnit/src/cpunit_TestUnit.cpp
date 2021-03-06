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



#include "cpunit_TestUnit.hpp"
#include <string>

cpunit::TestUnit::TestUnit(Callable *_setUp, Callable *_tearDown, Callable *_test)
  : setUp(_setUp)
  , tearDown(_tearDown)
  , test(_test)
{}

cpunit::TestUnit::~TestUnit()
{}

void
cpunit::TestUnit::run_set_up() {
  if (setUp != NULL) {
    setUp->run();
  }
}

void
cpunit::TestUnit::run_test() {
  if (test != NULL) {
    test->run();
  }
}

cpunit::Callable*
cpunit::TestUnit::get_test() {
  return test;
}

cpunit::Callable*
cpunit::TestUnit::get_tear_down() {
  return tearDown;
}

cpunit::SafeTearDown::SafeTearDown(Callable *td)
  : tear_down(td)
{}

cpunit::SafeTearDown::~SafeTearDown() {
  if (tear_down != NULL) {
    tear_down->run();
  }
}

bool 
cpunit::TestUnit::lexical_cmp(const TestUnit &a, const TestUnit &b) {
  const std::string sa = a.test->get_reg_info().get_path() + "::" + a.test->get_reg_info().get_name();
  const std::string sb = b.test->get_reg_info().get_path() + "::" + b.test->get_reg_info().get_name();
  return sa < sb;
}
