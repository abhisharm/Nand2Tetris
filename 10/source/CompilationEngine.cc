#include "CompilationEngine.h"
#include "JackTokenizer.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

namespace JackCompiler {

using JackCompiler::JackTokenizer;
using std::cerr;

void CompilationEngine::compileClass() {
  *o_stream_ << "<class>\n";
  processKeyWord(JackTokenizer::KeyWord::CLASS);
  // processing className
  processIdentifier();
  processSymbol('{');
  bool processing_class_var_decs = true;
  while (processing_class_var_decs) {
    if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD &&
      (tokenizer_->keyWord() == JackTokenizer::KeyWord::STATIC ||
      tokenizer_->keyWord() == JackTokenizer::KeyWord::FIELD)) {
      compileClassVarDec();
    } else {
      processing_class_var_decs = false;
    }
  }

  bool processing_subroutine_decs = true;
  while (processing_subroutine_decs) {
    if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD &&
      (tokenizer_->keyWord() == JackTokenizer::KeyWord::CONSTRUCTOR ||
      tokenizer_->keyWord() == JackTokenizer::KeyWord::FUNCTION ||
      tokenizer_->keyWord() == JackTokenizer::KeyWord::METHOD)) {
      compileSubroutine(); 
    } else {
      processing_subroutine_decs = false;
    }
  }

  processSymbol('}');
  *o_stream_ << "</class>\n";
}

void CompilationEngine::compileClassVarDec() {
  *o_stream_ << "<classVarDec>\n";
  if (tokenizer_->keyWord() == JackTokenizer::KeyWord::STATIC) {
    processKeyWord(JackTokenizer::KeyWord::STATIC);
  }
  else {
    processKeyWord(JackTokenizer::KeyWord::FIELD);
  }

  // processing type
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
    if (tokenizer_->keyWord() == JackTokenizer::KeyWord::INT) {
      processKeyWord(JackTokenizer::KeyWord::INT);
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::CHAR) {
      processKeyWord(JackTokenizer::KeyWord::CHAR);
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::BOOLEAN) {
      processKeyWord(JackTokenizer::KeyWord::BOOLEAN);
    } else {
      cerr << "Got wrong keyword-type in 'compileClassVarDec'. got: "
        << JackTokenizer::keyWordToString(tokenizer_->keyWord())
        << ". Expected one of int, char, or boolean\n";
    }
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
    processIdentifier();
  } else {
    cerr << "Got wrong tokentype in in 'compileClassVarDec'. got: "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType())
      << ". Expected keyword or identifier\n";
  }

  // processing varName
  processIdentifier();

  // processing additional local variables, if any
  while (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    tokenizer_->Symbol() == ',') {
    // process semicolon
    processSymbol(',');
    // process varName
    processIdentifier();
  }
  // process closing semicolon
  processSymbol(';');
  *o_stream_ << "</classVarDec>\n";
}

void CompilationEngine::compileSubroutine() {
  *o_stream_ << "<subroutineDec>\n";
  // process 
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
    if (tokenizer_->keyWord() == JackTokenizer::KeyWord::CONSTRUCTOR) {
      processKeyWord(JackTokenizer::KeyWord::CONSTRUCTOR);
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::FUNCTION) {
      processKeyWord(JackTokenizer::KeyWord::FUNCTION);
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::METHOD) {
      processKeyWord(JackTokenizer::KeyWord::METHOD);
    } else {
      cerr << "Got wrong keyword-type in 'compileSubroutine'. got: "
        << JackTokenizer::keyWordToString(tokenizer_->keyWord())
        << ". Expected one of constructor, function, or method.\n";
    }
  }  else {
    cerr << "Got wrong tokentype in in 'compileClassVarDec'. got: "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType())
      << ". Expected keyword\n";
  }

  if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD &&
    tokenizer_->keyWord() == JackTokenizer::KeyWord::VOID) {
    processKeyWord(JackTokenizer::KeyWord::VOID);
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
    // processing type in this branch
    if (tokenizer_->keyWord() == JackTokenizer::KeyWord::INT) {
      processKeyWord(JackTokenizer::KeyWord::INT);
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::CHAR) {
      processKeyWord(JackTokenizer::KeyWord::CHAR);
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::BOOLEAN) {
      processKeyWord(JackTokenizer::KeyWord::BOOLEAN);
    } else {
      cerr << "Got wrong keyword-type in 'compileClassVarDec'. got: "
        << JackTokenizer::keyWordToString(tokenizer_->keyWord())
        << ". Expected one of int, char, or boolean\n";
    }
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
    // processing type with class name
    processIdentifier();
  } else {
    cerr << "Got wrong tokentype in in 'compileSubroutine'. got: "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType())
      << ". Expected keyword or identifier\n";
  }

  // process subroutineName
  processIdentifier();
  processSymbol('(');
  compileParameterList();
  processSymbol(')');
  compileSubroutineBody();
  *o_stream_ << "</subroutineDec>\n";
}

