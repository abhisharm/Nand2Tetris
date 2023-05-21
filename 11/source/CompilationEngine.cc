#include "CompilationEngine.h"
#include "JackTokenizer.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

namespace AbhishekJackCompiler {

using AbhishekJackCompiler::JackTokenizer;
using std::cerr;
using std::string;

CompilationEngine::CompilationEngine(JackTokenizer* tokenizer,
  std::ostream* o_stream, string filename) : tokenizer_(tokenizer),
    o_stream_(o_stream), filename_(filename), vm_writer_(o_stream) {}

void CompilationEngine::compileClass() {
  class_symbol_table_.reset();
  local_symbol_table_.reset();
  curr_className_ = "";
  processKeyWord(JackTokenizer::KeyWord::CLASS);
  // processing className
  string className = processIdentifier();
  curr_className_ = className;
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
      compileSubroutine(className); 
    } else {
      processing_subroutine_decs = false;
    }
  }

  processSymbol('}');
}

void CompilationEngine::compileClassVarDec() {
  SymbolTable::Kind kind;
  if (tokenizer_->keyWord() == JackTokenizer::KeyWord::STATIC) {
    processKeyWord(JackTokenizer::KeyWord::STATIC);
    kind = SymbolTable::Kind::STATIC;
  }
  else {
    processKeyWord(JackTokenizer::KeyWord::FIELD);
    kind = SymbolTable::Kind::FIELD;
  }

  // processing type
  string type;
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
    if (tokenizer_->keyWord() == JackTokenizer::KeyWord::INT) {
      processKeyWord(JackTokenizer::KeyWord::INT);
      type = "int";
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::CHAR) {
      processKeyWord(JackTokenizer::KeyWord::CHAR);
      type = "char";
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::BOOLEAN) {
      processKeyWord(JackTokenizer::KeyWord::BOOLEAN);
      type = "boolean";
    } else {
      cerr << "Got wrong keyword-type in 'compileClassVarDec'. got: "
        << JackTokenizer::keyWordToString(tokenizer_->keyWord())
        << ". Expected one of int, char, or boolean\n";
    }
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
    type = processIdentifier();
  } else {
    cerr << "Got wrong tokentype in in 'compileClassVarDec'. got: "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType())
      << ". Expected keyword or identifier\n";
  }

  // processing varName
  string name = processIdentifier();
  class_symbol_table_.define(name, type, kind);

  // processing additional local variables, if any
  while (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    tokenizer_->Symbol() == ',') {
    // process semicolon
    processSymbol(',');
    // process varName
    name = processIdentifier();
    class_symbol_table_.define(name, type, kind);
  }
  // process closing semicolon
  processSymbol(';');
}

void CompilationEngine::compileSubroutine(string className) {
  local_symbol_table_.reset();
  // process 
  bool is_method = false;
  bool is_constructor = false;
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
    if (tokenizer_->keyWord() == JackTokenizer::KeyWord::CONSTRUCTOR) {
      processKeyWord(JackTokenizer::KeyWord::CONSTRUCTOR);
      is_constructor = true;
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::FUNCTION) {
      processKeyWord(JackTokenizer::KeyWord::FUNCTION);
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::METHOD) {
      processKeyWord(JackTokenizer::KeyWord::METHOD);
      is_method = true;
      local_symbol_table_.define("this", className, SymbolTable::Kind::ARG);
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
  string subroutineName = processIdentifier();
  processSymbol('(');
  compileParameterList();
  processSymbol(')');
  string fullName = className + "." + subroutineName;
  compileSubroutineBody(is_method, is_constructor, fullName);
  if (is_constructor) {
    vm_writer_.writePush(VMWriter::Segment::POINTER, 0);
    vm_writer_.writeReturn();
  }
}

void CompilationEngine::compileParameterList() {
  // processing type (if present)
  string type;
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
    if (tokenizer_->keyWord() == JackTokenizer::KeyWord::INT) {
      processKeyWord(JackTokenizer::KeyWord::INT);
      type = "int";
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::CHAR) {
      processKeyWord(JackTokenizer::KeyWord::CHAR);
      type = "char";
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::BOOLEAN) {
      processKeyWord(JackTokenizer::KeyWord::BOOLEAN);
      type = "boolean";
    } else {
      cerr << "Got wrong keyword-type in 'compileParameterList'. got: "
        << JackTokenizer::keyWordToString(tokenizer_->keyWord())
        << ". Expected one of int, char, or boolean\n";
    }
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
    // processing type with class name
    type = processIdentifier();
  } else {
    // else no parameter list was passed
    return;
  }

  // processing varName
  string name = processIdentifier();
  local_symbol_table_.define(name, type, SymbolTable::Kind::ARG);
  // processing additional parameters (if present).
  while (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    tokenizer_->Symbol() == ',') {
    processSymbol(',');
    // processing type (now, must be present)
    if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
      if (tokenizer_->keyWord() == JackTokenizer::KeyWord::INT) {
        processKeyWord(JackTokenizer::KeyWord::INT);
        type = "int";
      } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::CHAR) {
        processKeyWord(JackTokenizer::KeyWord::CHAR);
        type = "char";
      } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::BOOLEAN) {
        processKeyWord(JackTokenizer::KeyWord::BOOLEAN);
        type = "boolean";
      } else {
        cerr << "Got wrong keyword-type in 'compileParameterList'. got: "
          << JackTokenizer::keyWordToString(tokenizer_->keyWord())
          << ". Expected one of int, char, or boolean\n";
      }
    } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
      // processing type with class name
      type = processIdentifier();
    } else {
      cerr << "Got wrong token type in 'compileParameterList'. got: "
        << JackTokenizer::tokenTypeToString(tokenizer_->tokenType())
        << ". Expected keyword or identifier\n";
    }

    // processing varName
    name = processIdentifier();
    local_symbol_table_.define(name, type, SymbolTable::Kind::ARG);
  }
}

