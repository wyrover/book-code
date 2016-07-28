#include <iomanip>
#include <ostream>
#include <sstream>

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

std::string node_impl::evaluate_string()
const
{
  std::ostringstream stream{};
  stream << evaluate();
  return stream.str();
}


node_void::node_void()
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


node_identifier::node_identifier(std::string identifier)
: node_impl{}, identifier_{std::move(identifier)}
{}

std::string const& node_identifier::identifier()
const
{
  return identifier_;
}

void node_identifier::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << identifier() << '\n';
}

double node_identifier::evaluate_node()
const
{
  return get_variable(identifier());
}

std::string node_identifier::evaluate_string()
const
{
  return identifier();
}


node_assign::node_assign(node identifier, node value)
: node_impl{}, identifier_{identifier}, value_{value}
{}

node node_assign::identifier()
const
{
  return identifier_;
}

node node_assign::value()
const
{
  return value_;
}

std::string node_assign::get_identifier()
const
{
  return identifier().to_string();
}

double node_assign::evaluate_value()
const
{
  return value().evaluate();
}

void node_assign::print_node(std::ostream& stream, int indent)
const
{
  stream << std::setw(indent) << "" << get_identifier() << ":=\n";
  value().print(stream, indent + 2);
}

double node_assign::evaluate_node()
const
{
  double result( evaluate_value() );
  set_variable(get_identifier(), result);
  return result;
}



node_unary::node_unary(node operand)
: node_impl{}, operand_{operand}
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
: node_binary{left, right}
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
: node_binary{left, right}
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
: node_binary{left, right}
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
: node_binary{left, right}
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
