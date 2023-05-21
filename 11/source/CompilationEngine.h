// CompilationEngine definition

#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"

#include <iostream>
#include <string>
#include <utility>

namespace AbhishekJackCompiler {

using AbhishekJackCompiler::JackTokenizer;
using AbhishekJackCompiler::SymbolTable;
using AbhishekJackCompiler::VMWriter;
using std::ostream;
using std::string;

class CompilationEngine {

  public:

  CompilationEngine(JackTokenizer* tokenizer, std::ostream* o_stream,
    string filename); 

  void compileClass();

  void compileClassVarDec(); 

  void compileSubroutine(string className);

  void compileParameterList();

  void compileSubroutineBody(bool is_method, bool is_constructor,
    string fullName);

  void compileVarDec();

  void compileStatements();

  void compileLet();

  void compileIf();

  void compileWhile();

  void compileDo();

  void compileReturn();

  void compileExpression();

  void compileTerm();

  int compileExpressionList();

  private:

  JackTokenizer* tokenizer_;
  std::ostream* o_stream_;
  string filename_;
  SymbolTable class_symbol_table_;
  SymbolTable local_symbol_table_;
  VMWriter vm_writer_;
  string curr_className_;

  static int getAndIncrementBranchCount() {
    static int branch_count;
    int to_return = branch_count;
    branch_count += 1;
    return to_return;
  }

  void processKeyWord(JackTokenizer::KeyWord key_word);

  JackTokenizer::KeyWord processKeyWord();

  char processSymbol(char symbol);

  char processSymbol();

  int processIntegerConstant();

  string processStringConstant();

  string processIdentifier();

  // Determines if the current token is a keyword constant.
  bool isKeywordConstant();

  // Determines if the current token is a unary op.
  bool isUnaryOp();

  std::pair<SymbolTable::Kind, int> getKindAndIndexFromVarName(string varName);

  string getTypeFromVarName(string varName);

  static VMWriter::Segment GetSegmentNameFromKind(SymbolTable::Kind kind);
};
}
