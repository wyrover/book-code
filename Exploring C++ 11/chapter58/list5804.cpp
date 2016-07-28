// Listing 58-4. Second Draft, Adding Modification Times to Artifacts
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "artifact.hpp"
#include "topological_sort.hpp"

typedef std::size_t artifact_index;

class dependency_graph
{
public:
  typedef std::unordered_map<artifact_index, std::unordered_set<artifact_index>> graph_type;

  void store_dependency(artifact_index target, artifact_index dependency)
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

std::vector<artifact> artifacts{};
artifact_index lookup_artifact(std::string const& name)
{
  auto iter( std::find_if(artifacts.begin(), artifacts.end(),
    [&name](artifact const& a) { return a.name() == name; })
  );
  if (iter != artifacts.end())
    return iter - artifacts.begin();
  // Artifact not found, so add it to the end.
  artifacts.push_back(artifact{name});
  return artifacts.size() - 1;
}

int main()
{
  dependency_graph graph{};

  std::string line{};
  while (std::getline(std::cin, line))
  {
    std::string target_name{}, dependency_name{};
    std::istringstream stream{line};
    if (stream >> target_name >> dependency_name)
    {
      artifact_index target{lookup_artifact(target_name)};
      artifact_index dependency{lookup_artifact(dependency_name)};
      graph.store_dependency(target, dependency);
    }
    else if (not target_name.empty())
      // Input line has a target with no dependency,
      // so report an error.
      std::cerr << "malformed input: target, " << target_name <<
                   ", must be followed by a dependency name\n";
    // else ignore blank lines
  }

  try {
    // Get the sorted artifact indices in reverse order.
    std::vector<artifact_index> sorted{};
    graph.sort(std::back_inserter(sorted));
    // Then print the artifacts in the correct order.
    for (auto it(sorted.rbegin()), end(sorted.rend());
         it != end;
         ++it)
    {
      std::cout << artifacts.at(*it).name() << '\n';
    }
  } catch (std::runtime_error const& ex) {
    std::cerr << ex.what() << '\n';
    return EXIT_FAILURE;
  }
}
