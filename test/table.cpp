#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "Table.h"

TEST_CASE( "Initial State" )
{
  Table t("T");
  REQUIRE( t.size() == 0 );
  REQUIRE( !t.hasColumn("Molecule") );
}

TEST_CASE( "Add Column Normal Use" )
{
  Table t("T");
  REQUIRE( t.size() == 0 );
  REQUIRE( !t.hasColumn("Molecule") );
  REQUIRE( t.addColumn("Molecule", Table::DataType::STRING) );
  REQUIRE( t.hasColumn("Molecule") );
  REQUIRE( t.columnType("Molecule") == Table::DataType::STRING );
  REQUIRE( t.size() == 0 );
}

TEST_CASE( "Add Column Twice" )
{
  // Try a double add with same type
  {
    Table t("T");
    REQUIRE( t.size() == 0 );
    REQUIRE( !t.hasColumn("Molecule") );
    REQUIRE( t.addColumn("Molecule", Table::DataType::STRING) );
    REQUIRE( t.hasColumn("Molecule") );
    REQUIRE( t.columnType("Molecule") == Table::DataType::STRING );
    REQUIRE( !t.addColumn("Molecule", Table::DataType::STRING) );
    REQUIRE( t.hasColumn("Molecule") );
    REQUIRE( t.columnType("Molecule") == Table::DataType::STRING );
    REQUIRE( t.size() == 0 );
  }
  // Try a double add with different type
  {
    Table t("T");
    REQUIRE( t.size() == 0 );
    REQUIRE( !t.hasColumn("Molecule") );
    REQUIRE( t.addColumn("Molecule", Table::DataType::STRING) );
    REQUIRE( t.hasColumn("Molecule") );
    REQUIRE( t.columnType("Molecule") == Table::DataType::STRING );
    REQUIRE( !t.addColumn("Molecule", Table::DataType::NUMBER) );
    REQUIRE( t.hasColumn("Molecule") );
    REQUIRE( t.columnType("Molecule") == Table::DataType::STRING ); // Type does not get updated !
    REQUIRE( t.size() == 0 );
  }
}


TEST_CASE( "Add Record Normal Use" )
{
  Table t("T");
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
  Table t("T");
  REQUIRE( t.addColumn("Molecule", Table::DataType::STRING) );
  REQUIRE( t.addColumn("Solubility", Table::DataType::NUMBER) );
  REQUIRE( t.addColumn("Molecular Weight", Table::DataType::NUMBER) );

  REQUIRE( t.size() == 0 );
  REQUIRE( !t.addRecord({"Indomethacin", 0.4, "358"}) );
  REQUIRE( t.size() == 0 );
}

TEST_CASE( "Get Record" )
{
  Table t("T");
  REQUIRE( t.addColumn("Molecule", Table::DataType::STRING) );
  REQUIRE( t.addColumn("Solubility", Table::DataType::NUMBER) );
  REQUIRE( t.addColumn("Weight", Table::DataType::NUMBER) );

  REQUIRE( t.addRecord({"M1", 0.4, 358}) );
  REQUIRE( t.addRecord({"M2", 0.7, 38}) );
  REQUIRE( t.addRecord({"M3", 0.5, 51}) );
  REQUIRE( t.addRecord({"M4", 0.8, 358}) );
  REQUIRE( t.addRecord({"M5", 0.9, 32}) );

  // SELECT * FROM t WHERE Weight == 358
  auto r1 = t.getRecords({std::make_pair("Weight", 358)});
  REQUIRE( r1.size() == 2 );
  REQUIRE( r1[0][0] == Table::Value("M1") );
  REQUIRE( r1[1][0] == Table::Value("M4") );

  // SELECT * FROM t WHERE Weight == 358 AND Dolubility == 0.8
  auto r2 = t.getRecords({std::make_pair("Weight", 358), std::make_pair("Solubility", 0.8)});
  REQUIRE( r2.size() == 1 );
  REQUIRE( r2[0][0] == Table::Value("M4") );
}
