#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <map>
#include <string>

namespace AbhishekJackCompiler {

using std::string;

class SymbolTable {
 public:
  enum Kind {
    NONE,
    STATIC,
    FIELD,
    ARG,
    VAR,
  };

  SymbolTable();

  // Sets the count of every kind to 0
  void reset();

  void define(string name, string type, Kind kind);

  int varCount(Kind kind);

  Kind kindOf(string name);

  string typeOf(string name);

  int indexOf(string name);

 private:
  std::map<Kind, int> kind_counts_;
  std::map<string, Kind> name_kind_;
  std::map<string, string> name_type_;
  std::map<string, int> name_id_;
};

}

#endif
