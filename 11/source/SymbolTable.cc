// Implementation of the SymbolTable class

#include "SymbolTable.h"

#include <string>

namespace AbhishekJackCompiler {

using std::string;

SymbolTable::SymbolTable() {
  reset();
}

void SymbolTable::reset() {
  kind_counts_.insert_or_assign(SymbolTable::Kind::STATIC, 0);
  kind_counts_.insert_or_assign(SymbolTable::Kind::FIELD, 0);
  kind_counts_.insert_or_assign(SymbolTable::Kind::ARG, 0);
  kind_counts_.insert_or_assign(SymbolTable::Kind::VAR, 0);

  name_kind_.clear();
  name_type_.clear();
  name_id_.clear();
}

void SymbolTable::define(string name, string type, Kind kind) {
  int kind_count = kind_counts_[kind];
  kind_counts_[kind] += 1;
  name_id_[name] = kind_count;
  name_type_[name] = type;
  name_kind_[name] = kind;
}

int SymbolTable::varCount(Kind kind) {
  return kind_counts_[kind];
}

SymbolTable::Kind SymbolTable::kindOf(string name) {
  auto search = name_kind_.find(name);
  if (search == name_kind_.end()) {
    return SymbolTable::Kind::NONE;
  }

  return search->second;
}

string SymbolTable::typeOf(string name) {
  return name_type_[name];
}

int SymbolTable::indexOf(string name) {
  return name_id_[name];
}


}
