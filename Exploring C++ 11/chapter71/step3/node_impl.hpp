#ifndef NODE_IMPL_HPP_
#define NODE_IMPL_HPP_

#include <cstdlib>
#include <string>

#include "node.hpp"

/** Base class for all parse tree nodes.
 * The node classes all derive from node_impl.
 * Each derived class overrides print_node(), for debugging
 * and evalute_node() to evaluate the node. A few classes
 * also override evaluate_string(), but most classes can inherit
 * the node_impl::evaluate_string implementation, which prints
 * the result of calling evaluate().
 */
class node_impl {
public:
  node_impl();
  virtual ~node_impl();

  void print(std::ostream& stream, int indent) const;

  double evaluate() const;

  std::string to_string() const;

private:
  node_impl(node_impl const& n) = delete;
  node_impl& operator=(node_impl const& n) = delete;

  virtual void print_node(std::ostream& stream, int indent) const = 0;
  virtual double evaluate_node() const = 0;
  virtual std::string evaluate_string() const;
};

/** Void data type.
 * Currently used only for the node default constructor.
 */
class node_void : public node_impl {
public:
  node_void();
private:
  void print_node(std::ostream& stream, int indent) const override;
  double evaluate_node() const override;
  std::string evaluate_string() const override;
};

/** Number data type.
 * Used for numeric literals.
 */
class node_number : public node_impl {
public:
  node_number(double value);
  double value() const;
private:
  void print_node(std::ostream& stream, int indent) const override;
  double evaluate_node() const override;

  double value_;
};

/** Identifier data type.
 * Used in assignments and for variable references in expressions.
 */
class node_identifier : public node_impl {
public:
  node_identifier(std::string identifier);
  std::string const& identifier() const;
private:
  virtual void print_node(std::ostream& stream, int indent) const override;
  virtual double evaluate_node() const override;
  virtual std::string evaluate_string() const override;

  std::string identifier_;
};

/** Assignment
 * Keep track of an identifier and a value.
 */
class node_assign : public node_impl {
public:
  node_assign(node identifier, node value);
  node identifier() const;
  node value() const;
private:
  std::string get_identifier() const;
  double evaluate_value() const;

  virtual void print_node(std::ostream& stream, int indent) const override;
  virtual double evaluate_node() const override;

  node identifier_;
  node value_;
};

/** Abstract base class for unary operators. */
class node_unary : public node_impl {
public:
  node_unary(node operand);
  node operand()  const;
protected:
  double evaluate_operand() const;
private:
  virtual void print_node(std::ostream& stream, int indent) const override = 0;
  virtual double evaluate_node() const override = 0;

  node operand_;
};

/** Abstract base class for binary operators. */
class node_binary : public node_impl {
public:
  node_binary(node left, node right);
  node left()  const;
  node right() const;
protected:
  double evaluate_left() const;
  double evaluate_right() const;
private:
  virtual void print_node(std::ostream& stream, int indent) const override = 0;
  virtual double evaluate_node() const override = 0;

  node left_;
  node right_;
};

/** Unary negation. */
class node_negate : public node_unary {
public:
  node_negate(node operand);
private:
  virtual void print_node(std::ostream& stream, int indent) const override;
  virtual double evaluate_node() const override;
};

/** Addition. */
class node_add : public node_binary {
public:
  node_add(node left, node right);
private:
  virtual void print_node(std::ostream& stream, int indent) const override;
  virtual double evaluate_node() const override;
};

/** Subtraction. */
class node_subtract : public node_binary {
public:
  node_subtract(node left, node right);
private:
  virtual void print_node(std::ostream& stream, int indent) const override;
  virtual double evaluate_node() const override;
};

/** Multiplication. */
class node_multiply : public node_binary {
public:
  node_multiply(node left, node right);
private:
  virtual void print_node(std::ostream& stream, int indent) const override;
  virtual double evaluate_node() const override;
};

/** Division. */
class node_divide : public node_binary {
public:
  node_divide(node left, node right);
private:
  virtual void print_node(std::ostream& stream, int indent) const override;
  virtual double evaluate_node() const override;
};

#endif
