// Assemble all the pieces of the pseudo-make program.
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "artifact.hpp"

#include "topological_sort.hpp"

class dependency_graph
{
public:
  typedef std::unordered_map<artifact*, std::unordered_set<artifact*>> graph_type;

  void store_dependency(artifact* target, artifact* dependency)
  {
    graph_[dependency].insert(target);
    graph_[target]; // ensures that target is in the graph
  }

  graph_type const& graph() const { return graph_; }

  template<class OutIter>
  void sort(OutIter sorted)
  const
  {
    topological_sort(graph_, sorted);
  }

private:
  graph_type graph_;
};

std::map<std::string, artifact> artifacts{};

artifact* lookup_artifact(std::string const& name)
{
  auto a( artifacts.find(name) );
  if (a == artifacts.end())
    artifacts.emplace(name, artifact{name});
  return &artifacts[name];
}

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

// Listing 59-4. Parsing Variable Definitions
void parse_graph(std::istream& in, dependency_graph& graph)
{
  std::string line{};
  while (std::getline(in, line))
  {
    std::string target_name{}, dependency_name{};
    std::istringstream stream{line};
    if (stream >> target_name >> dependency_name)
    {
      artifact* target{lookup_artifact(expand(target_name, nullptr))};
      artifact* dependency{lookup_artifact(expand(dependency_name, nullptr))};
      graph.store_dependency(target, dependency);
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
      {
        std::string variable_name{target_name.substr(0, equal)};
        std::string variable_value{target_name.substr(equal+1)};
        global_variables[variable_name] = variable_value;
      }
    }
    // else ignore blank lines
  }
}

int main()
{
  dependency_graph graph{};

  parse_graph(std::cin, graph);

  try {
    // Get the sorted artifacts in reverse order.
    std::vector<artifact*> sorted;
    graph.sort(std::back_inserter(sorted));

    // Then print the artifacts in the correct order.
    for (auto it(sorted.rbegin()), end(sorted.rend());
         it != end;
         ++it)
    {
      std::cout << (*it)->name() << '\n';
    }
  } catch (std::runtime_error const& ex) {
    std::cerr << ex.what() << '\n';
    return EXIT_FAILURE;
  }
}
