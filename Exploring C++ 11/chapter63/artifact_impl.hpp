// Listing 63-5. Defining the Artifact Implementation Class
#ifndef ARTIFACT_IMPL_HPP_
#define ARTIFACT_IMPL_HPP_

#include <cstdlib>
#include <chrono>
#include <memory>
#include <string>
#include "variables.hpp"

class artifact_impl
{
public:
  typedef std::chrono::system_clock clock;
  artifact_impl();
  artifact_impl(std::string const& name);
  artifact_impl(artifact_impl&&) = default;
  artifact_impl(artifact_impl const&) = delete;
  ~artifact_impl() = default;
  artifact_impl& operator=(artifact_impl&&) = default;
  artifact_impl& operator=(artifact_impl&) = delete;

  std::string const& name()     const { return name_; }
  clock::time_point  mod_time() const { return mod_time_; }

  std::string        expand(std::string str) const;
  void               build();
  clock::time_point  get_mod_time();
  void store_variable(std::string const& name, std::string const& value);
private:
  std::string name_;
  clock::time_point mod_time_;
  std::unique_ptr<variable_map> variables_;
};

#endif // ARTIFACT_IMPL_HPP_
