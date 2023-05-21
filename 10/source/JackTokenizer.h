// Jack Tokenizer class
#ifndef JACK_TOKENIZER_H
#define JACK_TOKENIZER_H

#include <string>
#include <iostream>

namespace JackCompiler {

class JackTokenizer {
  public:
    enum TokenType {
      KEYWORD = 0,
      SYMBOL = 1,
      IDENTIFIER = 2,
      INT_CONST = 3,
      STRING_CONST = 4,
    };

    enum KeyWord {
      CLASS = 1,
      METHOD = 2,
      FUNCTION = 3,
      CONSTRUCTOR = 4,
      INT = 5,
      BOOLEAN = 6,
      CHAR = 7,
      VOID = 8,
      VAR = 9,
      STATIC = 10,
      FIELD = 11,
      LET = 12,
      DO = 13,
      IF = 14,
      ELSE = 15,
      WHILE = 16,
      RETURN = 17,
      TRUE = 18,
      FALSE = 19,
      VAL_NULL = 20,
      THIS = 21,
    };

    static std::string keyWordToString(KeyWord keyword);

    static std::string tokenTypeToString(TokenType token_type);
 
    JackTokenizer(std::istream* i_stream);

    // Returns whether or not there are more tokens in the input stream.
    bool hasMoreTokens();

    // Gets the next token from input, and makes it the current token. Only
    // call if 'hasMoreTokens' is true.
    // Initally there is no current token.
    void advance();

    // Returns the type of the current token
    TokenType tokenType();

    // Returns the current keyword (only if the current token type is a 'KEYWORD')
    KeyWord keyWord();

    // Returns the character representing the current symbol (only if the
    // current token is a 'SYMBOL').
    char Symbol();

    // Returns the string value of the current identifier (only if the
    // current token is an 'IDENTIFIER').
    std::string Identifier();

    // Returns the integer value of the current token (only if the current
    // token is 'INT_CONST').
    int intVal();

    // Returns the string value of the current string constant token (only if
    // the current token is 'STRING_CONST')
    std::string stringVal();

  private:
    std::istream* i_stream_;
    TokenType token_type_;
    KeyWord keyword_;
    char symbol_;
    std::string identifier_;
    int intval_;
    std::string stringval_;

    void tokenizeKeywordOrIdentifier();

    void tokenizeIntVal();

    void tokenizeStringVal();
};

} // namespace JackAnalyzer
#endif
