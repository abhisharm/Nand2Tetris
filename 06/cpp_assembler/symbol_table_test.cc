#define BOOST_TEST_MODULE symbol_table test
#include <boost/test/included/unit_test.hpp>
#include "symbol_table.h"
#include <optional>

using assembler::SymbolTable;

BOOST_AUTO_TEST_CASE( add_and_get_symbols ) {

  SymbolTable st;
  std::string var = "var";
  BOOST_CHECK( st.AddSymbol(var, 16) );
  std::optional<int64_t> result = st.GetSymbol(var);
  BOOST_REQUIRE( result.has_value() );
  BOOST_CHECK( result.value() == 16 );
  std::string var2 = "varsecond";
  BOOST_CHECK( st.AddSymbol(var2, 17) );
  std::optional<int64_t> result2 = st.GetSymbol(var2);
  BOOST_REQUIRE( result2.has_value() );
  BOOST_CHECK( result2.value() == 17 );
}

BOOST_AUTO_TEST_CASE( cant_re_add_symbol ) {

  SymbolTable st;
  std::string var = "var";
  BOOST_CHECK( st.AddSymbol(var, 16) );
  std::optional<int64_t> result = st.GetSymbol(var);
  BOOST_REQUIRE( result.has_value() );
  BOOST_CHECK( result.value() == 16 );
  BOOST_CHECK( !st.AddSymbol(var, 17) );
}

BOOST_AUTO_TEST_CASE( default_symbols) {

  SymbolTable st;
  std::optional<int64_t> result = st.GetSymbol("SCREEN");
  BOOST_REQUIRE(result.has_value() );
  BOOST_CHECK( result.value() == 16384 );
}
// EOF
