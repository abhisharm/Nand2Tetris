#include "parser.h"

#include <iostream>
#include <sstream>
#include <regex>
#include <stdexcept>

namespace Translator {
  using std::istream;
  using std::string;
  using std::cout;

  Parser::Parser(istream* stream) :
    i_stream_(stream) {}

  bool Parser::has_more_lines() {
    while (true) {
      char c = i_stream_->peek();
      if (c == EOF) {
        return false;
      } else if (c == ' ' || c == '\n' || c == '\t' || (int)c == 13 ) {
        char tmp;
        i_stream_->get(tmp);
      } else if (c == '/') {
        string tmp;
        std::getline(*i_stream_, tmp);
      } else {
        // Assuming well-formatted input, we have reached the start of a
        // command.
        return true;
      }
    }
    // should not reach this - suggests badly-formatted input.
    return false;
  }

  void Parser::advance() {
    string line;
    std::getline(*i_stream_, line);
    std::regex pat (R"((\w+)\s*(\w*)\s*(\d*)\s*(\/{2}.*)?)");
    // get the first match and branch on each command type
    std::smatch sm;
    std::regex_match(line, sm, pat);
    string command = sm[1];
    if (command == "add" | command == "sub" | command == "neg" |
        command == "eq" | command == "gt" | command == "lt" |
        command == "and" | command == "or" | command == "not") {
        command_type_ = CommandType::C_ARITHMETIC;
        arg1_ = command; 
    } else if (command == "push") {
        command_type_ = CommandType::C_PUSH;
        string segment (sm[2]);
        arg1_ = segment;
        string index (sm[3]);
        arg2_ = atoi(index.c_str());
    }
    else if (command == "pop") {
        command_type_ = CommandType::C_POP;
        string segment (sm[2]);
        arg1_ = segment;
        string index (sm[3]);
        arg2_ = atoi(index.c_str());
    } else {
        string exception = "Unable to parse command " + command + "\n";
        throw std::invalid_argument(exception); 
    }
  }

  CommandType Parser::command_type() {
    return command_type_;
  }

  string Parser::arg1() {
    return arg1_;
  }

  int Parser::arg2() {
    return arg2_;
  }
}
