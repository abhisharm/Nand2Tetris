#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <optional>
#include <map>

namespace assembler {

class SymbolTable {

  public:
    // A default-constructor for symbol-table that calls
    // 'populate_symbol_table', so that the symbol-table is properly
    // initialized.
    SymbolTable();

    // Add a symbol to the symbol-table corresponding to the specified
    // location. A symbol may only be added to the symbol-table once.
    // Returns a bool representing true if the symbol was added to the
    // symbol table, and false if it was already present and therefore
    // unable to be added.
    bool AddSymbol(std::string symbol_name, int64_t location);
    
    // Get a symbol from the symbol-table, if it exists.
    // Returns std::nullopt if it does not exist, otherwise an optional
    // with the symbol.
    std::optional<int64_t> GetSymbol(std::string symbol_name);

  private:
    std::map<std::string, int64_t> symbol_to_value_map_;

    // Inserts the default values (e.g, 'R0' for 0, 'SCREEN' etc.) into the
    // symbol-table. Intended to be called on initialization.
    void populate_symbol_table();
};

}
#endif
