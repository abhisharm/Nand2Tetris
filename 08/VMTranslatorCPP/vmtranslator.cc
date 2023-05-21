#include <string>
#include <iostream>
#include <filesystem>
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
    std::regex fname (R"(((?:[^/]*/)*?)([^./]*[/]?)(.vm)?)");
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
    else {
      // this means a directory was passed
      string directory_name = sm[0];
      string terminal_directory = sm[2];
      if (terminal_directory[terminal_directory.size() - 1] == '/') {
        directory_name.pop_back();
        terminal_directory.pop_back();
      }
      output_filename_ = directory_name + "/" + terminal_directory + ".asm";
      std::ofstream o_stream { output_filename_ };
      CodeWriter code_writer { &o_stream };
      code_writer.write_bootstrap();
      for (auto const& dir_entry
        : std::filesystem::directory_iterator{directory_name}) {
        string path (dir_entry.path());
        std::regex vm_fname (R"(((?:[^/]*/)*)([^.]*)[.]vm)");
        std::smatch vm_sm;
        if (std::regex_match(path, vm_sm, vm_fname)) {
          string prefix (vm_sm[2]);
          int64_t result =
            handle_translation_for_file(path, prefix, code_writer);
          if (result != 0) {
            code_writer.close();
            return result;
          }
        }
      }
      return 0;
    }
    return -1;
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
        case CommandType::C_LABEL:
          {
            string label = parser.arg1();
            code_writer.write_label(label); 
          }
          break;
        case CommandType::C_GOTO:
          {
            string label = parser.arg1();
            code_writer.write_goto(label);
          }
          break;
        case CommandType::C_IF:
          {
            string label = parser.arg1();
            code_writer.write_if(label);
          }
          break;
        case CommandType::C_FUNCTION:
          {
            string function_name = parser.arg1();
            int nvars = parser.arg2();
            code_writer.write_function(function_name, nvars);
          }
          break;
        case CommandType::C_RETURN:
          {
            code_writer.write_return();
          }
          break;
        case CommandType::C_CALL:
          {
            string function_name = parser.arg1();
            int nargs = parser.arg2();
            code_writer.write_call(function_name, nargs);
          } 
          break;
        default:
          cerr << "Unsupported command_type encountered: " << command_type
            << ". Exiting.";
          return 1;
      }
    }

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
