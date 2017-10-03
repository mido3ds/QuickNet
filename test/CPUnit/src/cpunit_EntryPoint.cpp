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

#include "cpunit_AssertionException.hpp"
#include "cpunit_TestStore.hpp"
#include "cpunit_StringFlyweightStore.hpp"
#include "cpunit_TestExecutionFacade.hpp"
#include "cpunit_ExecutionReport.hpp"
#include "cpunit_ErrorReportFormat.hpp"
#include "cpunit_RegInfo.hpp"
#include "cpunit_ConformityChecker.hpp"
#include "cpunit_WrongSetupException.hpp"
#include "cpunit_CmdLineParser.hpp"
#include "cpunit_TimeFormat.hpp"
#include "cpunit_trace.hpp"
#include "cpunit_EntryPoint.hpp"
#include "cpunit_impl_BootStream.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace cpunit {

  namespace {
  
    using namespace std;
  
    void usage() {
      cout<<endl;
      cout<<"Usage: <test_executable> [option]* [test-pattern]*"<<endl;
      cout<<"where [test-pattern] is a glob-pattern matching tests to run (the default is '*'), and [option] is any of the following: "<<endl;
      cout<<endl;
      cout<<"    -h          - Display this message and exit. All other options are ignored."<<endl;
      cout<<endl;
      cout<<"    -L          - List registered tests matching the patterns and exit. All other options are ignored."<<endl;
      cout<<endl;
      cout<<"    -v          - Verbose mode."<<endl;
      cout<<endl;
      cout<<"    -V          - Version information."<<endl;
      cout<<endl;
      cout<<"    -a          - Robust mode, runs all tests (default is to stop on first error)."<<endl;
      cout<<endl;
      cout<<"    -f=<format> - Error format specification (used for reporting in robust mode):"<<endl;
      cout<<"                   %N - newline"<<endl;
      cout<<"                   %T - tab"<<endl;
      cout<<"                   %e - error type (OK, FAILURE, ERROR)"<<endl;
      cout<<"                   %p - suite name"<<endl;
      cout<<"                   %n - test name"<<endl;
      cout<<"                   %t - test time"<<endl;
      cout<<"                   %f - file name"<<endl;
      cout<<"                   %l - line number where test is registered"<<endl;
      cout<<"                   %m - error message"<<endl;  
      cout<<endl;
      cout<<"                   Default is '%p::%n - %m (%ts)%N(Registered at %f:%l)'."<<endl;
      cout<<endl;
      cout<<"    <pattern>  - Which tests to run. Supports name globbing with wildcards, e.g. 'SomeScope::*::test_*_OK'"<<endl;
      cout<<"                 Default is '*'."<<endl;
      cout<<endl;
      cout<<"Examples:"<<endl;
      cout<<"    ./test_runner 'cpunit::*' -- Run all tests in the 'cpunit' namespace."<<endl;
      cout<<"    ./test_runner 'unit*' 'io*' -- Run all tests starting with 'unit', and then all tests starting with 'io'."<<endl;
      cout<<"    ./test_runner -a            -- Try to run all tests, even if there are errors."<<endl;
      cout<<"    ./test_runner -av           -- Run all tests in verbose mode, printing the name of each test just prior to execution."<<endl;
      cout<<"    ./test_runner -af=%e%p::%n  -- Run all tests and display error messages as e.g. 'FAILURE MyTests::test_stuff'."<<endl;
      cout<<endl;
      cout<<"Experimental options:"<<endl;
      cout<<endl;
      cout<<"    --max-time=<time> - Specify the maximal legal running time for unit tests in seconds."<<endl;
      cout<<"                        Decimal number values are accepted, e.g. --max-time=0.5"<<endl;
      cout<<"                        Notice that, this will not cause the test to be broken off. All tests will"<<endl;
      cout<<"                        execute until completion, and tests that spend longer time that this value"<<endl;
      cout<<"                        are reported as FAILED."<<endl;
      cout<<endl;
      cout<<"                        Default max-time is 1.0e10, i.e. about 317 years."<<endl;
      cout<<endl;
      cout<<"    --conformity - Check all registered test for name conformities:"<<endl;
      cout<<"                   * For match rules, see 'conformity-format' below"<<endl;
      cout<<"                   * All test suites must reside in files with reflecting names, i.e. if the suite is named"<<endl;
      cout<<"                     'SomeScope::MyTest', the files short name must match 'SomeScope_MyTest.*'"<<endl;
      cout<<"                     That is, '::' is exchanged with '_'."<<endl;
      cout<<endl;
      cout<<"    --conformity-format - Specify how conformity checking is to be performed. Must be glob-patterns."<<endl;
      cout<<"                         * There must be three tokens, each separated by pipe symbols"<<endl;
      cout<<"                         * First token is used to check suite names"<<endl;
      cout<<"                         * Second token is used to check test names"<<endl;
      cout<<"                         * Third token is used to check test file extensions"<<endl;
      cout<<endl;
      cout<<"                           Default is '*Test|test_*|*'."<<endl;
    }

    const std::string conformity_token("--conformity");
    const std::string conformity_format_token("--conformity-format");
    const std::string error_format_token("-f");
    const std::string robust_token("-a");
    const std::string max_time_token("--max-time");

    void print_version_info() {
      cout<<"CPUnit version 0.95 (beta)."<<endl;
      cout<<"Get the latest version at http://cpunit.sourceforge.net/"<<endl;
    }

    std::vector<std::string> get_conformity_format(const CmdLineParser &p) {
      const char sep = '|';
      std::vector<std::string> result;
      std::ostringstream oss;
      const std::string spec = p.value_of<string>(conformity_format_token);
      CPUNIT_ITRACE("EntryPoint - Conformity format: '"<<spec<<'\'');
      for (std::size_t i = 0; i<spec.length(); i++) {
	if (spec[i] == sep) {
	  result.push_back(oss.str());
	  oss.str("");
	} else {
	  oss<<spec[i];
	}
      }
      result.push_back(oss.str());
      if (result.size() != 3) {
	oss.str("");
	oss<<"Wrong specification of conformity format: "<<spec;
	throw cpunit::WrongSetupException(oss.str());
      }
      return result;
    }
      
    bool report_result(const std::vector<cpunit::ExecutionReport> &result, const std::string &format, ostream &out) {
      CPUNIT_ITRACE("EntryPoint - Reporting result with error report format '"<<format<<'\'');
      const cpunit::ErrorReportFormat formatter(format);
      int errors = 0;
      double time_spent = 0;
      for (std::size_t i=0; i<result.size(); i++) {
	if (result[i].get_execution_result() != cpunit::ExecutionReport::OK) {
	  CPUNIT_DTRACE("EntryPoint - Reporting error for "<<result[i].get_test().to_string());
	  out<<std::endl<<formatter.format(result[i])<<std::endl;
	  errors++;
	}
	time_spent += result[i].get_time_spent();
      }
      out<<std::endl<<"Time: "<<std::setprecision(3)<<cpunit::TimeFormat(time_spent)<<std::endl;
      out<<std::endl;
      if (errors == 0) {
	out<<"OK ("<<result.size()<<" tests)"<<std::endl;
      } else {
	out<<"FAILURE!!! ("<<errors<<" out of "<<result.size()<<" tests failed)"<<std::endl;
      }
      return errors == 0;
    }

    void list_tests(const std::vector<std::string> &patterns) {
      std::vector<cpunit::RegInfo> tests;
      for (std::size_t i=0; i<patterns.size(); i++) {
	std::vector<cpunit::RegInfo> p_tests = cpunit::TestStore::get_instance().get_tests(patterns[i]);
	tests.insert(tests.end(), p_tests.begin(), p_tests.end());
      }
      for (std::size_t i=0; i<tests.size(); i++) {
	std::cout<<tests[i].get_path()<<"::"<<tests[i].get_name()<<std::endl;
      }
      std::cout<<tests.size()<<" tests in total."<<std::endl;
    }

    int conformity_report(const std::vector<std::string> &tests_to_check_patterns, 
			  const std::string path_pattern, 
			  const std::string name_pattern, 
			  const std::string extension_pattern, 
			  const bool verbose, ostream &out) {
      int finds = 0;
      for (std::size_t i=0; i<tests_to_check_patterns.size(); ++i) {
	std::string tests_to_check_pattern = tests_to_check_patterns[i];
	if (verbose) {
	  out<<"Running conformity check with patterns ["<<path_pattern<<"], ["<<name_pattern<<"], [";
	  out<<extension_pattern<<"] on tests matching ["<<tests_to_check_pattern<<']'<<std::endl;
	}
	const vector<cpunit::RegInfo> tests = cpunit::TestStore::get_instance().get_tests(tests_to_check_pattern);
	cpunit::ConformityChecker cc(path_pattern, name_pattern);
	cc.set_extension(extension_pattern);
	for (std::size_t i=0; i<tests.size(); i++) {
	  if (verbose) {
	    out<<"Conformity check on "<<tests[i].to_string()<<std::endl;
	  }
	  if (!cc.matches(tests[i])) {
	    out<<"NON CONFORMITY DETECTED:"<<std::endl<<tests[i].to_string()<<std::endl;
	    finds++;
	  }
	}
      }
      return finds;
    }

    struct AutoDisposer {
      ~AutoDisposer() {
	CPUNIT_ITRACE("AutoDisposer::~AutoDisposer() - Disposing the TestStore.");
	cpunit::TestStore::dispose();
	CPUNIT_ITRACE("AutoDisposer::~AutoDisposer() - Disposing the StringFlyweightStore.");
	cpunit::StringFlyweightStore::dispose();
	CPUNIT_ITRACE("AutoDisposer::~AutoDisposer() - Disposing the BootStream.");
	cpunit::impl::BootStream::dispose();
      }
    };
  }

  /**
   * Creates a command line parser initialized with default values.
   * @return a command line parser initialized with default values.
   */
  CmdLineParser get_cmd_line_parser() {
    CmdLineParser parser;
    parser.add_legal("-h --help -L --list -v --verbose -V --version -a --all -f --conformity --conformity-format --max-time");
    const int argc = 3;
    const char *defaults[argc] = {
      "--conformity-format=*Test|test_*|*",
      "-f=%p::%n - %m (%ts)%N(Registered at %f:%l)",
      "--max-time=1e+10",
    };
    parser.parse(argc, defaults);
    CPUNIT_ITRACE("EntryPoint - Default arguments:"<<parser.to_string());
    return parser;
  }


  /**
   * Forward your program parameters directly from main to this function to start CPUnit.
   *  
   * @param argc The number of arguments, including the program name.
   * @param args The program arguments, including the program name.
   * @return 0 if all tests succeed, and >0 elsewise.
   */
  int main(const int argc, const char **args) {
    CmdLineParser parser = get_cmd_line_parser();
    parser.parse(argc-1, args+1);
    return main(parser);
  }

  /**
   * If you have extra command line parameters, you may use this method to start CPUnit
   * after you have done your own. You should use {@link #get_cmd_line_parser()} to obtain
   * a parser which is already set up to accept the CPUnit specific command line parameters,
   * and then use CmdLineParser.add_legal to specify the arguments you expect in addition.
   * 
   * @param parser A parser contaiing the command line arguments, excluding the program name.
   *               I.e. from main(int& argc,char** args), the parser would be called as
   *               {@link CmdLineParser#parse(const int, const char**) parser.parse(arcg-1,args+1)}.
   * @return 0 if all tests succeed, and >0 elsewise.
  */
  int main(const CmdLineParser &parser) {

    AutoDisposer ad;
    try {
      CPUNIT_ITRACE("EntryPoint - Actual arguments:"<<parser.to_string());

      std::vector<std::string> patterns = parser.program_input();
      if (patterns.size() == 0) { 
	patterns.push_back("*");
      }
      
      if(parser.has_one_of("-h --help")) {
	usage();
	return 0;
      }
      if(parser.has_one_of("-L --list")) {
	list_tests(patterns);
	return 0;
      }
      if(parser.has_one_of("-V --version")) {
	print_version_info();
	return 0;
      }
      
      const bool verbose    = parser.has("-v") || parser.has("--verbose");
      const bool robust     = parser.has("-a") || parser.has("--all");
      const bool conformity = parser.has("--conformity");
      
      CPUNIT_ITRACE("EntryPoint - verbose="<<verbose<<" robust="<<robust<<" conformity="<<conformity);
      
      bool conform = true;
      if (conformity) {
	std::vector<std::string> conf_patterns = get_conformity_format(parser);
	const int num_conformity_breaches = conformity_report(patterns, conf_patterns[0], conf_patterns[1], conf_patterns[2], verbose, std::cout);
	std::cout<<"There where "<<num_conformity_breaches<<" conformity breaches."<<std::endl;
	conform = num_conformity_breaches == 0;
      }

      const std::string report_format = parser.value_of<std::string>(error_format_token);
      const double max_time = parser.value_of<double>(max_time_token);
      const std::vector<cpunit::ExecutionReport> result = cpunit::TestExecutionFacade().execute(patterns, max_time, verbose, robust);
      bool all_well = report_result(result, report_format, std::cout);
      
      int exit_value = 0;
      if (!all_well) {
	exit_value |= 0x1;
      }
      if(!conform) {
	exit_value |= 0x2;
      }
      return exit_value;
    } catch (AssertionException &e) {
      std::cout<<"Terminated due to AssertionException: "<<std::endl<<e.what()<<std::endl;
      return 1;
    }
  }
}
