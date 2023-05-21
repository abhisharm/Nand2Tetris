#include <string>
#include <iostream>
#include <fstream>
#include <regex>

#include "vmtranslator.h"
#include "parser.h"

namespace Translator {
  using std::cerr;
  using std::ifstream;
  using std::ofstream;
  using std::string;
  using std::cout;

  VMTranslator::VMTranslator(string filename) :
    input_filename_{ filename }
  {}

  int64_t VMTranslator::Run() {
    // Validating the input filename and using it to get the output.
    std::regex fname (R"(((?:[^/]*/)*)([^.]*)(.vm)?)");
    std::smatch sm;
    if (!std::regex_match(input_filename_, sm, fname)) {
      cerr << "Invalid argument to vmtranslator: " << input_filename_
        << "\nFilename must either be a file of the format File.vm or"
        << " a directory";
      return 1;
    }

    // Handling the case where a '.vm' file is passed
    string vm_ex (sm[3]);
    if (vm_ex.size() != 0) {
      string prefix (sm[2]);
      if (!isupper(prefix[0])) {
        cerr << "Invalid filename: " << input_filename_
          << "\nFilename must be of the format File.vm\n";
        return 1;
      }

      string path;
      if (sm[1] != "") {
        path = sm[1];
      }
      output_filename_ = path + prefix + ".asm"; 
      std::ofstream o_stream { output_filename_ };
      CodeWriter code_writer { &o_stream };
      int64_t result =
        handle_translation_for_file(input_filename_, prefix, code_writer);
      if (result != 0) {
        code_writer.close();
        return result;
      }
      code_writer.add_infinite_loop();
      code_writer.close();
      return 0;
    }

    return 0;
  }

  int64_t VMTranslator::handle_translation_for_file(
    string infile,
    string prefix,
    CodeWriter& code_writer) {
      std::ifstream i_stream { infile };
      Parser parser { &i_stream };
      code_writer.set_file_name(prefix);
      while (parser.has_more_lines()) {
        parser.advance();
        CommandType command_type = parser.command_type();
        switch (command_type) {
          case CommandType::C_ARITHMETIC:
            {
              string command = parser.arg1();
              code_writer.write_arithmetic(command);
            }
            break;
          case CommandType::C_PUSH:
          case CommandType::C_POP:
            {
              string segment = parser.arg1();
              int index = parser.arg2();
              code_writer.write_push_pop(command_type, segment, index);
            }
            break;
          default:
            cerr << "Unsupported command_type encountered: " << command_type
              << ". Exiting.";
            return 1;
        }
      }
      code_writer.add_infinite_loop();
      return 0;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "This program requires 1 argument\n"
         << "Usage: VMTranslator <infile>\n";
    return 1;
  }

  std::string input_file(argv[1]);
  Translator::VMTranslator vm_translator (input_file);
  return vm_translator.Run();
}
