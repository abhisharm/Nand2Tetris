// CompilationEngine definition

#include "JackTokenizer.h"

#include <iostream>
#include <string>

namespace JackCompiler {

using JackCompiler::JackTokenizer;
using std::ostream;
using std::string;

class CompilationEngine {
  
  public:

  CompilationEngine(JackTokenizer* tokenizer, std::ostream* o_stream) :
    tokenizer_(tokenizer), o_stream_(o_stream) {}

  void compileClass();

  void compileClassVarDec(); 

  void compileSubroutine();

  void compileParameterList();

  void compileSubroutineBody();

  void compileVarDec();

  void compileStatements();

  void compileLet();

  void compileIf();

  void compileWhile();

  void compileDo();

  void compileReturn();

  void compileExpression();

  void compileTerm();

  void compileTermInternal();

  int compileExpressionList();

  private:

  JackTokenizer* tokenizer_;
  std::ostream* o_stream_;

  void processKeyWord(JackTokenizer::KeyWord key_word);

  void processKeyWord();

  void processSymbol(char symbol);

  void processSymbol();

  void processIntegerConstant();

  void processStringConstant();

  void processIdentifier();

  // Determines if the current token is a keyword constant.
  bool isKeywordConstant();

  // Determines if the current token is a unary op.
  bool isUnaryOp();
};
}
