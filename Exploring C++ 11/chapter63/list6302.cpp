// Listing 63-2. Using Smart Pointers to Manage Artifacts
std::map<std::string, std::shared_ptr<artifact>> artifacts;

std::shared_ptr<artifact>
lookup_artifact(std::string const& name)
{
  std::shared_ptr<artifact> a{artifacts[name]};
  if (a.get() == nullptr)
  {
    a = std::make_shared<artifact>(name);
    artifacts[name] = a;
  }
  return a;
}
