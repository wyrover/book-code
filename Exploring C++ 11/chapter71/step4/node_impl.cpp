#include <iomanip>
#include <iterator>
#include <ostream>
#include <sstream>
#include <utility>

#include "calc_error.hpp"
#include "node.hpp"
#include "node_impl.hpp"
#include "variables.hpp"

node_impl::node_impl()
{}

node_impl::~node_impl()
{}

void node_impl::print(std::ostream& stream, int indent)
const
{
  print_node(stream, indent);
}

double node_impl::evaluate()
const
{
  return evaluate_node();
}

std::string node_impl::to_string()
const
{
  return evaluate_string();
}

identifier_list const& node_impl::get_parameters()
const
{
  return evaluate_parameters();
}

std::string node_impl::evaluate_string()
const
{
  std::ostringstream stream{};
  stream << evaluate();
  return stream.str();
}

identifier_list const& node_impl::evaluate_parameters()
const
{
  static identifier_list const empty{};
  return empty;
}


node_void::node_void()
: node_impl()
{}

void node_void::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << "void\n";
}

double node_void::evaluate_node()
const
{
  return 0.0;
}

std::string node_void::evaluate_string()
const
{
  return std::string{};
}


node_number::node_number(double value)
: node_impl{}, value_{value}
{}

double node_number::value()
const
{
  return value_;
}

void node_number::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << value() << '\n';
}

double node_number::evaluate_node()
const
{
  return value();
}

node_identifier::node_identifier(std::string name)
: node_impl{}, name_{std::move(name)}
{}

std::string const& node_identifier::name()
const
{
  return name_;
}

void node_identifier::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << "identifier:" << name() << '\n';
}

double node_identifier::evaluate_node()
const
{
  return get_variable(name()).evaluate();
}

std::string node_identifier::evaluate_string()
const
{
  return name();
}


node_function::node_function(identifier_list parameters, node definition)
: node_impl{}, parameters_{std::move(parameters)}, definition_{definition}
{}

identifier_list const& node_function::parameters()
const
{
  return parameters_;
}

node node_function::definition()
const
{
  return definition_;
}

void print_identifier_list(std::ostream& stream, identifier_list const& identifiers)
{
  stream << '(';
  char const* sep = "";
for (auto const& id : identifiers) {
    stream << sep << id;
    sep = ", ";
  }
  stream << ')';
}

void node_function::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << "fun\n";
  print_identifier_list(stream, parameters());
  stream << '=';
  definition().print(stream, indent + 2);
}

double node_function::evaluate_node()
const
{
  return definition().evaluate();
}

identifier_list const& node_function::evaluate_parameters()
const
{
  return parameters();
}


node_function_call::node_function_call(std::string name, node_list arguments)
: node_impl{}, name_{std::move(name)}, arguments_{std::move(arguments)}
{}

std::string node_function_call::name()
const
{
  return name_;
}

node_list const& node_function_call::arguments()
const
{
  return arguments_;
}

void node_function_call::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << name() << "(\n";
  std::size_t index{0};
  for (auto const& arg : arguments()) {
    stream << std::setw(indent+1) << "" << "arg " << index << ": ";
    arg.print(stream, indent + 2);
    ++index;
  }
  stream << std::setw(indent) << "" << ")\n";
}

double node_function_call::evaluate_node()
const
{
  // Create a local symbol table, assigning all the node values to the parameters.
  node function{get_function(name())};
  identifier_list const& parameters{ function.get_parameters() };
  if (parameters.size() != arguments().size())
    throw function_error{name(), parameters.size(), arguments().size()};
  else
  {
    // Create a local symbol table by assigning the arguments to the function parameters.
    symbol_table locals{};
    identifier_list::const_iterator parm{parameters.begin()};
    for (auto const& arg : arguments()) {
      locals.emplace(*parm, arg);
      ++parm;
    }
    set_symbol_table syms{locals};
    return function.evaluate();
  }
}



node_unary::node_unary(node operand)
: node_impl(), operand_(operand)
{}

node node_unary::operand()
const
{
  return operand_;
}

double node_unary::evaluate_operand()
const
{
  return operand().evaluate();
}

node_binary::node_binary(node left, node right)
: left_(left), right_(right)
{}

node node_binary::left()
const
{
  return left_;
}

node node_binary::right()
const
{
  return right_;
}

double node_binary::evaluate_left()
const
{
  return left().evaluate();
}

double node_binary::evaluate_right()
const
{
  return right().evaluate();
}


node_negate::node_negate(node operand)
: node_unary(operand)
{}

void node_negate::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << "-\n";
  operand().print(stream, indent + 2);
}

double node_negate::evaluate_node()
const
{
  return -evaluate_operand();
}


node_add::node_add(node left, node right)
: node_binary(left, right)
{}

void node_add::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << "+\n";
  left().print(stream, indent + 2);
  right().print(stream, indent + 2);
}

double node_add::evaluate_node()
const
{
  return evaluate_left() + evaluate_right();
}


node_subtract::node_subtract(node left, node right)
: node_binary(left, right)
{}

void node_subtract::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << "-\n";
  left().print(stream, indent + 2);
  right().print(stream, indent + 2);
}

double node_subtract::evaluate_node()
const
{
  return evaluate_left() - evaluate_right();
}


node_multiply::node_multiply(node left, node right)
: node_binary(left, right)
{}

void node_multiply::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << "*\n";
  left().print(stream, indent + 2);
  right().print(stream, indent + 2);
}

double node_multiply::evaluate_node()
const
{
  return evaluate_left() * evaluate_right();
}


node_divide::node_divide(node left, node right)
: node_binary(left, right)
{}

void node_divide::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << "/\n";
  left().print(stream, indent + 2);
  right().print(stream, indent + 2);
}

double node_divide::evaluate_node()
const
{
  return evaluate_left() / evaluate_right();
}
