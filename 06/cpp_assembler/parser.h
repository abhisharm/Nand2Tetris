#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <variant>
#include "instruction.h"
#include "label.h"

namespace assembler {

  // Represents a parser that is meant to tokenize the input lines and return
  // A vector of 'instruction's (defined in the common header-file.
  class Parser {
    public:
      // Parse a vector of instructions (already cleaned and extracted
      // line-by-line from the input file). There should be no empty lines.
      // Return false if there was an error in parsing. True otherwise.
      static bool ParseInstructions(
        const std::vector<std::string>& input,
        std::vector<std::variant<AInstruction, CInstruction, Label>>* output
      ); 
  };

}
#endif
