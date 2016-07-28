#include <fstream>
#include <istream>
#include <limits>
#include <map>
#include <ostream>

#include "calc_error.hpp"
#include "node.hpp"
#include "variables.hpp"

namespace {
  symbol_table variables;
  std::vector<symbol_table const*> symbol_tables;

  class initializer {
  public:
    initializer() {
      variables["pi"] = node(3.141592653589792);
      variables["e"]  = node(2.718281828459);
      symbol_tables.push_back(&variables);
    }
  };
  initializer init;
}

set_symbol_table::set_symbol_table(symbol_table const& locals)
{
  symbol_tables.push_back(&locals);
}

set_symbol_table::~set_symbol_table()
{
  symbol_tables.pop_back();
}

bool find_symbol(std::string const& name, node& value)
{
  for (auto iter(symbol_tables.rbegin()), end(symbol_tables.rend()); iter != end; ++iter) {
    symbol_table const& table{ **iter };
    symbol_table::const_iterator entry{ table.find(name) };
    if (entry != table.end()) {
      value = entry->second;
      return true;
    }
  }
  return false;
}

node get_variable(std::string const& name)
{
  node result{};
  if (not find_symbol(name, result))
    return node{};
  else if (result.get_parameters().empty())
    return result;
  else
    throw function_error{name, result.get_parameters().size(), 0};
}

void set_variable(std::string const& name, node value)
{
  variables[name] = value;
}

node get_function(std::string const& name)
{
  node result{};
  if (not find_symbol(name, result))
    throw no_such_function{name};
  else
    return result;
}

void set_function(std::string const& name, node value)
{
  set_variable(name, value);
}

void save_library(std::string const& filename)
{
  std::ofstream stream{filename.c_str()};
  if (not stream)
    throw file_error{filename};
  // Be sure to save numbers with maximum precision.
  stream.precision(std::numeric_limits<double>::digits10);
  stream << ":library:\n";
  for (auto const& symbol : variables)
  {
    stream << symbol.first << ' ';
    symbol.second.save(stream);
    if (stream.fail())
      throw file_error{filename};
  }
  stream << "*\n";
  stream.close();
  if (stream.fail())
    throw file_error{filename};
}

void load_library(std::string const& filename)
{
  std::ifstream stream{filename.c_str()};
  if (not stream)
    throw file_error{filename};
  std::string token{};
  if (not std::getline(stream, token))
    throw calc_error{filename + ": is empty"};
  if (token != ":library:")
    throw calc_error{filename + ": is not a calculator library file"};
  while (stream >> token) {
    if (token == "*")
      return;
    else
      set_variable(token, node{stream});
  }
}
