#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "Table.h"

TEST_CASE( "Initial State" )
{
  Table t;
  REQUIRE( t.size() == 0 );
  REQUIRE( !t.hasColumn("Molecule") );
}

TEST_CASE( "Add Column Normal Use" )
{
  Table t;
  REQUIRE( t.size() == 0 );
  REQUIRE( !t.hasColumn("Molecule") );
  REQUIRE( t.addColumn("Molecule", Table::DataType::STRING) );
  REQUIRE( t.hasColumn("Molecule") );
  REQUIRE( t.size() == 0 );
}

TEST_CASE( "Add Column Twice" )
{
  // Try a double add with same type
  {
    Table t;
    REQUIRE( t.size() == 0 );
    REQUIRE( !t.hasColumn("Molecule") );
    REQUIRE( t.addColumn("Molecule", Table::DataType::STRING) );
    REQUIRE( t.hasColumn("Molecule") );
    REQUIRE( !t.addColumn("Molecule", Table::DataType::STRING) );
    REQUIRE( t.hasColumn("Molecule") );
    REQUIRE( t.size() == 0 );
  }
  // Try a double add with different type
  {
    Table t;
    REQUIRE( t.size() == 0 );
    REQUIRE( !t.hasColumn("Molecule") );
    REQUIRE( t.addColumn("Molecule", Table::DataType::STRING) );
    REQUIRE( t.hasColumn("Molecule") );
    REQUIRE( !t.addColumn("Molecule", Table::DataType::NUMBER) );
    REQUIRE( t.hasColumn("Molecule") );
    REQUIRE( t.size() == 0 );
  }
}


TEST_CASE( "Add Record Normal Use" )
{
  Table t;
  REQUIRE( t.addColumn("Molecule", Table::DataType::STRING) );
  REQUIRE( t.addColumn("Solubility", Table::DataType::NUMBER) );
  REQUIRE( t.addColumn("Molecular Weight", Table::DataType::NUMBER) );

  REQUIRE( t.size() == 0 );
  REQUIRE( t.addRecord({"Paracetamol", 4.97, 151}) );
  REQUIRE( t.addRecord({"Caffeine", 5.05, 194}) );
  REQUIRE( t.addRecord({"Indomethacin", 0.4, 358}) );
  REQUIRE( t.size() == 3 );

  // Double add record
  REQUIRE( t.addRecord({"Indomethacin", 0.4, 358}) );
  REQUIRE( t.size() == 4 );
}

TEST_CASE( "Add Record Bad Datatype" )
{
  Table t;
  REQUIRE( t.addColumn("Molecule", Table::DataType::STRING) );
  REQUIRE( t.addColumn("Solubility", Table::DataType::NUMBER) );
  REQUIRE( t.addColumn("Molecular Weight", Table::DataType::NUMBER) );

  REQUIRE( t.size() == 0 );
  REQUIRE( !t.addRecord({"Indomethacin", 0.4, "358"}) );
  REQUIRE( t.size() == 0 );
}
