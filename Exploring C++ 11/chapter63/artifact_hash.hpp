#ifndef ARTIFACT_HASH_HPP_
#define ARTIFACT_HASH_HPP_

#include <functional>
#include <string>

#include "artifact.hpp"

namespace std
{

/** Hash an artifact so it can be used in a unordered container. */
template<>
struct hash<artifact> : std::hash<std::string>
{
   /** Hash an artifact by hashing its name */
   std::size_t operator()(artifact const& a) const
   {
      return static_cast<std::hash<std::string> const&>(*this)(a.name());
   }
};

template<>
struct equal_to<artifact>
{
   typedef artifact first_argument_type;
   typedef artifact second_argument_type;
   typedef bool result_type;

   result_type operator()(artifact const& a, artifact const& b) const
   {
      return a.name() == b.name();
   }
};

} // namespace

#endif
