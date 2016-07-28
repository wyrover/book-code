// Listing 58-3. New Definition of an Artifact
#ifndef ARTIFACT_HPP_
#define ARTIFACT_HPP_

#include <chrono>
#include <string>

class artifact
{
public:
  typedef  std::chrono::system_clock clock;
  artifact() : name_{}, mod_time_{clock::time_point{}} {}
  artifact(std::string const& name)
  : name_{name}, mod_time_{get_mod_time()}
  {}

  std::string const& name()     const { return name_; }
  clock::time_point  mod_time() const { return mod_time_; }

  /// Build a target.
  /// After completing the actions (not yet implemented),
  /// update the modification time.
  void build();

  /// Look up the modification time of the artifact.
  /// Return time_point{} if the artifact does not
  /// exist (and therefore must be built) or if the time cannot
  /// be obtained for any other reason.
  /// Also see boost::file_system::last_write_time.
  clock::time_point get_mod_time()
  {
    // Real programs should get this information from the
    // operating system. This program returns the current time.
    return clock::now();
  }
private:
  std::string name_;
  clock::time_point mod_time_;
};

#endif // ARTIFACT_HPP_