void CompilationEngine::compileParameterList() {
  *o_stream_ << "<parameterList>\n";
  // processing type (if present)
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
    if (tokenizer_->keyWord() == JackTokenizer::KeyWord::INT) {
      processKeyWord(JackTokenizer::KeyWord::INT);
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::CHAR) {
      processKeyWord(JackTokenizer::KeyWord::CHAR);
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::BOOLEAN) {
      processKeyWord(JackTokenizer::KeyWord::BOOLEAN);
    } else {
      cerr << "Got wrong keyword-type in 'compileParameterList'. got: "
        << JackTokenizer::keyWordToString(tokenizer_->keyWord())
        << ". Expected one of int, char, or boolean\n";
    }
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
    // processing type with class name
    processIdentifier();
  } else {
    // else no parameter list was passed
    *o_stream_ << "</parameterList>\n";
    return;
  }

  // processing varName
  processIdentifier();

  // processing additional parameters (if present).
  while (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    tokenizer_->Symbol() == ',') {
    processSymbol(',');
    // processing type (now, must be present)
    if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
      if (tokenizer_->keyWord() == JackTokenizer::KeyWord::INT) {
        processKeyWord(JackTokenizer::KeyWord::INT);
      } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::CHAR) {
        processKeyWord(JackTokenizer::KeyWord::CHAR);
      } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::BOOLEAN) {
        processKeyWord(JackTokenizer::KeyWord::BOOLEAN);
      } else {
        cerr << "Got wrong keyword-type in 'compileParameterList'. got: "
          << JackTokenizer::keyWordToString(tokenizer_->keyWord())
          << ". Expected one of int, char, or boolean\n";
      }
    } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
      // processing type with class name
      processIdentifier();
    } else {
      cerr << "Got wrong token type in 'compileParameterList'. got: "
        << JackTokenizer::tokenTypeToString(tokenizer_->tokenType())
        << ". Expected keyword or identifier\n";
    }

    // processing varName
    processIdentifier();
  }
  *o_stream_ << "</parameterList>\n";  
}

void CompilationEngine::compileSubroutineBody() {
  *o_stream_ << "<subroutineBody>\n";
  processSymbol('{');
  while (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD &&
    tokenizer_->keyWord() == JackTokenizer::KeyWord::VAR) {
    compileVarDec();
  }
  compileStatements();
  processSymbol('}');
  *o_stream_ << "</subroutineBody>\n";
}

void CompilationEngine::compileVarDec() {
  *o_stream_ << "<varDec>\n";
  processKeyWord(JackTokenizer::KeyWord::VAR);

  // processing type
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
    if (tokenizer_->keyWord() == JackTokenizer::KeyWord::INT) {
      processKeyWord(JackTokenizer::KeyWord::INT);
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::CHAR) {
      processKeyWord(JackTokenizer::KeyWord::CHAR);
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::BOOLEAN) {
      processKeyWord(JackTokenizer::KeyWord::BOOLEAN);
    } else {
      cerr << "Got wrong keyword-type in 'compileVarDec'. got: "
        << JackTokenizer::keyWordToString(tokenizer_->keyWord())
        << ". Expected one of int, char, or boolean\n";
    }
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
    processIdentifier();
  } else {
    cerr << "Got wrong tokentype in in 'compileVarDec'. got: "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType())
      << ". Expected keyword or identifier\n";
  }

  // processing varName
  processIdentifier();

  // processing additional variable-declarations (if present)
  while (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    tokenizer_->Symbol() == ',') {
    processSymbol(',');
    processIdentifier();
  }

  processSymbol(';');
  *o_stream_ << "</varDec>\n";
}

void CompilationEngine::compileStatements() {
  *o_stream_ << "<statements>\n";
  // compiling statement*
  // the only thing that could follow a statement is '}', so this works
  while (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
    if (tokenizer_->keyWord() == JackTokenizer::KeyWord::LET) {
      compileLet();
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::IF) {
      compileIf();
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::WHILE) {
      compileWhile();
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::DO) {
      compileDo();
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::RETURN) {
      compileReturn();
    } else {
      cerr << "Error while compiling statements. Got keyword: "
        << JackTokenizer::keyWordToString(tokenizer_->keyWord())
        << ", expected one of let, if, while, do, return.\n";
    }
  }
  *o_stream_ << "</statements>\n";
}

