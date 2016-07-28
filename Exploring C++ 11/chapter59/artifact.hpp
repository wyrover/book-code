// Listing 59-5. Adding Variable Storage and Lookup to the artifact Class
#ifndef ARTIFACT_HPP_
#define ARTIFACT_HPP_

#include <chrono>
#include <string>

#include "variables.hpp"

class artifact
{
public:
  typedef  std::chrono::system_clock clock;
  artifact()
  : name_{}, mod_time_{clock::time_point{}}, variables_{nullptr}
  {}
  artifact(std::string const& name)
  : name_{name}, mod_time_{get_mod_time()}, variables_{nullptr}
  {}
  ~artifact()
  {
    delete variables_;
  }

  std::string const& name()     const { return name_; }
  clock::time_point  mod_time() const { return mod_time_; }
  std::string        expand(std::string str) const
  {
    return ::expand(str, variables_);
  }

  /// Build a target.
  /// After completing the actions (not yet implemented),
  /// update the modification time.
  void build();

  /// Look up the modification time of the artifact.
  /// Return time_point{} if the artifact does not
  /// exist (and therefore must be built) or if the time cannot
  /// be obtained for any other reason.
  clock::time_point get_mod_time()
  {
    // Real programs should get this information from the
    // operating system. This program returns the current time.
    return clock::now();
  }

  /// Store a per-target variable.
  void store_variable(std::string const& name, std::string const& value)
  {
    if (variables_ == nullptr)
      variables_ = new variable_map;
    (*variables_)[name] = value;
  }
private:
  std::string name_;
  clock::time_point mod_time_;
  variable_map* variables_;
};

#endif // ARTIFACT_HPP_
