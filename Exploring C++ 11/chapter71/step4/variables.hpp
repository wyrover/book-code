#ifndef VARIABLES_HPP_
#define VARIABLES_HPP_

#include <map>
#include <string>

#include "node.hpp"

/** Represent a global or local symbol table, which
 * stores variables and functions. In fact, there's no difference
 * between a variable and a function of no arguments.
 */
typedef std::map<std::string, node> symbol_table;

/** Set a temporary symbol table, for the duration of a single function.
 * When the set_symbol_table object goes out of scope,
 * the local symbol table is no longer in force.
 */
class set_symbol_table
{
  public:
    set_symbol_table(symbol_table const& symtab);
    ~set_symbol_table();
};

/** Get the value of a variable.
 * @param name The variable name
 * @return The value of variable @p name or node() if the variable is undefined.
 */
node get_variable(std::string const& name);

/** Get the definition of a function.
 * @param name The function name
 * @return The definition of function named @p name
 * @throw no_such_function if the function is not defined
 */
node get_function(std::string const& name);

/** Set the value of a variable.
 * @param name The variable name
 * @param value The value. if the variable is already defined, changes its value.
 */
void set_variable(std::string const& name, node value);

/** Set the definition of a function.
 * @param name The function name
 * @param definition The function definition
 */
void set_function(std::string const& name, node definition);

#endif
