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

void node_impl::save(std::ostream& stream)
const
{
  save_node(stream);
}

/* Factory function to create node_impl objects from a saved library file. */
std::shared_ptr<node_impl> node_impl::read_node(std::istream& stream)
{
  std::string type{};
  if (not (stream >> type))
    return nullptr;
  if (type == "void")
    return std::make_shared<node_void>(stream);
  if (type == "number")
    return std::make_shared<node_number>(stream);
  if (type == "identifier")
    return std::make_shared<node_identifier>(stream);
  if (type == "function")
    return std::make_shared<node_function>(stream);
  if (type == "call")
    return std::make_shared<node_function_call>(stream);
  if (type == "negate")
    return std::make_shared<node_negate>(stream);
  if (type == "add")
    return std::make_shared<node_add>(stream);
  if (type == "subtract")
    return std::make_shared<node_subtract>(stream);
  if (type == "multiply")
    return std::make_shared<node_multiply>(stream);
  if (type == "divide")
    return std::make_shared<node_divide>(stream);

  throw calc_error{"unknown node type: " + type};
}

node_void::node_void()
: node_impl{}
{}

node_void::node_void(std::istream&)
: node_impl{}
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

void node_void::save_node(std::ostream& stream)
const
{
  stream << "void\n";
}

node_number::node_number(double value)
: node_impl{}, value_{value}
{}

node_number::node_number(std::istream& stream)
: node_impl{}
{
  if (not (stream >> value_))
    throw calc_error{"malformed library file, cannot read number"};
}

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

void node_number::save_node(std::ostream& stream)
const
{
  stream << "number " << value() << '\n';
}


node_identifier::node_identifier(std::string name)
: node_impl{}, name_{std::move(name)}
{}

node_identifier::node_identifier(std::istream& stream)
: node_impl{}
{
  if (not (stream >> name_))
    throw calc_error{"malformed library file, cannot read identifier"};
}

std::string const& node_identifier::name()
const
{
  return name_;
}

void node_identifier::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << "identifier " << name() << '\n';
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

void node_identifier::save_node(std::ostream& stream)
const
{
  stream << "identifier " << name() << '\n';
}


node_function::node_function(identifier_list parameters, node definition)
: node_impl{}, parameters_{std::move(parameters)}, definition_(definition)
{}

node_function::node_function(std::istream& stream)
: node_impl{}
{
  std::size_t size{};
  if (not (stream >> size))
    throw calc_error{"malformed library file, cannot read function"};
  parameters_.reserve(size);
  while (size-- != 0) {
    std::string parameter{};
    if (not (stream >> parameter))
      throw calc_error{"malformed library file, cannot read function parameter"};
    parameters_.emplace_back(std::move(parameter));
  }
  definition_ = node(stream);
}

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

void node_function::save_node(std::ostream& stream)
const
{
  stream << "function " << parameters().size() << ' ';
  for (auto const& parameter : parameters())
    stream << parameter << ' ';
  definition().save(stream);
}


node_function_call::node_function_call(std::string name, node_list arguments)
: node_impl{}, name_{std::move(name)}, arguments_{std::move(arguments)}
{}

node_function_call::node_function_call(std::istream& stream)
: node_impl{}
{
  std::string name{};
  if (not (stream >> name_))
    throw calc_error{"malformed library file, cannot read function call name"};
  std::size_t size{};
  if (not (stream >> size))
    throw calc_error{"malformed library file, cannot read function call"};
  arguments_.reserve(size);
  while (size-- != 0) {
    arguments_.emplace_back(stream);
  }
}

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
  node function{ get_function(name()) };
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

void node_function_call::save_node(std::ostream& stream)
const
{
  stream << "call " << name() << ' ' << arguments().size() << ' ';
  for (auto const& arg : arguments())
    arg.save(stream);
}


node_unary::node_unary(node operand)
: node_impl{}, operand_{operand}
{}

node_unary::node_unary(std::istream& stream)
: node_impl{}, operand_{stream}
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
: node_impl{}, left_{left}, right_{right}
{}

node_binary::node_binary(std::istream& stream)
: node_impl{}, left_{stream}, right_{stream}
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
: node_unary{operand}
{}

node_negate::node_negate(std::istream& stream)
: node_unary{stream}
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

void node_negate::save_node(std::ostream& stream)
const
{
  stream << "negate ";
  operand().save(stream);
}



node_add::node_add(node left, node right)
: node_binary{left, right}
{}

node_add::node_add(std::istream& stream)
: node_binary{stream}
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

void node_add::save_node(std::ostream& stream)
const
{
  stream << "add ";
  left().save(stream);
  right().save(stream);
}


node_subtract::node_subtract(node left, node right)
: node_binary{left, right}
{}

node_subtract::node_subtract(std::istream& stream)
: node_binary{stream}
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

void node_subtract::save_node(std::ostream& stream)
const
{
  stream << "subtract ";
  left().save(stream);
  right().save(stream);
}


node_multiply::node_multiply(node left, node right)
: node_binary{left, right}
{}

node_multiply::node_multiply(std::istream& stream)
: node_binary{stream}
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

void node_multiply::save_node(std::ostream& stream)
const
{
  stream << "multiply ";
  left().save(stream);
  right().save(stream);
}


node_divide::node_divide(node left, node right)
: node_binary{left, right}
{}

node_divide::node_divide(std::istream& stream)
: node_binary{stream}
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

void node_divide::save_node(std::ostream& stream)
const
{
  stream << "divide ";
  left().save(stream);
  right().save(stream);
}
