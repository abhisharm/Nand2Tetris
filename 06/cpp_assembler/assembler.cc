#include <variant>
#include <string>
#include <optional>
#include <iostream>

#include "assembler.h"


namespace assembler {

Assembler::Assembler(
  std::vector<std::variant<AInstruction, CInstruction, Label>> commands) :
  commands_(commands) {}

bool Assembler::assemble() {
  if (!first_pass()) {
    return false;
  } else {
    return second_pass();
  }
}

bool Assembler::first_pass() {
  if (first_pass_done_) {
    return true;
  }

  int64_t command_index = 0;
  for (std::variant<AInstruction, CInstruction, Label> command : commands_) {
    if (std::holds_alternative<Label>(command)) {
      Label label = std::get<Label>(command);
      if (!symbol_table_.AddSymbol(label.name, command_index)) {
        return false;
      }
    } else {
      if (std::holds_alternative<AInstruction>(command)) {
        AInstruction a_instruction = std::get<AInstruction>(command);
        intermediate_commands_.emplace_back(a_instruction);
      } else if (std::holds_alternative<CInstruction>(command)) {
        CInstruction c_instruction = std::get<CInstruction>(command);
        intermediate_commands_.emplace_back(c_instruction);
      }

      ++command_index;
    }
  }

  first_pass_done_ = true;
  return true;
}

bool Assembler::second_pass() {
  if (!first_pass_done_) {
    return false;
  }

  if (second_pass_done_) {
    return true;
  }

  int64_t var_counter = 16;
  for (std::variant<AInstruction, CInstruction> command : intermediate_commands_) {
    if (std::holds_alternative<AInstruction>(command)) {
      AInstruction a_instruction = std::get<AInstruction>(command);
      if (a_instruction.variable != "") {
        std::optional<int64_t> value =
          symbol_table_.GetSymbol(a_instruction.variable);
        if (value != std::nullopt) {
          a_instruction.variable = "";
          a_instruction.address = value.value();
        } else {
          symbol_table_.AddSymbol(a_instruction.variable, var_counter);
          a_instruction.variable = "";
          a_instruction.address= var_counter;
          var_counter++;
        }
      }
      final_commands_.emplace_back(a_instruction);
    } else {
      CInstruction c_instruction = std::get<CInstruction>(command);
      final_commands_.emplace_back(c_instruction);
    }
  }

  second_pass_done_ = true;
  return true;
}

std::vector<std::variant<AInstruction, CInstruction>>*
  Assembler::GetFinalCommands() {
  return &final_commands_;
}
}
