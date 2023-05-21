#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>

namespace Translator {
  using std::istream;
  using std::string;

  enum CommandType {
      C_ARITHMETIC = 0,
      C_PUSH = 1,
      C_POP = 2,
      C_LABEL = 3,
      C_GOTO = 4,
      C_IF = 5,
      C_FUNCTION = 6,
      C_RETURN = 7,
      C_CALL = 8,
   };

  class Parser {
    public:
    Parser(istream* stream); 

    bool has_more_lines();

    void advance();

    CommandType command_type();

    string arg1();

    int arg2();

    private:
    istream* i_stream_;
    CommandType command_type_;
    string arg1_;
    int arg2_;
  };

}
#endif
