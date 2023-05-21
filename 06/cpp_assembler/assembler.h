#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <variant>
#include <vector>
#include "instruction.h"
#include "label.h"
#include "symbol_table.h"

namespace assembler {
  class Assembler {
    public:
      // Constructor for Assembler. Able to assemble a single program.
      // Stores the given list of commands.
      Assembler(
        std::vector<std::variant<AInstruction, CInstruction, Label>> commands);

      // Runs the 2-pass asembler algorithm on the input list of instructions
      // (which may have symbols). Returns true if successful, false otherwise.
      bool assemble();

      // Returns the final vector of instructions
      std::vector<std::variant<AInstruction, CInstruction>>* GetFinalCommands();

    private:
      // Runs the first-pass of the assembler algorithm on the input list
      // of commands. Determines the values of all label symbols and stores
      // them in the symbol-table.
      bool first_pass();

      // Run the second-pass of the assembler algorithm on the input list of
      // of commands. Populates all A-Instruction's with variables to their
      // values or the associated label.
      bool second_pass();

      bool first_pass_done_ = false;
      bool second_pass_done_ = false;
      SymbolTable symbol_table_;
      std::vector<std::variant<AInstruction, CInstruction, Label>> commands_;
      std::vector<std::variant<AInstruction, CInstruction>> intermediate_commands_;
      std::vector<std::variant<AInstruction, CInstruction>> final_commands_;
  };
}
#endif
