#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <string>

namespace assembler {

enum InstructionType {
  A_INSTRUCTION = 0,
  C_INSTRUCTION = 1,
};

class Instruction {
  public:
    // Returns the type of this instruction.
    virtual InstructionType GetInstructionType() = 0;
};

class AInstruction : Instruction {
  public:
    InstructionType GetInstructionType() override {
      return InstructionType::A_INSTRUCTION;
    }

    std::string variable;
    int64_t address;
};

class CInstruction : Instruction{
  public:
    InstructionType GetInstructionType() override {
      return InstructionType::C_INSTRUCTION;
    }

    std::string cmp;
    std::string dst;
    std::string jmp;  
};

}
#endif
