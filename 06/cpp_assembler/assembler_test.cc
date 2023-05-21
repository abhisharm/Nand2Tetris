#define BOOST_TEST_MODULE assembler test
#include <boost/test/included/unit_test.hpp>

#include <variant>
#include <string>
#include <vector>
#include <iostream>

#include "assembler.h"

using assembler::Assembler;
using assembler::AInstruction;
using assembler::CInstruction;
using assembler::Label;
using std::cout;

BOOST_AUTO_TEST_CASE( test_no_variables_remove_label ) {
  AInstruction first;
  first.address = 0;
  AInstruction second;
  second.address = 1;
  CInstruction third;
  third.cmp = "M+D";
  third.dst = "M";
  Label fourth;
  fourth.name = "END";
  CInstruction fifth;
  fifth.cmp = "0";
  fifth.jmp = "JEQ"; 
  std::vector<std::variant<AInstruction, CInstruction, Label>> instructions =
    { first, second, third, fourth, fifth };     
  Assembler assembler { instructions };
  BOOST_CHECK( assembler.assemble() );
  std::vector<std::variant<AInstruction, CInstruction>>* commands =
    assembler.GetFinalCommands();
  BOOST_CHECK( commands->size() == 4 );
}

BOOST_AUTO_TEST_CASE( test_with_variables ) {
  AInstruction first;
  first.address = 0;
  AInstruction second;
  second.variable = "tmp";
  CInstruction third;
  third.cmp = "M+D";
  third.dst = "M";
  Label fourth;
  fourth.name = "END";
  CInstruction fifth;
  fifth.cmp = "0";
  fifth.jmp = "JEQ"; 
  AInstruction sixth;
  sixth.variable = "tmp";
  std::vector<std::variant<AInstruction, CInstruction, Label>> instructions =
    { first, second, third, fourth, fifth, sixth };     
  Assembler assembler { instructions };
  BOOST_CHECK( assembler.assemble() );

  std::vector<std::variant<AInstruction, CInstruction>>* commands =
    assembler.GetFinalCommands();
  
  BOOST_CHECK( commands->size() == 5 );
  BOOST_CHECK(std::holds_alternative<AInstruction>((*commands)[1]));
  std::variant<AInstruction, CInstruction> command = (*commands)[1];
  AInstruction a_instruction = std::get<AInstruction>(command);
  BOOST_CHECK(a_instruction.address == 16);
  BOOST_CHECK(std::holds_alternative<AInstruction>((*commands)[4]));
  command = (*commands)[4];
  a_instruction = std::get<AInstruction>(command);
  BOOST_CHECK(a_instruction.address == 16);
}

BOOST_AUTO_TEST_CASE( test_with_variables_and_default_symbols ) {
  AInstruction first;
  first.variable = "R14";
  AInstruction second;
  second.variable = "tmp";
  CInstruction third;
  third.cmp = "M+D";
  third.dst = "M";
  Label fourth;
  fourth.name = "END";
  CInstruction fifth;
  fifth.cmp = "0";
  fifth.jmp = "JEQ"; 
  AInstruction sixth;
  sixth.variable = "tmp";
  std::vector<std::variant<AInstruction, CInstruction, Label>> instructions =
    { first, second, third, fourth, fifth, sixth };     
  Assembler assembler { instructions };
  BOOST_CHECK( assembler.assemble() );

  std::vector<std::variant<AInstruction, CInstruction>>* commands =
    assembler.GetFinalCommands();
  
  BOOST_CHECK( commands->size() == 5 );
  BOOST_CHECK(std::holds_alternative<AInstruction>((*commands)[0]));
  std::variant<AInstruction, CInstruction> command = (*commands)[0];
  AInstruction a_instruction = std::get<AInstruction>(command);
  BOOST_CHECK(a_instruction.address == 14);
  BOOST_CHECK(std::holds_alternative<AInstruction>((*commands)[1]));
  command = (*commands)[1];
  a_instruction = std::get<AInstruction>(command);
  BOOST_CHECK(a_instruction.address == 16);
  BOOST_CHECK(std::holds_alternative<AInstruction>((*commands)[4]));
  command = (*commands)[4];
  a_instruction = std::get<AInstruction>(command);
  BOOST_CHECK(a_instruction.address == 16);
}

BOOST_AUTO_TEST_CASE( test_with_variables_and_label ) {
  AInstruction first;
  first.address = 0;
  AInstruction second;
  second.variable = "TEST";
  CInstruction third;
  third.cmp = "M+D";
  third.dst = "M";
  Label fourth;
  fourth.name = "END";
  Label fifth;
  fifth.name = "TEST";
  AInstruction sixth;
  sixth.variable = "TEST";
  std::vector<std::variant<AInstruction, CInstruction, Label>> instructions =
    { first, second, third, fourth, fifth, sixth };     
  Assembler assembler { instructions };
  BOOST_CHECK( assembler.assemble() );

  std::vector<std::variant<AInstruction, CInstruction>>* commands =
    assembler.GetFinalCommands();
  BOOST_CHECK( commands->size() == 4 );
  BOOST_CHECK(std::holds_alternative<AInstruction>((*commands)[1]));
  std::variant<AInstruction, CInstruction> command = (*commands)[1];
  AInstruction a_instruction = std::get<AInstruction>(command);
  BOOST_CHECK(a_instruction.address == 3);
  BOOST_CHECK(std::holds_alternative<AInstruction>((*commands)[4]));
  command = (*commands)[3];
  a_instruction = std::get<AInstruction>(command);
  BOOST_CHECK(a_instruction.address == 3);
}
// EOF
