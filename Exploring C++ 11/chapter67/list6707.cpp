// Listing 67-7. The array_storage Class Template
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <array>

template<class Char, std::size_t MaxSize>
class array_storage {
public:
   typedef std::array<Char, MaxSize> array_type;
   typedef std::size_t size_type;
   typedef Char value_type;
   typedef typename array_type::iterator iterator;
   typedef typename array_type::const_iterator const_iterator;

   array_storage() : size_(0), string_() { string_[0] = Char(); }

   void swap(array_storage& storage) {
      string_.swap(storage.string_);
      std::swap(size_, storage.size_);
   }
   size_type max_size() const { return string_.max_size() - 1; }
   void reserve(size_type size) {
     if (size > max_size()) throw std::length_error("reserve");
   }
   void resize(size_type newsize, value_type value) {
      if (newsize > max_size())
         throw std::length_error("resize");
      if (newsize > size_)
         std::fill(begin() + size_, begin() + newsize, value);
      size_ = newsize;
      string_[size_] = Char();
   }
   Char const* c_str() const { return &string_[0]; }

   iterator begin()             { return string_.begin(); }
   const_iterator begin() const { return string_.begin(); }
   iterator end()               { return begin() + size_; }
   const_iterator end() const   { return begin() + size_; }

private:
   size_type size_;
   array_type string_;
};