void CompilationEngine::compileLet() {
  *o_stream_ << "<letStatement>\n";
  processKeyWord(JackTokenizer::KeyWord::LET);
  // process varName
  processIdentifier();
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
     tokenizer_->Symbol() == '[') {
    processSymbol('[');
    compileExpression();
    processSymbol(']');
  }
  processSymbol('=');
  compileExpression();
  processSymbol(';');
  *o_stream_ << "</letStatement>\n";
}

void CompilationEngine::compileIf() {
  *o_stream_ << "<ifStatement>\n";
  processKeyWord(JackTokenizer::KeyWord::IF);
  processSymbol('(');
  compileExpression();
  processSymbol(')');
  processSymbol('{');
  compileStatements();
  processSymbol('}');
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD &&
    tokenizer_->keyWord() == JackTokenizer::KeyWord::ELSE) {
    processKeyWord(JackTokenizer::KeyWord::ELSE); 
    processSymbol('{');
    compileStatements();
    processSymbol('}');
  }
  *o_stream_ << "</ifStatement>\n";
}

void CompilationEngine::compileWhile() {
  *o_stream_ << "<whileStatement>\n";
  processKeyWord(JackTokenizer::KeyWord::WHILE);
  processSymbol('(');
  compileExpression();
  processSymbol(')');
  processSymbol('{');
  compileStatements();
  processSymbol('}');
  *o_stream_ << "</whileStatement>\n";
}

void CompilationEngine::compileDo() {
  *o_stream_ << "<doStatement>\n";
  processKeyWord(JackTokenizer::KeyWord::DO);
  // process subroutineCall:
  // in practice its compiled as a term, without 'term' xml tags
  compileTermInternal();
  processSymbol(';');
  *o_stream_ << "</doStatement>\n";
}

void CompilationEngine::compileReturn() {
  *o_stream_ << "<returnStatement>\n";
  processKeyWord(JackTokenizer::KeyWord::RETURN);
  // processing the return expression, if necessary
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::SYMBOL ||
    (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    tokenizer_->Symbol() != ';')) {
    compileExpression();
  }
  processSymbol(';');
  *o_stream_ << "</returnStatement>\n";
}

void CompilationEngine::compileExpression() {
  *o_stream_ << "<expression>\n";
  compileTerm();
  std::vector<char> ops = {'+', '-', '*', '/', '&', '|', '<', '>', '='};
  while (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    (std::find(ops.begin(), ops.end(), tokenizer_->Symbol()) !=
          ops.end())) {
    processSymbol();
    compileTerm();
  }
  *o_stream_ << "</expression>\n";
}

bool CompilationEngine::isKeywordConstant() {
  std::vector<JackTokenizer::KeyWord> keyword_constants =
    { JackTokenizer::KeyWord::TRUE, JackTokenizer::KeyWord::FALSE,
      JackTokenizer::KeyWord::VAL_NULL,
      JackTokenizer::KeyWord::THIS };
  return (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD &&
    (std::find(keyword_constants.begin(), keyword_constants.end(),
      tokenizer_->keyWord()) != keyword_constants.end()));
}

bool CompilationEngine::isUnaryOp() {
  std::vector<char> unary_ops = {'-', '~'};
  return (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    (std::find(unary_ops.begin(), unary_ops.end(), tokenizer_->Symbol()) !=
      unary_ops.end()));
}

void CompilationEngine::compileTerm() {
  *o_stream_ << "<term>\n";
  compileTermInternal();
  *o_stream_ << "</term>\n";
}

void CompilationEngine::compileTermInternal() {
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::INT_CONST) {
    processIntegerConstant();
  } else if (
    tokenizer_->tokenType() == JackTokenizer::TokenType::STRING_CONST) {
    processStringConstant();
  } else if (isKeywordConstant()) {
    processKeyWord();
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
    // identifier case requires some lookahead
    std::string identifier = tokenizer_->Identifier();
    tokenizer_->advance();
    if (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
      tokenizer_->Symbol() == '[') {
      // process subscripting operation
      *o_stream_ << "<identifier>" << identifier << "</identifier>\n";
      processSymbol('[');
      compileExpression();
      processSymbol(']');
    } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
      (tokenizer_->Symbol() == '.' || tokenizer_->Symbol() == '(')) {
      // process subroutine call
      *o_stream_ << "<identifier>" << identifier << "</identifier>\n";
      if (tokenizer_->Symbol() == '.') {
        processSymbol('.');
        processIdentifier();
      }
      processSymbol('(');
      compileExpressionList();
      processSymbol(')');
    } else {
      // process simple simple varname
      *o_stream_ << "<identifier>" << identifier << "</identifier>\n";
    }
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
      tokenizer_->Symbol() == '(') {
    // process expression in parentheses
    processSymbol('(');
    compileExpression();
    processSymbol(')');
  } else if (isUnaryOp()) {
    // process unary op term
    processSymbol();
    compileTerm();
  } else {
    cerr << "CompilationEngine::compileTerm ERROR: Got unexpected tokens when "
      << "attempting to compile term.\n";
  }
}

