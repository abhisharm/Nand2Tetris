// Jack Tokenizer implementation
#include "JackTokenizer.h"

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

namespace AbhishekJackCompiler {

using std::cerr;
using std::cout;

JackTokenizer::JackTokenizer(std::istream* i_stream) : i_stream_(i_stream) {}

std::string JackTokenizer::keyWordToString(JackTokenizer::KeyWord keyword) {
  switch (keyword) {
    case (JackTokenizer::KeyWord::CLASS):
    	return "class";
    	break;
    case (JackTokenizer::KeyWord::METHOD):
    	return "method";
    	break;
    case (JackTokenizer::KeyWord::FUNCTION):
    	return "function";
    	break;
    case (JackTokenizer::KeyWord::CONSTRUCTOR):
    	return "constructor";
    	break;
    case (JackTokenizer::KeyWord::INT):
    	return "int";
    	break;
    case (JackTokenizer::KeyWord::BOOLEAN):
    	return "boolean";
    	break;
    case (JackTokenizer::KeyWord::CHAR):
    	return "char";
    	break;
    case (JackTokenizer::KeyWord::VOID):
    	return "void";
    	break;
    case (JackTokenizer::KeyWord::VAR):
    	return "var";
    	break;
    case (JackTokenizer::KeyWord::STATIC):
    	return "static";
    	break;
    case (JackTokenizer::KeyWord::FIELD):
    	return "field";
    	break;
    case (JackTokenizer::KeyWord::LET):
    	return "let";
    	break;
    case (JackTokenizer::KeyWord::DO):
    	return "do";
    	break;
    case (JackTokenizer::KeyWord::IF):
    	return "if";
    	break;
    case (JackTokenizer::KeyWord::ELSE):
    	return "else";
    	break;
    case (JackTokenizer::KeyWord::WHILE):
    	return "while";
    	break;
    case (JackTokenizer::KeyWord::RETURN):
    	return "return";
    	break;
    case (JackTokenizer::KeyWord::TRUE):
    	return "true";
    	break;
    case (JackTokenizer::KeyWord::FALSE):
    	return "false";
    	break;
    case (JackTokenizer::KeyWord::VAL_NULL):
    	return "null";
    	break;
    case (JackTokenizer::KeyWord::THIS):
    	return "this";
    	break;
    default:
      cerr << "Error converting keyword to string\n";
      return "";
      break;
  }
}

std::string JackTokenizer::tokenTypeToString(
  JackTokenizer::TokenType token_type) {
  switch (token_type) {
    case (JackTokenizer::TokenType::KEYWORD):
      return "keyword";
    case (JackTokenizer::TokenType::SYMBOL):
      return "symbol";
    case (JackTokenizer::TokenType::IDENTIFIER):
      return "identifier";
    case (JackTokenizer::TokenType::INT_CONST):
      return "int_const";
    case (JackTokenizer::TokenType::STRING_CONST):
      return "string_const";
    default:
      cerr << "ERROR converting tokentype to string\n";
      return "";
  }
}

bool JackTokenizer::hasMoreTokens() {
  bool scanning_multiline_comment = false;
  while (true) {
    char c = i_stream_->peek();
    if (scanning_multiline_comment) {
      i_stream_->get(c);
      if (c == '*') {
        // if next character is '*', check if it signals end of multi-line
        // comment.
        char next;
        next = i_stream_->peek();
        if (next == '/') {
          // if next character after '*' is '/', we've reached the end and can
          // continue on after setting the end of the multi-line comment
          scanning_multiline_comment = false;
          i_stream_->get(next);
        } else {
          // if the next character after '*' is not '/', we haven't reached the
          // end and must continue from this character
        }
      }
      // if the next character is not '*', ignore it and keep scanning
    } else if (c == EOF) {
      return false;
    } else if (c == ' ' || c == '\n' || c == '\t' || (int)c == 13 ) {
      i_stream_->seekg(1, i_stream_->cur);
    } else {
      // if not whitespace or newline, need to check if its a comment
      if (c == '/') {
        // scan ahead 2 to determine if its a comment or a '/'
        char next1;
        char next2;
        i_stream_->get(next1);
        i_stream_->get(next2);
        if (next2 == '/') {
          // if the next character is a forward-slash, its a one-line comment,
          // so move back and consume the line
          i_stream_->seekg(-2, i_stream_->cur);
          std::string tmp;
          std::getline(*i_stream_, tmp);
        } else if (next2 == '*') {
          // If the next character is a '*', its a multi-line comment.
          scanning_multiline_comment = true;
          // move forward and get the next character
          char next3;
          i_stream_->get(next3);
          if (next3 != '*') {
            // if third char is not '*' we need to move back one character
            i_stream_->seekg(-1, i_stream_->cur);
          }
          // otherwise, we start scanning after the second '*'
        } else {
          // if the next character is not a forward-slash, return true (the
          // token is '/')
          i_stream_->seekg(-2, i_stream_->cur);
          cout << "Value of i_stream after seeing a '/': "
            << i_stream_->peek() << "\n";
          return true;
        }
      } else {
        // if none of the above conditions are true, the parser has reached the
        // start of a token
        return true;
      }
    }
  }
  // should not reach this - suggests badly-formatted input.
  return false;
}

void JackTokenizer::tokenizeKeywordOrIdentifier() {
  std::string token;
  char c = i_stream_->peek();
  while (isalpha(c) | isdigit(c) | c == '_') {
    i_stream_->get(c);
    token += c;
    c = i_stream_->peek();
  }

	if (token == ("class")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::CLASS;
    return;
	} else if (token == ("constructor")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::CONSTRUCTOR;
    return;
	} else if (token == ("function")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::FUNCTION;
    return;
	} else if (token == ("method")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::METHOD;
    return;
	} else if (token == ("field")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::FIELD;
    return;
	} else if (token == ("static")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::STATIC;
    return;
	} else if (token == ("var")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::VAR;
    return;
	} else if (token == ("int")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::INT;
    return;
	} else if (token == ("char")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::CHAR;
    return;
	} else if (token == ("boolean")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::BOOLEAN;
    return;
	} else if (token == ("void")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::VOID;
    return;
	} else if (token == ("true")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::TRUE;
    return;
	} else if (token == ("false")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::FALSE;
    return;
	} else if (token == ("null")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::VAL_NULL;
    return;
	} else if (token == ("this")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::THIS;
    return;
	} else if (token == ("let")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::LET;
    return;
	} else if (token == ("do")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::DO;
    return;
	} else if (token == ("if")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::IF;
    return;
	} else if (token == ("else")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::ELSE;
    return;
	} else if (token == ("while")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::WHILE;
    return;
	} else if (token == ("return")) {
    token_type_ = JackTokenizer::TokenType::KEYWORD;
		keyword_ = JackTokenizer::KeyWord::RETURN;
    return;
  }

  // else the string is an identifier
  token_type_ = JackTokenizer::TokenType::IDENTIFIER;
  identifier_ = token;
  return;
}

void JackTokenizer::tokenizeIntVal() {
  std::string token;
  char c = i_stream_->peek();
  while (isdigit(c)) {
    i_stream_->get(c);
    token += c;
    c = i_stream_->peek();
  }

  int intVal = atoi(token.c_str());
  intval_ = intVal;
  token_type_ = JackTokenizer::TokenType::INT_CONST;
  return;
}

void JackTokenizer::tokenizeStringVal() {
  std::string token;
  char c;
  // c should = '"'
  i_stream_->get(c);
  c = i_stream_->peek();
  while (c != '"') {
    i_stream_->get(c);
    token += c;
    c = i_stream_->peek();
  }
  // advance past the last '"'
  i_stream_->get(c);

  token_type_ = JackTokenizer::TokenType::STRING_CONST;
  stringval_ = token;
  return;
}

void JackTokenizer::advance() {
  if (!hasMoreTokens()) {
    return;
  }

  char c = i_stream_->peek();
  std::vector<char> symbols { '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~' };
  if (isalpha(c) | c == '_') {
    tokenizeKeywordOrIdentifier();
  } else if (std::count(symbols.begin(), symbols.end(), c)) {
    token_type_ = JackTokenizer::TokenType::SYMBOL;
    symbol_ = c;
    i_stream_->seekg(1, i_stream_->cur);
  } else if (isdigit(c)) {
    tokenizeIntVal();
  } else if (c == '"') {
    tokenizeStringVal();
  } else {
    cerr << "unrecognized character: " << c << " at start of a token.\n";
  }
  return;
}

JackTokenizer::TokenType JackTokenizer::tokenType() { return token_type_; }

JackTokenizer::KeyWord JackTokenizer::keyWord() { return keyword_; }

char JackTokenizer::Symbol() { return symbol_; }

std::string JackTokenizer::Identifier() { return identifier_; }

int JackTokenizer::intVal() { return intval_; }

std::string JackTokenizer::stringVal() { return stringval_; }
} // namespace JackAnalyzer
