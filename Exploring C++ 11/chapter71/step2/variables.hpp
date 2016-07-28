#ifndef VARIABLES_HPP_
#define VARIABLES_HPP_

#include <string>

/** Get the value of a variable.
 * @param name The variable name
 * @return The value of variable @p name or 0.0 if the variable is undefined.
 */
double get_variable(std::string const& name);

/** Set the value of a variable.
 * @param name The variable name
 * @param value The value. if the variable is already defined, changes its value.
 */
void set_variable(std::string name, double value);

#endif
