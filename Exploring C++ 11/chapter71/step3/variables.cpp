#include <map>

#include "variables.hpp"

namespace {
  std::map<std::string, double> variables;

  class initializer {
  public:
    initializer() {
      variables["pi"] = 3.141592653589792;
      variables["e"]  = 2.718281828459;
    }
  };
  initializer init;
}

double get_variable(std::string const& name)
{
  return variables[name];
}

void set_variable(std::string name, double value)
{
  variables[std::move(name)] = value;
}
