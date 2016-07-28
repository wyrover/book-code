// Listing 58-1. Topological Sort of a Directed Acyclic Graph
#ifndef TOPOLOGICAL_SORT_HPP_
#define TOPOLOGICAL_SORT_HPP_

#include <deque>
#include <stdexcept>

/// Helper function for topological_sort().
/// Finds all the nodes in the graph with no incoming edges,
/// that is, with empty values. Removes each one from the graph
/// and adds it to the set @p nodes.
/// @param[in,out] graph A map of node/set pairs
/// @param[in,out] nodes A queue of nodes with no incoming edges
template<class Graph, class Nodes>
void topsort_clean_graph(Graph& graph, Nodes& nodes)
{
  for (auto iter(graph.begin()), end(graph.end()); iter != end;)
  {
    if (iter->second.empty())
    {
      nodes.push_back(iter->first);
      graph.erase(iter++);  // advance iterator before erase invalidates it
    }
    else
      ++iter;
  }
}

/// Topological sort of a directed acyclic graph.
/// A graph is a map keyed by nodes, with sets of nodes as values.
/// Edges run from values to keys. The sorted list of nodes
/// is copied to an output iterator in reverse order.
/// @param graph The graph
/// @param sorted The output iterator
/// @throws std::runtime_error if the graph contains a cycle
/// @pre Graph::key_type == Graph::mapped_type::key_type
template<class Graph, class OutIterator>
void topological_sort(Graph graph, OutIterator sorted)
{
  std::deque<typename Graph::key_type> nodes{};
  // Start with the set of nodes with no incoming edges.
  topsort_clean_graph(graph, nodes);

  while (not nodes.empty())
  {
    // Grab the first node to process, output it to sorted,
    // and remove it from the graph.
    typename Graph::key_type n{nodes.front()};
    nodes.pop_front();
    *sorted = n;
    ++sorted;

    // Erase n from the graph
    for (auto& node : graph)
    {
      node.second.erase(n);
    }
    // After removing n, find any nodes that no longer
    // have any incoming edges.
    topsort_clean_graph(graph, nodes);
  }
  if (not graph.empty())
    throw std::invalid_argument("Dependency graph contains cycles");
}

#endif // TOPOLOGICAL_SORT_HPP_
