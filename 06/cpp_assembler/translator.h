#include <string>

#include "assembler.h"

namespace assembler {

class Translator {
  public:
    Translator(Assembler* assembler);

    // Outputs the string from an assembler object. The second_pass_done
    // variable must be 'true'.
    std::string Translate();
  private:
    std::string translate_a_instruction(AInstruction a_instruction);
    std::string translate_c_instruction(CInstruction c_instruction);
    std::string translate_c_cmp(std::string cmp);
    std::string translate_c_jmp(std::string jmp);
    std::string translate_c_dst(std::string dst);

    Assembler* assembler_;
};

}
