// Listing 59-7. The variables.cpp File Implements the expand Function
#include "variables.hpp"

variable_map global_variables{};

// Get a variable's value. Try the local variables first; if not found
// try the global variables. If still not found, define the name with
// an empty string and return an empty string. Subsequent lookups of
// the same name will find the empty string. Exercise for reader:
// print a message the first time the undefined variable's name
// is used.
std::string get_value(std::string const& name, variable_map const* local_variables)
{
   if (local_variables != nullptr)
   {
      variable_map::const_iterator iter{local_variables->find(name)};
      if (iter != local_variables->end())
         return iter->second;
   }
   return global_variables[name];
}

std::string expand(std::string str, variable_map const* local_variables)
{
   std::string::size_type start{0}; // start searching here
   while (true)
   {
      // Find a dollar sign.
      std::string::size_type pos{str.find('$', start)};
      if (pos == std::string::npos)
         // No more dollar signs.
         return str;
      if (pos == str.size() - 1 or str[pos + 1] != '(')
         // Not a variable reference.
         // Skip the dollar sign, and keep searching.
         start = pos + 1;
      else
      {
         std::string::size_type end{str.find(')', pos)};
         if (end == std::string::npos)
            // No closing parenthesis.
            return str;

         // Get the variable name.
         std::string varname{str.substr(pos + 2, end - pos - 2)};
         // Replace the entire variable reference.
         std::string value{get_value(varname, local_variables)};
         str.replace(pos, end - pos + 1, value);
         // Scan the replacement text for more variables.
         start = pos;
      }
   }
}
