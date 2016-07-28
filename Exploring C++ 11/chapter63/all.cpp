#include <chrono>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "artifact.hpp"
#include "artifact_hash.hpp"
#include "artifact_impl.hpp"
#include "depgraph.hpp"  // Listing 58-5
#include "variables.hpp" // Listing 59-6

// Listing 59-7. The variables.cpp File Implements the expand Function
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

artifact_impl::artifact_impl()
: name_{}, mod_time_{clock::time_point{}}, variables_{}
{}

artifact_impl::artifact_impl(std::string const& name)
: name_{name}, mod_time_{get_mod_time()}, variables_{}
{}

std::string artifact_impl::expand(std::string str)
const
{
  return ::expand(str, variables_.get());
}

void artifact_impl::build()
{
   std::cout << name() << '\n';
}

artifact::clock::time_point  artifact_impl::get_mod_time()
{
  return clock::now();
}

void artifact_impl::store_variable(std::string const& name, std::string const& value)
{
    if (variables_ == nullptr)
      variables_.reset(new variable_map);
    (*variables_)[name] = value;
}

// Listing 63-4. Implementing the artifact Class

artifact::artifact() : pimpl_{std::make_shared<artifact_impl>()} {}

artifact::artifact(std::string const& name)
: pimpl_(std::make_shared<artifact_impl>(name))
{}

std::string const& artifact::name()
const
{
   return pimpl_->name();
}

artifact::clock::time_point artifact::mod_time()
const
{
   return pimpl_->mod_time();
}

std::string artifact::expand(std::string str)
const
{
   return pimpl_->expand(str);
}

void artifact::build()
{
   pimpl_->build();
}

artifact::clock::time_point artifact::get_mod_time()
{
   return pimpl_->get_mod_time();
}

void artifact::store_variable(std::string const& name, std::string const& value)
{
    pimpl_->store_variable(name, value);
}

// Listing 63-6. Rewriting the Program to Use the New artifact Value Class
void parse_graph(std::istream& in, dependency_graph& graph)
{
  std::map<std::string, artifact> artifacts{};
  std::string line{};
  while (std::getline(in, line))
  {
    std::string target_name{}, dependency_name{};
    std::istringstream stream{line};
    if (stream >> target_name >> dependency_name)
    {
      artifact target{expand(target_name, nullptr)};
      if (artifacts.find(target.name()) == artifacts.end())
        artifacts.emplace(target.name(), target);
      std::string::size_type equal{dependency_name.find('=')};
      if (equal == std::string::npos)
      {
        // It's a dependency specification
        artifact dependency{target.expand(dependency_name)};
        if (artifacts.find(dependency.name()) == artifacts.end())
          artifacts.emplace(dependency.name(), dependency);
        graph.store_dependency(target, dependency);
      }
      else
        // It's a target-specific variable
        target.store_variable(dependency_name.substr(0, equal-1),
                              dependency_name.substr(equal+1));
    }
    else if (not target_name.empty())
    {
      std::string::size_type equal{target_name.find('=')};
      if (equal == std::string::npos)
        // Input line has a target with no dependency,
        // so report an error.
        std::cerr << "malformed input: target, " << target_name <<
                     ", must be followed by a dependency name\n";
      else
        global_variables[target_name.substr(0, equal)] =
                                          target_name.substr(equal+1);
    }
    // else ignore blank lines
  }
}

int main()
{
  dependency_graph graph{};

  parse_graph(std::cin, graph);
  graph.dump();

  try {
    // Get the sorted artifacts in reverse order.
    std::vector<artifact> sorted{};
    graph.sort(std::back_inserter(sorted));

    // Then print the artifacts in the correct order.
    for (auto it(sorted.rbegin()), end(sorted.rend());
         it != end;
         ++it)
    {
      std::cout << it->name() << '\n';
    }
  } catch (std::runtime_error const& ex) {
    std::cerr << ex.what() << '\n';
    return EXIT_FAILURE;
  }
}
