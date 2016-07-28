// Listing 67-5. The newstring Class Template
#include <algorithm>
#include <cstddef>

template<class Char, class Storage, class Traits = std::char_traits<Char>>
class newstring {
public:
   typedef Char value_type;
   typedef std::size_t size_type;
   typedef typename Storage::iterator iterator;
   typedef typename Storage::const_iterator const_iterator;

   newstring() : storage_() {}
   newstring(newstring&&) = default;
   newstring(newstring const&) = default;
   newstring(Storage const& storage) : storage_(storage) {}
   newstring(Char const* ptr, size_type size) : storage_() {
      resize(size);
      std::copy(ptr, ptr + size, begin());
   }
 
   static const size_type npos = static_cast<size_type>(-1);

   newstring& operator=(newstring const&) = default;
   newstring& operator=(newstring&&) = default;

   void swap(newstring& str) { storage_.swap(str.storage_); }

   Char operator[](size_type i) const { return *(storage_.begin() + i); }
   Char& operator[](size_type i)      { return *(storage_.begin() + i); }

   void resize(size_type size, Char value = Char()) {
     storage_.resize(size, value);
   }
   void reserve(size_type size)    { storage_.reserve(size); }
   size_type size() const noexcept { return storage_.end() - storage_.begin(); }
   size_type max_size() const noexcept { return storage_.max_size(); }
   bool empty() const noexcept     { return size() == 0; }
   void clear()                    { resize(0); }
   void push_back(Char c)          { resize(size() + 1, c); }

   Char const* c_str() const { return storage_.c_str(); }
   Char const* data() const  { return storage_.c_str(); }

   iterator begin()              { return storage_.begin(); }
   const_iterator begin() const  { return storage_.begin(); }
   const_iterator cbegin() const { return storage_.begin(); }
   iterator end()                { return storage_.end(); }
   const_iterator end() const    { return storage_.end(); }
   const_iterator cend() const   { return storage_.end(); }

   size_type find(newstring const& s, size_type pos = 0) const {
      pos = std::min(pos, size());
      const_iterator result( std::search(begin() + pos, end(),
                             s.begin(), s.end(), Traits::eq) );
      if (result == end())
         return npos;
      else
         return result - begin();
   }
         
private:
   Storage storage_;
};

template<class Traits>
class newstringcmp
{
public:
   bool operator()(typename Traits::value_type a, typename Traits::value_type b)
   const
   {
      return Traits::cmp(a, b) < 0;
   }
};

template<class Char, class Storage1, class Storage2, class Traits>
bool operator <(newstring<Char, Storage1, Traits> const& a,
                newstring<Char, Storage2, Traits> const& b)
{
   return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(),
                                       newstringcmp<Traits>());
}

template<class Char, class Storage1, class Storage2, class Traits>
bool operator ==(newstring<Char, Storage1, Traits> const& a,
                 newstring<Char, Storage2, Traits> const& b)
{
   return std::equal(a.begin(), a.end(), b.begin(), b.end(), Traits::eq);
}

