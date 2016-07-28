// Listing 59-8. Adding per-Target Variables to parse_graph
void parse_graph(std::istream& in, dependency_graph& graph)
{
  std::string line{};
  while (std::getline(in, line))
  {
    std::string target_name{}, dependency_name{};
    std::istringstream stream{line};
    if (stream >> target_name >> dependency_name)
    {
      artifact* target{lookup_artifact(expand(target_name, 0))};
      std::string::size_type equal{dependency_name.find('=')};
      if (equal == std::string::npos)
      {
        // It's a dependency specification
        artifact* dependency{lookup_artifact(target->expand(dependency_name))};
        graph.store_dependency(target, dependency);
      }
      else
        // It's a target-specific variable
        target->store_variable(dependency_name.substr(0, equal-1),
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
