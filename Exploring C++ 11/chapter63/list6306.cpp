// Listing 63-6. Rewriting the Program to Use the New artifact Value Class
#include <chrono>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "artifact.hpp"
#include "artifact_hash.hpp"
#include "depgraph.hpp"  // Listing 58-5
#include "variables.hpp" // Listing 59-6


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
