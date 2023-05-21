// Jack Analyzer class implementation

#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>

#include "JackAnalyzer.h"
#include "JackTokenizer.h"
#include "CompilationEngine.h"

namespace JackCompiler {

using std::cerr;
using std::cout;
using std::string;
using JackCompiler::JackTokenizer;
using JackCompiler::CompilationEngine;

JackAnalyzer::JackAnalyzer(vector<string> args) : args_(args) {}

JackAnalyzer::ArgumentDescriptor JackAnalyzer::GetArgumentType(string arg) {
  // Validating the input filename and using it to get the output.
  std::regex fname_regex (R"(((?:[^/]*/)*?)([^./]*[/]?)(.jack)?)");
  std::smatch sm;
  if (!std::regex_match(arg, sm, fname_regex)) {
    ArgumentDescriptor argument_descriptor = {
      .argument_type = JackAnalyzer::ArgumentType::UNMATCHED,
    };
    return argument_descriptor;
  }

  // Handling the case where a '.Jack' file is passed
  string suffix (sm[3]);
  if (suffix.size() != 0) {
    string prefix (sm[2]);
    if (!isupper(prefix[0])) {
      cerr << "Invalid filename: " << arg
        << "\nFilename must be of the format File.jack\n";
    }

    string path;
    if (sm[1] != "") {
      path = sm[1];
      if (path[path.length() - 1] != '/') {
        path.append("/");
      }
    }

    JackAnalyzer::ArgumentDescriptor argument_descriptor = {
      .argument_type = JackAnalyzer::ArgumentType::FILENAME,
      .path = path,
      .name = prefix,
    };
    return argument_descriptor;
  } // else a directory was passed
  else {
    if (arg[arg.length() - 1] != '/') {
      arg.append("/");
    }

    JackAnalyzer::ArgumentDescriptor argument_descriptor = {
      .argument_type = ArgumentType::DIRNAME,
      .path = arg,
    };
    return argument_descriptor;
  }
}

void WriteTokenizerXml(JackTokenizer& tokenizer, std::ofstream& o_stream) {
  JackTokenizer::TokenType token_type = tokenizer.tokenType();
  string output = "";
  switch (token_type) {
		case(JackTokenizer::TokenType::KEYWORD):
      {
        output += "<keyword> ";
        JackTokenizer::KeyWord keyword = tokenizer.keyWord();
        output += JackTokenizer::keyWordToString(keyword);
        output += " </keyword>";
        break;
      }
		case(JackTokenizer::TokenType::SYMBOL):
    {
      output += "<symbol> ";
      char symbol = tokenizer.Symbol();
      if (symbol == '<') {
        output += "&lt;";
      } else if (symbol == '>') {
        output += "&gt;";
      } else if (symbol == '"') {
        output += "&quot;";
      } else if (symbol == '&') {
        output += "&amp;";
      } else {
        output += symbol; 
      }
      output += " </symbol>";
      break;
    }
		case(JackTokenizer::TokenType::IDENTIFIER):
      output += "<identifier> ";
      output += tokenizer.Identifier();
      output += " </identifier>";
      break;
		case(JackTokenizer::TokenType::INT_CONST):
    {
      output += "<integerConstant> ";
      output += std::to_string(tokenizer.intVal());
      output += " </integerConstant>";
      break;
    }
		case(JackTokenizer::TokenType::STRING_CONST):
      output += "<stringConstant>";
      output += tokenizer.stringVal();
      output += " </stringConstant>";
      break;
  }

  output += "\n";
  o_stream.write(output.c_str(), output.length());
}

void JackAnalyzer::ConstructSimplifiedOutputForFile(
  JackAnalyzer::ArgumentDescriptor argument_descriptor) {
  // create proper input and output streams
  string input = argument_descriptor.path + argument_descriptor.name + ".jack";
  cout << "reading from file: " << input << "\n";
  std::ifstream i_stream { input };
  string output = argument_descriptor.path + argument_descriptor.name + ".xml";
  cout << "writing to file: " << output << "\n";
  // create the tokenizer
  std::ofstream o_stream { output };
  JackTokenizer tokenizer(&i_stream);
  // specific to the first milestone:
  o_stream << "<tokens>\n";
  while (tokenizer.hasMoreTokens()) {
    tokenizer.advance();
    WriteTokenizerXml(tokenizer, o_stream);
  }
  o_stream << "</tokens>\n";
}

void JackAnalyzer::ConstructOutputForFile(
  JackAnalyzer::ArgumentDescriptor argument_descriptor) {
  // create proper input and output streams
  string input = argument_descriptor.path + argument_descriptor.name + ".jack";
  cout << "reading from file: " << input << "\n";
  std::ifstream i_stream { input };
  string output = argument_descriptor.path + argument_descriptor.name + ".xml";
  cout << "writing to file: " << output << "\n";
  // create the tokenizer
  std::ofstream o_stream { output };
  JackTokenizer tokenizer(&i_stream);
  if (!tokenizer.hasMoreTokens()) {
    return;
  }
  tokenizer.advance();

  // create the compilation engine
  CompilationEngine compilation_engine(&tokenizer, &o_stream);
  // the first token should be a class. Start processing from there
  if (tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD ||
    tokenizer.keyWord() != JackTokenizer::KeyWord::CLASS) {
    cerr << "ERROR: file " << input << " is malformed: does not start with a "
      << "class definition.\n";
  }

  compilation_engine.compileClass();
  // while (tokenizer.hasMoreTokens()) { 
  //   if (tokenizer.tokenType() != JackTokenizer::TokenType::KEYWORD ||
  //   tokenizer.keyWord() != JackTokenizer::KeyWord::CLASS) {
  //   cerr << "ERROR: file " << input << " is malformed. Class definition "
  //     << "followed by non-class definition.\n";
  //   }
  //   compilation_engine.compileClass();
  // }
  return;
}

void JackAnalyzer::ConstructOutputForDirectory(
  ArgumentDescriptor argument_descriptor) {
  std::string directory_name = argument_descriptor.path;
  for (auto const& dir_entry
    : std::filesystem::directory_iterator{directory_name}) {
    std::string path = dir_entry.path();
    ArgumentDescriptor argument_descriptor = GetArgumentType(path);
    // check if the path is a jack file:
    if (argument_descriptor.argument_type == JackAnalyzer::ArgumentType::FILENAME) {
      ConstructOutputForFile(argument_descriptor);
    }
  }
}

void JackAnalyzer::ConstructOutputForAllArgs() {
  for (auto arg : args_) {
    ArgumentDescriptor argument_descriptor = GetArgumentType(arg);
    switch (argument_descriptor.argument_type) {
      case JackAnalyzer::ArgumentType::FILENAME:
        ConstructOutputForFile(argument_descriptor);
        break;
      case JackAnalyzer::ArgumentType::DIRNAME:
        ConstructOutputForDirectory(argument_descriptor);
        break;
      case JackAnalyzer::ArgumentType::UNMATCHED:
        break;
    }
  }
  return;
}
}

int main(int argc, char** argv) {
  std::vector<std::string> args;
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    args.push_back(arg);
  }

  JackCompiler::JackAnalyzer analyzer(args);
  analyzer.ConstructOutputForAllArgs();
  return 0;
}
