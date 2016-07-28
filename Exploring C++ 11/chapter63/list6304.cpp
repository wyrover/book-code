// Listing 63-4. Implementing the artifact Class
#include "artifact.hpp"
#include "artifact_impl.hpp"

artifact::artifact() : pimpl_{std::make_shared<artifact_impl>()} {}

artifact::artifact(std::string const& name)
: pimpl_(std::make_shared<artifact_impl>(name))
{}

std::string const& artifact::name()
const
{
   return pimpl_->name();
}

artifact::clock::time_point artifact::mod_time()
const
{
   return pimpl_->mod_time();
}

std::string artifact::expand(std::string str)
const
{
   return pimpl_->expand(str);
}

void artifact::build()
{
   pimpl_->build();
}

artifact::clock::time_point artifact::get_mod_time()
{
   return pimpl_->get_mod_time();
}

void artifact::store_variable(std::string const& name, std::string const& value)
{
    pimpl_->store_variable(name, value);
}