void CompilationEngine::compileSubroutineBody(
  bool is_method, bool is_constructor, string fullName) {

  processSymbol('{');
  while (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD &&
    tokenizer_->keyWord() == JackTokenizer::KeyWord::VAR) {
    compileVarDec();
  }
  int nVars = local_symbol_table_.varCount(SymbolTable::Kind::VAR);
  vm_writer_.writeFunction(fullName, nVars);
  if (is_method) {
    vm_writer_.writePush(VMWriter::Segment::ARGUMENT, 0);
    vm_writer_.writePop(VMWriter::Segment::POINTER, 0);
  }
  if (is_constructor) {
    int num_fields = class_symbol_table_.varCount(SymbolTable::Kind::FIELD);
    vm_writer_.writePush(VMWriter::Segment::CONSTANT, num_fields);
    vm_writer_.writeCall("Memory.alloc", 1);
    vm_writer_.writePop(VMWriter::Segment::POINTER, 0);
  }
  compileStatements();
  processSymbol('}');
}

void CompilationEngine::compileVarDec() {
  processKeyWord(JackTokenizer::KeyWord::VAR);

  // processing type
  string type;
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD) {
    if (tokenizer_->keyWord() == JackTokenizer::KeyWord::INT) {
      processKeyWord(JackTokenizer::KeyWord::INT);
      type = "int";
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::CHAR) {
      processKeyWord(JackTokenizer::KeyWord::CHAR);
      type = "char";
    } else if (tokenizer_->keyWord() == JackTokenizer::KeyWord::BOOLEAN) {
      processKeyWord(JackTokenizer::KeyWord::BOOLEAN);
      type = "boolean";
    } else {
      cerr << "Got wrong keyword-type in 'compileVarDec'. got: "
        << JackTokenizer::keyWordToString(tokenizer_->keyWord())
        << ". Expected one of int, char, or boolean\n";
    }
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
    type = processIdentifier();
  } else {
    cerr << "Got wrong tokentype in in 'compileVarDec'. got: "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType())
      << ". Expected keyword or identifier\n";
  }

  // processing varName
  string name = processIdentifier();
  local_symbol_table_.define(name, type, SymbolTable::Kind::VAR);

  // processing additional variable-declarations (if present)
  while (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    tokenizer_->Symbol() == ',') {
    processSymbol(',');
    name = processIdentifier();
    local_symbol_table_.define(name, type, SymbolTable::Kind::VAR);
  }

  processSymbol(';');
}

void CompilationEngine::compileStatements() {
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
}

