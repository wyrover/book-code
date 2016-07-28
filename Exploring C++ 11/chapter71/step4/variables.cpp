#include <map>

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
    return node();
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
