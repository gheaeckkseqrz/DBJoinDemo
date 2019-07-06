#pragma once

#include <iostream>
#include <iomanip>
#include <list>
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

  class Record : public std::vector<Value>
  {
  public:
    template <typename... Params>
    Record(Params... params) : std::vector<Value>({params...}) {}

    bool matchConstraintList(std::list<std::pair<unsigned int, Value>> const &constrains) const
    {
      try
        {
          for (auto const &c : constrains)
            if (this->at(c.first) != c.second)
              return false;
        }
      catch (std::out_of_range const &e)
        {
          return false;
        }
      return true;
    }
  };

public:
  Table(std::string name)
    :_name(std::move(name)) {}

  bool addColumn(std::string name, DataType t)
  {
    if (hasColumn(name))
      return false;
    _columns.emplace_back(std::move(name), t);
    return true;
  }

  bool hasColumn(std::string const &name) const
  {
    return std::find_if(_columns.begin(), _columns.end(),
                        [&name](Columns const &a) {return a.name == name; }) != _columns.end();
  }

  unsigned int columnIndex(std::string const &name) const
  {
    auto find = std::find_if(_columns.begin(), _columns.end(), [&name](Columns const &a) {return a.name == name; });
    if (find == _columns.end())
      return static_cast<unsigned int>(-1);
    return std::distance(_columns.begin(), find);
  }

  DataType columnType(std::string const &name) const
  {
    assert(hasColumn(name));
    return std::find_if(_columns.begin(), _columns.end(),
                        [&name](Columns const &a) {return a.name == name; })->datatype;
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

  Record const &getRecord(unsigned int i) const
  {
    return _rows.at(i);
  }

  std::vector<Record> getRecords(std::list<std::pair<unsigned int, Value>> const &constrains) const
  {
    std::vector<Record> res;
    for (auto const &r : _rows)
      {
        if (r.matchConstraintList(constrains))
          res.push_back(r);
      }
    return res;
  }

  std::vector<Record> getRecords(std::list<std::pair<std::string, Value>> const &constrains) const
  {
    std::list<std::pair<unsigned int, Value>> integerConstrains;
    for (auto const &c : constrains)
      integerConstrains.push_back(make_pair(columnIndex(c.first), c.second));
    return getRecords(integerConstrains);
  }

  std::size_t size() const
  {
    return _rows.size();
  }

  void print() const
  {
    for (auto const &c : _columns)
      std::cout << std::setw(15) << c.name << '\t';
    std::cout << std::endl;

    for (auto const &r : _rows)
      {
        for (unsigned int i(0) ; i < _columns.size() ; ++i)
          std::visit([](auto&& arg){std::cout << std::setw(15) << arg << '\t';}, r[i]);
        std::cout << std::endl;
      }
  }

  Table join(Table const &o, std::list<std::pair<std::string, std::string>> const &columns, JoinMode mode = JoinMode::INNER)
  {
    for (auto const &c : columns)
      {
        assert(hasColumn(c.first));
        assert(o.hasColumn(c.second));
        assert(columnType(c.first) == o.columnType(c.second));
      }

    Table joinResult(_name + "_UNION_" + o._name);
     // Include all column of first tables
    for (auto const &c : _columns)
      joinResult.addColumn(c.name, c.datatype);
    // Include columns of second tables as long as they're not key
    for (auto const &c : o._columns)
        if (std::find_if(columns.begin(), columns.end(), [c](std::pair<std::string, std::string> k) { return k.second == c.name;}) == columns.end())
          joinResult.addColumn(c.name, c.datatype);
    return joinResult;
  }

private:
  std::string          _name;
  std::vector<Columns> _columns;
  std::vector<Record>  _rows;
};