void CompilationEngine::compileLet() {
  processKeyWord(JackTokenizer::KeyWord::LET);
  // process varName
  string varName = processIdentifier();
  bool array_assignment = false;
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
     tokenizer_->Symbol() == '[') {
    auto kindAndIndex = getKindAndIndexFromVarName(varName);
    if (kindAndIndex.first != SymbolTable::Kind::NONE) {
      VMWriter::Segment segmentToPush = GetSegmentNameFromKind(kindAndIndex.first);
      vm_writer_.writePush(segmentToPush, kindAndIndex.second);
    }
    processSymbol('[');
    compileExpression();
    processSymbol(']');
    vm_writer_.writeArithmetic(VMWriter::Command::ADD);
    array_assignment = true;
  }
  processSymbol('=');
  compileExpression();
  processSymbol(';');
  if (!array_assignment) {
    auto kindAndIndex = getKindAndIndexFromVarName(varName);
    if (kindAndIndex.first != SymbolTable::Kind::NONE) {
      VMWriter::Segment segmentToPop = GetSegmentNameFromKind(kindAndIndex.first);
      vm_writer_.writePop(segmentToPop, kindAndIndex.second);
    }
  } else {
    vm_writer_.writePop(VMWriter::Segment::TEMP, 0);
    vm_writer_.writePop(VMWriter::Segment::POINTER, 1);
    vm_writer_.writePush(VMWriter::Segment::TEMP, 0);
    vm_writer_.writePop(VMWriter::Segment::THAT, 0);
  }
}

void CompilationEngine::compileIf() {
  processKeyWord(JackTokenizer::KeyWord::IF);
  processSymbol('(');
  compileExpression();
  processSymbol(')');
  processSymbol('{');
  string if_label = "L" + std::to_string(getAndIncrementBranchCount());
  vm_writer_.writeArithmetic(VMWriter::Command::NOT);
  vm_writer_.writeIf(if_label);
  compileStatements();
  processSymbol('}');
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::KEYWORD &&
    tokenizer_->keyWord() == JackTokenizer::KeyWord::ELSE) {
    processKeyWord(JackTokenizer::KeyWord::ELSE); 
    string outside_label = "L" + std::to_string(getAndIncrementBranchCount());
    vm_writer_.writeGoto(outside_label);
    vm_writer_.writeLabel(if_label);
    processSymbol('{');
    compileStatements();
    processSymbol('}');
    vm_writer_.writeLabel(outside_label);
  } else {
    vm_writer_.writeLabel(if_label);
  }
}

void CompilationEngine::compileWhile() {
  processKeyWord(JackTokenizer::KeyWord::WHILE);
  string while_label = "L" + std::to_string(getAndIncrementBranchCount());
  vm_writer_.writeLabel(while_label);
  processSymbol('(');
  compileExpression();
  processSymbol(')');
  vm_writer_.writeArithmetic(VMWriter::Command::NOT);
  string end_while_label = "L" + std::to_string(getAndIncrementBranchCount());
  vm_writer_.writeIf(end_while_label);
  processSymbol('{');
  compileStatements();
  vm_writer_.writeGoto(while_label);
  processSymbol('}');
  vm_writer_.writeLabel(end_while_label);
}

void CompilationEngine::compileDo() {
  processKeyWord(JackTokenizer::KeyWord::DO);
  // process subroutineCall:
  // in practice its compiled as a term/expression
  compileTerm();
  processSymbol(';');
  // Compiling as an expression causes the expression evaluation to be pushed
  // to the top of the stack.
  vm_writer_.writePop(VMWriter::Segment::TEMP, 0);
}

void CompilationEngine::compileReturn() {
  processKeyWord(JackTokenizer::KeyWord::RETURN);
  // put return expression at the top of the stack, if necessary
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::SYMBOL ||
    (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    tokenizer_->Symbol() != ';')) {
    compileExpression();
  }
  processSymbol(';');
  vm_writer_.writeReturn();
}

