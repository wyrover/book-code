// Listing 60-3. Adding a Move Constructor to the artifact Class
class artifact {
public:
   typedef std::chrono::system_clock clock;
   artifact(artifact&& source) noexcept
   : name_{std::move(source.name_)},
     mod_time_{std::move(source.mod_time_)},
     variables_{source.variables_}
   {
      source.variables_ = nullptr;
   }

   artifact& operator=(artifact&& source) noexcept
   {
      delete variables_;
      variables_ = source.variables_;
      source.variables_ = nullptr;
      name_ = std::move(source.name_);
      mod_time_ = std::move(source.mod_time_);
      return *this;
   }
   ... rest of artifact ...
};