int CompilationEngine::compileExpressionList() {
  *o_stream_ << "<expressionList>\n";
  int num_args = 0;
  if (!(tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    tokenizer_->Symbol() == ')')) {
    compileExpression();
    num_args += 1;
    while (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
      tokenizer_->Symbol() == ',') {
      processSymbol(',');
      compileExpression();
      num_args += 1;
    }
  }
  *o_stream_ << "</expressionList>\n";
  return num_args;
}

// terminal rule processors:
void CompilationEngine::processKeyWord(JackTokenizer::KeyWord key_word) {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::KEYWORD ||
    key_word != tokenizer_->keyWord()) {
    cerr << "Error processing keyword. Expected: " 
      << "(keyword, " << JackTokenizer::keyWordToString(key_word) << "). got "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType())
      << ", " << JackTokenizer::keyWordToString(tokenizer_->keyWord()) << "\n";
    return;
  }

  *o_stream_ << "<keyword>" << JackTokenizer::keyWordToString(key_word)
    << "</keyword>\n";
  tokenizer_->advance();
  return;
}

void CompilationEngine::processKeyWord() {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::KEYWORD) {
    cerr << "Error processing keyword. Expected: keyword. got " 
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << "\n";
    return;
  }

  *o_stream_ << "<keyword>"
    << JackTokenizer::keyWordToString(tokenizer_->keyWord())
    << "</keyword>\n";
  tokenizer_->advance();
  return;
}

void CompilationEngine::processSymbol(char symbol) {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::SYMBOL ||
    symbol != tokenizer_->Symbol()) {
    cerr << "Error processing symbol. Expected: " 
      << "(symbol, " << symbol << "). got "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << ", "
      << tokenizer_->Symbol() << "\n";
  }

  *o_stream_ << "<symbol>";
  if (symbol == '<') {
    *o_stream_ << "&lt;";
  } else if (symbol == '>') {
    *o_stream_ << "&gt;";
  } else if (symbol == '"') {
    *o_stream_ << "&quot;";
  } else if (symbol == '&') {
    *o_stream_ << "&amp;";
  } else {
    *o_stream_ << symbol; 
  }
  *o_stream_ << "</symbol>\n";
  tokenizer_->advance();
}

void CompilationEngine::processSymbol() {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::SYMBOL) {
    cerr << "Error processing symbol. Expected: symbol. Got "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << "\n";
  }

  *o_stream_ << "<symbol>";
  char symbol = tokenizer_->Symbol();
  if (symbol == '<') {
    *o_stream_ << "&lt;";
  } else if (symbol == '>') {
    *o_stream_ << "&gt;";
  } else if (symbol == '"') {
    *o_stream_ << "&quot;";
  } else if (symbol == '&') {
    *o_stream_ << "&amp;";
  } else {
    *o_stream_ << symbol; 
  }
  *o_stream_ << "</symbol>\n";
  tokenizer_->advance();
}

void CompilationEngine::processIntegerConstant() {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::INT_CONST) {
    cerr << "Error processing int constant. Expected: " 
      << "int_const. got "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << "\n";
  }

  *o_stream_ << "<integerConstant>" << tokenizer_->intVal()
    << "</integerConstant>\n";
  tokenizer_->advance();
}

void CompilationEngine::processStringConstant() {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::STRING_CONST) {
    cerr << "Error processing string constant. Expected: " 
      << "string_const. got "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << "\n";
  }

  *o_stream_ << "<stringConstant>" << tokenizer_->stringVal()
    << "</stringConstant>\n";
  tokenizer_->advance();
}

void CompilationEngine::processIdentifier() {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
    cerr << "Error processing identifier. Expected: " 
      << "identifier. Got "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << "\n";
  }

  *o_stream_ << "<identifier>" << tokenizer_->Identifier() << "</identifier>\n";
  tokenizer_->advance();
}
}
