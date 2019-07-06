#include "Table.h"

int main(int ac, char **av)
{
  Table a("Properties");
  a.addColumn("Molecule", Table::DataType::STRING);
  a.addColumn("Solubility", Table::DataType::NUMBER);
  a.addColumn("Molecular Weight", Table::DataType::NUMBER);

  a.addRecord({"Paracetamol", 4.97, 151});
  a.addRecord({"Caffeine", 5.05, 194});
  a.addRecord({"Indomethacin", 0.4, 358});
  a.addRecord({"Trimethoprim", 3.14, 290});

  a.print();

  std::cout << "\n\n";

  Table b("Effects");
  b.addColumn("Molecule", Table::DataType::STRING);
  b.addColumn("Effect", Table::DataType::STRING);
  b.addRecord({"Paracetamol", "Treat pain"});
  b.addRecord({"Caffeine", "Wakes you up"});
  b.addRecord({"Ethanol", "Gets you drunk"});
  b.print();

  std::cout << "\n\n";

  Table j = a.join(b, {std::make_pair("Molecule", "Molecule")});
  j.print();

  return 0;
}