void CompilationEngine::compileExpression() {
  compileTerm();
  std::vector<char> ops = {'+', '-', '*', '/', '&', '|', '<', '>', '='};
  char op;
  while (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
    (std::find(ops.begin(), ops.end(), tokenizer_->Symbol()) !=
          ops.end())) {
    op = processSymbol();
    compileTerm();

    switch (op) {
	  	case '+':
        vm_writer_.writeArithmetic(VMWriter::Command::ADD);
        break;
 	  	case '-':
        vm_writer_.writeArithmetic(VMWriter::Command::SUB);
        break;
 	  	case '*':
        vm_writer_.writeCall("Math.multiply", 2);
        break;
 	  	case '/':
        vm_writer_.writeCall("Math.divide", 2);
        break;
 	  	case '&':
        vm_writer_.writeArithmetic(VMWriter::Command::AND);
        break;
 	  	case '|':
        vm_writer_.writeArithmetic(VMWriter::Command::OR);
        break;
 	  	case '<':
        vm_writer_.writeArithmetic(VMWriter::Command::LT);
        break;
 	  	case '>':
        vm_writer_.writeArithmetic(VMWriter::Command::GT);
        break;
      case '=':
        vm_writer_.writeArithmetic(VMWriter::Command::EQ);
        break;
      default:
        break;
    }
  }
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
  if (tokenizer_->tokenType() == JackTokenizer::TokenType::INT_CONST) {
    int int_const = processIntegerConstant();
    vm_writer_.writePush(VMWriter::Segment::CONSTANT, int_const);
  } else if (
    tokenizer_->tokenType() == JackTokenizer::TokenType::STRING_CONST) {
    string string_const = processStringConstant();
    int str_len = string_const.length();
    vm_writer_.writePush(VMWriter::Segment::CONSTANT, str_len);
    vm_writer_.writeCall("String.new", 1);
    for (int i = 0; i < str_len; i++) {
      int curr_char = string_const[i];
      vm_writer_.writePush(VMWriter::Segment::CONSTANT, curr_char);
      vm_writer_.writeCall("String.appendChar", 2);
    }
  } else if (isKeywordConstant()) {
    JackTokenizer::KeyWord keyWord = processKeyWord();
    switch (keyWord) {
      case (JackTokenizer::KeyWord::TRUE):
        vm_writer_.writePush(VMWriter::Segment::CONSTANT, 0);
        vm_writer_.writeArithmetic(VMWriter::Command::NOT);
        break;
      case (JackTokenizer::KeyWord::FALSE):
        vm_writer_.writePush(VMWriter::Segment::CONSTANT, 0);
        break;
      case (JackTokenizer::KeyWord::VAL_NULL):
        vm_writer_.writePush(VMWriter::Segment::CONSTANT, 0);
        break;
      case (JackTokenizer::KeyWord::THIS):
        vm_writer_.writePush(VMWriter::Segment::POINTER, 0);
        break;
    }
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::IDENTIFIER) {
    // identifier case requires some lookahead
    std::string identifier = tokenizer_->Identifier();
    tokenizer_->advance();
    if (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
      tokenizer_->Symbol() == '[') {
      // process subscripting operation
      auto kindAndIndex = getKindAndIndexFromVarName(identifier);
      if (kindAndIndex.first != SymbolTable::Kind::NONE) {
        VMWriter::Segment segmentToPop = GetSegmentNameFromKind(kindAndIndex.first);
        vm_writer_.writePush(segmentToPop, kindAndIndex.second);
      } else {
        cerr << "ERROR: got unexpected variable name: " << identifier << "when"
          << " attempting to parse a subscripting operation in a term.";
      }
      processSymbol('[');
      compileExpression();
      processSymbol(']');
      vm_writer_.writeArithmetic(VMWriter::Command::ADD);
      vm_writer_.writePop(VMWriter::Segment::POINTER, 1);
      vm_writer_.writePush(VMWriter::Segment::THAT, 0);
    } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
      (tokenizer_->Symbol() == '.' || tokenizer_->Symbol() == '(')) {
      // process subroutine call
      string subroutineName = identifier;
      bool self_method_call = false;
      bool has_dot = false;
      if (tokenizer_->Symbol() == '.') {
        processSymbol('.');
        subroutineName = processIdentifier();
        has_dot = true;
      } else {
        self_method_call = true;
      }
      int num_args = 0;
      if (has_dot) {
        auto kindAndIndex = getKindAndIndexFromVarName(identifier);
        if (kindAndIndex.first != SymbolTable::Kind::NONE) {
          VMWriter::Segment segmentToPush = GetSegmentNameFromKind(kindAndIndex.first);
          vm_writer_.writePush(segmentToPush, kindAndIndex.second);
          num_args += 1;
          string type = getTypeFromVarName(identifier);
          subroutineName = type + '.' + subroutineName;
        } else {
          subroutineName = identifier + '.' + subroutineName;
        }
      }
      if (self_method_call) {
        vm_writer_.writePush(VMWriter::Segment::POINTER, 0);
        num_args += 1;
        subroutineName = curr_className_ + '.' + subroutineName;
      }
      processSymbol('(');
      num_args += compileExpressionList();
      processSymbol(')');
      vm_writer_.writeCall(subroutineName, num_args);
    } else {
      // process simple simple varname
      auto kindAndIndex = getKindAndIndexFromVarName(identifier);
      if (kindAndIndex.first != SymbolTable::Kind::NONE) {
        VMWriter::Segment segmentToPush = GetSegmentNameFromKind(kindAndIndex.first);
        vm_writer_.writePush(segmentToPush, kindAndIndex.second);
      }
    }
  } else if (tokenizer_->tokenType() == JackTokenizer::TokenType::SYMBOL &&
      tokenizer_->Symbol() == '(') {
    // process expression in parentheses
    processSymbol('(');
    compileExpression();
    processSymbol(')');
  } else if (isUnaryOp()) {
    // process unary op term
    char unary_op = processSymbol();
    compileTerm();
    if (unary_op == '~') {
      vm_writer_.writeArithmetic(VMWriter::Command::NOT);
    } else if (unary_op == '-') {
      vm_writer_.writeArithmetic(VMWriter::Command::NEG);
    } else {
      cerr << "CompilationEngine::compileTerm ERROR: Got unexpected unary up: "
        << unary_op << " , expected either '~' or '-'.";
    }
  } else {
    cerr << "CompilationEngine::compileTerm ERROR: Got unexpected tokens when "
      << "attempting to compile term.\n";
  }
}

