#define BOOST_TEST_MODULE file_processor test
#include <boost/test/included/unit_test.hpp>

#include <optional>
#include <vector>
#include <memory>
#include <sstream>
#include <istream>

#include "file_processor.h"

using assembler::FileProcessor;

BOOST_AUTO_TEST_CASE( general_test ) {
  std::string input = "// This is a comment\n";
  input += "   \n      ";
  input += "@a\n";
  input += "D=0;JEQ // following comment\n";
  input += "// Another comment";
  std::istringstream str(input);
  std::vector<std::string> output;
  BOOST_CHECK( FileProcessor::clean_input(str, &output) );
  BOOST_CHECK( output.size() == 2 );
  BOOST_CHECK(output[0].length() == 2);
  BOOST_CHECK(output[1].length() == 7);
}
