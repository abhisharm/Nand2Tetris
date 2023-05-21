#define BOOST_TEST_MODULE parser test
#include <boost/test/included/unit_test.hpp>

#include "parser.h"

#include <iostream>

using std::istringstream;
using std::string;
using Translator::Parser;
using Translator::CommandType;

BOOST_AUTO_TEST_CASE( test_has_more_lines ) {
  string line = "push constant 5"
    "\n        \n"
    "\npush constant 10"
    "\nadd"; 
  istringstream stream(line);
  Parser parser(&stream);
  BOOST_CHECK( parser.has_more_lines() );
}

BOOST_AUTO_TEST_CASE( test_advance ) {
  string line = "push constant 5"
    "\npush constant 10"
    "\n     \n   "
    "\nadd"; 
  istringstream stream(line);
  Parser parser(&stream);
  BOOST_CHECK( parser.has_more_lines() );
  parser.advance();
  BOOST_CHECK( parser.has_more_lines() );
  parser.advance();
  BOOST_CHECK( parser.has_more_lines() );
  parser.advance();
  BOOST_CHECK( !parser.has_more_lines() );
}

BOOST_AUTO_TEST_CASE( test_full ) {
  string line = "push constant 5\n"
    "//test comment    "
    "\npop lcl 3    // comment"
    "\n      \n     "
    "\nadd // comment"; 
  istringstream stream(line);
  Parser parser(&stream);
  BOOST_CHECK( parser.has_more_lines() );
  parser.advance();
  BOOST_CHECK( parser.command_type() == CommandType::C_PUSH );
  BOOST_CHECK( parser.arg1() == "constant" );
  BOOST_CHECK( parser.arg2() == 5 );
  BOOST_CHECK( parser.has_more_lines() );
  parser.advance();
  BOOST_CHECK( parser.command_type() == CommandType::C_POP );
  BOOST_CHECK( parser.arg1() == "lcl" );
  BOOST_CHECK( parser.arg2() == 3 );
  BOOST_CHECK( parser.has_more_lines() );
  parser.advance();
  BOOST_CHECK( parser.command_type() == CommandType::C_ARITHMETIC );
  BOOST_CHECK( parser.arg1() == "add" );
  BOOST_CHECK( !parser.has_more_lines() );
}
