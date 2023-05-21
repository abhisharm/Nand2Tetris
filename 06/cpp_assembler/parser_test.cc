#define BOOST_TEST_MODULE parser test
#include <boost/test/included/unit_test.hpp>

#include "parser.h"

#include <iostream>

using assembler::Parser;
using assembler::Instruction;
using assembler::AInstruction;
using assembler::CInstruction;
using assembler::Label;

BOOST_AUTO_TEST_CASE( general_test ) {
  // Set up
  std::string a_instruction = "@test";
  std::string a_instruction_2 = "@17";
  std::string c_instruction = "D=M+A;JEQ";
  std::string c_instruction_2 = "D-M;JLT";
  std::string label = "(GO)";
  std::vector<std::string> cleaned_lines =
    { a_instruction, a_instruction_2, c_instruction, c_instruction_2, label };
  std::vector<std::variant<AInstruction, CInstruction, Label>> output;

  // Parse all instructions
  BOOST_CHECK( Parser::ParseInstructions(cleaned_lines, &output) );
  BOOST_CHECK( output.size() == 5);
  BOOST_CHECK( std::holds_alternative<AInstruction>(output[0]) );

  // Parse A Instruction with variable
  AInstruction a = std::get<AInstruction>(output[0]);
  BOOST_CHECK( a.variable == "test" );

  // Parse A Instruction with an address
  BOOST_CHECK( std::holds_alternative<AInstruction>(output[1]) );
  a = std::get<AInstruction>(output[1]);
  BOOST_CHECK( a.address == 17 );
  BOOST_CHECK( a.variable == "" );

  // Parse C Instruction
  BOOST_CHECK( std::holds_alternative<CInstruction>(output[2]) );
  CInstruction c = std::get<CInstruction>(output[2]);
  BOOST_CHECK( c.dst == "D" );
  BOOST_CHECK( c.cmp == "M+A" );
  BOOST_CHECK( c.jmp == "JEQ" );

  // Parse C Instruction without a dst
  BOOST_CHECK( std::holds_alternative<CInstruction>(output[3]) );
  c = std::get<CInstruction>(output[3]);
  BOOST_CHECK( c.dst == "" );
  BOOST_CHECK( c.cmp == "D-M" );
  BOOST_CHECK( c.jmp == "JLT" );

  // Parse a label
  BOOST_CHECK( std::holds_alternative<Label>(output[4]) );
  Label l = std::get<Label>(output[4]);
  BOOST_CHECK( l.name == "GO" );
}

