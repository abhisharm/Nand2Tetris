#include <string>
#include <variant>
#include <bitset>
#include <iostream>

#include "translator.h"

namespace assembler {

Translator::Translator(Assembler* assembler) :
  assembler_(assembler) {}

std::string Translator::Translate() {
  std::string to_return;
  std::vector<std::variant<AInstruction, CInstruction>>* final_commands =
    assembler_->GetFinalCommands();
  for (auto command : *final_commands) {
    if (std::holds_alternative<AInstruction>(command)) {
      AInstruction a_instruction = std::get<AInstruction>(command);
      std::string translated = translate_a_instruction(a_instruction);
      to_return += translated + "\n";
    } else {
      CInstruction c_instruction = std::get<CInstruction>(command);
      std::string translated = translate_c_instruction(c_instruction);
      to_return += translated + "\n";
    }
  }

  if (to_return.size() > 0) {
    // remove the final 'newline' character
    return to_return.substr(0, to_return.size() - 1);
  } else {
    return to_return;
  }
}

std::string Translator::translate_a_instruction(AInstruction a_instruction) {
  // convert the address to binary
  std::bitset<15> bitset { a_instruction.address };

  // convert to string and add an opcode
  return "0" + bitset.to_string();
}

std::string Translator::translate_c_instruction(CInstruction c_instruction) {
  std::string to_return = translate_c_cmp(c_instruction.cmp);
  to_return += translate_c_dst(c_instruction.dst);
  to_return += translate_c_jmp(c_instruction.jmp);
  return "111" + to_return;
}

std::string Translator::translate_c_cmp(std::string cmp) {
  if (cmp == "0") {
  	return "0101010";
  }

  if (cmp == "1") {
  	return "0111111";
  }

  if (cmp == "-1") {
  	return "0111010";
  }

  if (cmp == "D") {
  	return "0001100";
  }

  if (cmp == "A") {
  	return "0110000";
  }

  if (cmp == "M") {
  	return "1110000";
  }

  if (cmp == "!D") {
  	return "0001101";
  }

  if (cmp == "!A") {
  	return "0110001";
  }

  if (cmp == "!M") {
  	return "1110001";
  }

  if (cmp == "-D") {
  	return "0001111";
  }

  if (cmp == "-A") {
  	return "0110011";
  }

  if (cmp == "-M") {
  	return "1110011";
  }

  if (cmp == "D+1") {
  	return "0011111";
  }

  if (cmp == "A+1") {
  	return "0110111";
  }

  if (cmp == "M+1") {
  	return "1110111";
  }

  if (cmp == "D-1") {
  	return "0001110";
  }

  if (cmp == "A-1") {
  	return "0110010";
  }

  if (cmp == "M-1") {
  	return "1110010";
  }

  if (cmp == "D+A") {
  	return "0000010";
  }

  if (cmp == "D+M") {
  	return "1000010";
  }

  if (cmp == "D-A") {
  	return "0010011";
  }

  if (cmp == "D-M") {
  	return "1010011";
  }

  if (cmp == "A-D") {
  	return "0000111";
  }

  if (cmp == "M-D") {
  	return "1000111";
  }

  if (cmp == "D&A") {
  	return "0000000";
  }
  
  if (cmp == "D&M") {
  	return "1000000";
  }

  if (cmp == "D|A") {
  	return "0010101";
  }

  if (cmp == "D|M") {
	return "1010101";
  }

  // SHOULD NOT REACH (cmp should never be empty)
  return "";
}

std::string Translator::translate_c_dst(std::string dst) {
  if (dst == "") {
    return "000";
  }

  if (dst == "M"){
    return "001";
  }

  if (dst == "D"){
    return "010";
  }

  if (dst == "MD"){
    return "011";
  }

  if (dst == "A"){
    return "100";
  }

  if (dst == "AM"){
    return "101";
  }

  if (dst == "AD"){
    return "110";
  }

  if (dst == "AMD"){
    return "111";
  }

  // SHOULD NOT REACH
  return "";
}

std::string Translator::translate_c_jmp(std::string jmp) {
  if (jmp == "") {
    return "000";
  }
  if (jmp == "JGT"){
    return "001";
  }
  if (jmp == "JEQ"){
    return "010";
  }
  if (jmp == "JGE"){
    return "011";
  }
  if (jmp == "JLT"){
    return "100";
  }
  if (jmp == "JNE"){
    return "101";
  }
  if (jmp == "JLE"){
    return "110";
  }
  if (jmp == "JMP"){
    return "111";
  }

  // SHOULD NOT REACH
  return "";
}
}
