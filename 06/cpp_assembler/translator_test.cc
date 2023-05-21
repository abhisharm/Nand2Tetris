#define BOOST_TEST_MODULE assembler test
#include <boost/test/included/unit_test.hpp>

#include <variant>
#include <string>
#include <vector>
#include <iostream>

#include "translator.h"

using assembler::Assembler;
using assembler::AInstruction;
using assembler::CInstruction;
using assembler::Label;
using assembler::Translator;
using std::cout;

BOOST_AUTO_TEST_CASE( test_add ) {
  AInstruction first;
  first.address = 2;
  CInstruction second;
  second.cmp = "A";
  second.dst = "D";
  AInstruction third;
  third.address = 3;
  CInstruction fourth;
  fourth.cmp = "D+A";
  fourth.dst = "D";
  AInstruction fifth;
  fifth.address = 0;
  CInstruction sixth;
  sixth.cmp = "D";
  sixth.dst = "M";
  std::vector<std::variant<AInstruction, CInstruction, Label>> instructions =
    { first, second, third, fourth, fifth, sixth };     
  Assembler assembler { instructions };
  BOOST_CHECK( assembler.assemble() );
  Translator translator { &assembler };
  std::string result = translator.Translate();
  std::string cmp = 
    "0000000000000010\n"
    "1110110000010000\n"
    "0000000000000011\n"
    "1110000010010000\n"
    "0000000000000000\n"
    "1110001100001000";
  BOOST_CHECK( result == cmp );
}
