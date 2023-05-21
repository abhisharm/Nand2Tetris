#include <string>
#include <optional>
#include "symbol_table.h"

namespace assembler {

  SymbolTable::SymbolTable() {
    populate_symbol_table();
  }

  bool SymbolTable::AddSymbol(std::string symbol_name, int64_t location) {
    auto it = symbol_to_value_map_.find(symbol_name);
    if (it != symbol_to_value_map_.end()) {
      return false;
    }

    symbol_to_value_map_[symbol_name] = location;
    return true;
  } 

  std::optional<int64_t> SymbolTable::GetSymbol(std::string symbol_name) {
    auto it = symbol_to_value_map_.find(symbol_name);
    if (it == symbol_to_value_map_.end()) {
      return std::nullopt;
    }

    return it->second;
  }

  void SymbolTable::populate_symbol_table() {
    symbol_to_value_map_["R0"] = 0;
    symbol_to_value_map_["R1"] = 1;
    symbol_to_value_map_["R2"] = 2;
    symbol_to_value_map_["R3"] = 3;
    symbol_to_value_map_["R4"] = 4;
    symbol_to_value_map_["R5"] = 5;
    symbol_to_value_map_["R6"] = 6;
    symbol_to_value_map_["R7"] = 7;
    symbol_to_value_map_["R8"] = 8;
    symbol_to_value_map_["R9"] = 9;
    symbol_to_value_map_["R10"] = 10;
    symbol_to_value_map_["R11"] = 11;
    symbol_to_value_map_["R12"] = 12;
    symbol_to_value_map_["R13"] = 13;
    symbol_to_value_map_["R14"] = 14;
    symbol_to_value_map_["R15"] = 15;
    symbol_to_value_map_["SCREEN"] = 16384;
    symbol_to_value_map_["KBD"] = 24576;
    symbol_to_value_map_["SP"] = 0;
    symbol_to_value_map_["LCL"] = 1;
    symbol_to_value_map_["ARG"] = 2;
    symbol_to_value_map_["THIS"] = 3;
    symbol_to_value_map_["THAT"] = 4;
  }
}
