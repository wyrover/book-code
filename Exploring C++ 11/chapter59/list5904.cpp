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
      artifact* target{lookup_artifact(expand(target_name))};
      artifact* dependency{lookup_artifact(expand(dependency_name))};
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
        variables[variable_name] = variable_value;
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
