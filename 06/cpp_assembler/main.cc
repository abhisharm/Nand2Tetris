#include <fstream>
#include <istream>
#include <vector>
#include <variant>
#include <iostream>

#include "instruction.h"
#include "file_processor.h"
#include "parser.h"
#include "assembler.h"
#include "translator.h"


using assembler::AInstruction;
using assembler::CInstruction;
using assembler::Label;
using assembler::Assembler;
using assembler::Translator;
using assembler::Parser;
using assembler::FileProcessor;
using std::cout;
using std::ifstream;
using std::ofstream;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "This program requires 2 arguments"
         << " usage: assembler <infile> <outfile>\n";
    return 1;
  }

  std::string input_file(argv[1]);
  std::string output_file(argv[2]);

  // remove whitespace from the lines and create an array of std::string's 
  std::vector<std::string> cleaned_lines;
  ifstream ifs {input_file};
  bool file_read_result = FileProcessor::clean_input(ifs, &cleaned_lines);
  if (!file_read_result) {
    cout << "invalid file: " << input_file;
    return 1;
  }

  // convert each cleaned-line to an instruction or label 
  std::vector<std::variant<AInstruction, CInstruction, Label>> commands;
  bool parse_result = Parser::ParseInstructions(cleaned_lines, &commands);
  if (!parse_result) {
    cout << "error with parsing.";
    return 1;
  }

  // assemble the parsed commands
  Assembler as { commands };
  bool assembly_result = as.assemble();
  if (!assembly_result) {
    cout << "error with assembling";
    return 1;
  }

  Translator translator { &as };
  std::string assembled_string = translator.Translate();
  ofstream ofs {output_file};
  ofs << assembled_string;
  return 0; 
}