int CompilationEngine::compileExpressionList() {
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

  tokenizer_->advance();
  return;
}

JackTokenizer::KeyWord CompilationEngine::processKeyWord() {
  JackTokenizer::KeyWord to_return = JackTokenizer::KeyWord::VAL_NULL;
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::KEYWORD) {
    cerr << "Error processing keyword. Expected: keyword. got " 
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << "\n";
    return to_return;
  }

  to_return = tokenizer_->keyWord();
  tokenizer_->advance();
  return to_return;
}

char CompilationEngine::processSymbol(char symbol) {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::SYMBOL ||
    symbol != tokenizer_->Symbol()) {
    cerr << "Error processing symbol. Expected: " 
      << "(symbol, " << symbol << "). got "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << ", "
      << tokenizer_->Symbol() << "\n";
  }

  char to_return = tokenizer_->Symbol();
  tokenizer_->advance();
  return to_return;
}

char CompilationEngine::processSymbol() {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::SYMBOL) {
    cerr << "Error processing symbol. Expected: symbol. Got "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << "\n";
  }

  char to_return = tokenizer_->Symbol();
  tokenizer_->advance();
  return to_return;
}

int CompilationEngine::processIntegerConstant() {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::INT_CONST) {
    cerr << "Error processing int constant. Expected: " 
      << "int_const. got "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << "\n";
  }
  int to_return = tokenizer_->intVal();
  tokenizer_->advance();
  return to_return;
}

string CompilationEngine::processStringConstant() {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::STRING_CONST) {
    cerr << "Error processing string constant. Expected: " 
      << "string_const. got "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << "\n";
  }

  string to_return = tokenizer_->stringVal();
  tokenizer_->advance();
  return to_return;
}

string CompilationEngine::processIdentifier() {
  if (tokenizer_->tokenType() != JackTokenizer::TokenType::IDENTIFIER) {
    cerr << "Error processing identifier. Expected: " 
      << "identifier. Got "
      << JackTokenizer::tokenTypeToString(tokenizer_->tokenType()) << "\n";
  }

  string to_return = tokenizer_->Identifier();
  tokenizer_->advance();
  return to_return;
}

std::pair<SymbolTable::Kind, int>
CompilationEngine::getKindAndIndexFromVarName(string varName) {
  SymbolTable::Kind kind = local_symbol_table_.kindOf(varName);
  int index;
  if (kind != SymbolTable::Kind::NONE) {
    index = local_symbol_table_.indexOf(varName);
    return std::make_pair(kind, index);
  }

  kind = class_symbol_table_.kindOf(varName);
  index = class_symbol_table_.indexOf(varName);
  return std::make_pair(kind, index);
}

string
CompilationEngine::getTypeFromVarName(string varName) {
  SymbolTable::Kind kind = local_symbol_table_.kindOf(varName);
  if (kind != SymbolTable::Kind::NONE) {
    return local_symbol_table_.typeOf(varName);
  }

  return class_symbol_table_.typeOf(varName);
}

VMWriter::Segment CompilationEngine::GetSegmentNameFromKind(SymbolTable::Kind kind) {
  switch (kind) {
    case (SymbolTable::Kind::STATIC):
      return VMWriter::Segment::STATIC;
    case (SymbolTable::Kind::FIELD):
      return VMWriter::Segment::THIS;
    case (SymbolTable::Kind::ARG):
      return VMWriter::Segment::ARGUMENT;
    case (SymbolTable::Kind::VAR):
      return VMWriter::Segment::LOCAL;
    case (SymbolTable::Kind::NONE):
    default:
      cerr << "Got 'SymbolTable::Kind::NONE' in 'GetSegmentNameFromKind', "
        << "cannot determine the segment name for this variable kind";
      return VMWriter::Segment::LOCAL;
  }
}
}
