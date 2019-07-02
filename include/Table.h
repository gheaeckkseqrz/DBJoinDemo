#pragma once

#include <iostream>
#include <list>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>


class Table
{
public:
  enum class DataType
    {
     STRING,
     NUMBER
    };

    enum class JoinMode
    {
     INNER,
     OUTER
    };


  struct Columns
  {
    Columns(std::string n, DataType t)
      : name(std::move(n)), datatype(t) {}

    std::string name;
    DataType    datatype;
  };

  using Value = std::variant<std::string, double>;
  using Record = std::vector<Value>;

public:
  Table() = default;

  bool addColumn(std::string name, DataType t)
  {
    _columns.emplace_back(std::move(name), t);
    return true;
  }

  bool hasColumn(std::string const &name) const
  {
    return std::find_if(_columns.begin(), _columns.end(),
                     [&name](Columns const &a) {return a.name == name; }) != _columns.end();
  }

  bool checkStructure(Record const &r) const
  {
    if (r.size() == _columns.size())
      {
        for (unsigned int i(0) ; i < _columns.size() ; ++i)
          {
            if (_columns[i].datatype == DataType::STRING && !std::holds_alternative<std::string>(r[i]))
              {
                std::cerr << "Wrong type for column " << _columns[i].name << ", expected string" << std::endl;
                return false;
              }
            if (_columns[i].datatype == DataType::NUMBER && !std::holds_alternative<double>(r[i]))
              {
                std::cerr << "Wrong type for column " << _columns[i].name << ", expected number" << std::endl;
                return false;
              }
          }
        return true;
      }
    std::cerr << "Wrong number of items (" << r.size() << ") - expected " << _columns.size()  << std::endl;
    return false;
  }

  bool addRecord(Record r)
  {
    if (checkStructure(r))
      {
        _rows.emplace_back(std::move(r));
        return true;
      }
    return false;
  }

  std::size_t size() const
  {
    return _rows.size();
  }

  void print() const
  {
    for (auto const &c : _columns)
      std::cout << c.name << '\t';
    std::cout << std::endl;

    for (auto const &r : _rows)
      {
        for (unsigned int i(0) ; i < _columns.size() ; ++i)
          std::visit([](auto&& arg){std::cout << arg << '\t';}, r[i]);
        std::cout << std::endl;
      }
  }

  Table join(Table const &o, std::list<std::string> const &columns, JoinMode mode = JoinMode::INNER)
  {
    for (auto const &c : columns)
      {
        assert(hasColumn(c));
        assert(o.hasColumn(c));
      }
    return *this;
  }

private:
  std::vector<Columns> _columns;
  std::vector<Record>  _rows;
};
