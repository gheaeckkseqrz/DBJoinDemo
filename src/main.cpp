#include "Table.h"

int main(int ac, char **av)
{
  Table a;
  a.addColumn("Molecule", Table::DataType::STRING);
  a.addColumn("Solubility", Table::DataType::NUMBER);
  a.addColumn("Molecular Weight", Table::DataType::NUMBER);

  a.addRecord({"Paracetamol", 4.97, 151});
  a.addRecord({"Caffeine", 5.05, 194});
  a.addRecord({"Indomethacin", 0.4, 358});

  a.print();

  return 0;
}
