#include "parser.h"

#include <string>
#include <variant>
#include <stdexcept>
#include <iostream>

namespace assembler {

using std::cout;

bool Parser::ParseInstructions(
  const std::vector<std::string>& input,
  std::vector<std::variant<AInstruction, CInstruction, Label>>* output) {
  for (auto instruction : input) {
    if (instruction.length() == 0) {
      return false;
    }

    if (instruction[0] == '@') {
      // parse A Instruction
      AInstruction a_instruction;
      std::string variable_or_value;
      for (int i = 1; i < instruction.length(); i++) {
        variable_or_value += instruction[i];
      }

      // determine if instruction refers to a variable or address
      int64_t val;
      try {
        val = std::stoi(variable_or_value);
        a_instruction.address = val;
      } catch (std::invalid_argument const& ex) {
        a_instruction.variable = variable_or_value;
      }

      output->push_back(a_instruction);
    } else if (instruction[0] == '(') {
      // parse Label
      std::string name;
      for (int i = 1; i < instruction.length() && instruction[i] != ')'; i++) {
         name += instruction[i];
      }
      Label label {name}; 

      output->push_back(label);
    } else {
      // parse C instruction
      CInstruction c_instruction;

      // find dst
      size_t loc_eq = instruction.find('=');
      if (loc_eq != std::string::npos) {
        std::string dst;
        for (int i = 0; i < loc_eq; ++i) {
          dst += instruction[i];
        }
        c_instruction.dst = dst; 
      } else {
        loc_eq = -1;
      }

      // Find cmp
      size_t loc_sem = instruction.find(';');
      if (loc_sem == std::string::npos) {
        loc_sem = instruction.length();
      } 
      std::string cmp;
      for (int i = loc_eq + 1; i < loc_sem; ++i) {
        cmp += instruction[i];
      }
      c_instruction.cmp = cmp; 

      // Find jmp
      std::string jmp;
      for (int i = loc_sem + 1; i < instruction.length(); ++i) {
        jmp += instruction[i]; 
      }
      c_instruction.jmp = jmp;

      // Validating that we have a cmp if there is a jmp
      if (c_instruction.jmp != "" && c_instruction.cmp == "") {
        return false;
      }

      // Validating that we have some instruction (pure whitespace lines not allowed)
      if (c_instruction.cmp == "" && 
          c_instruction.jmp == "" &&
          c_instruction.dst == "") {
        return false;
      }

      output->push_back(c_instruction);
    }
  }

  // If we got through all instruction successfully, return true
  return true;
}

}
