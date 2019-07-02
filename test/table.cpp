#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "Table.h"

TEST_CASE( "Initial State" )
{
  Table t;
  REQUIRE( t.size() == 0 );
}
