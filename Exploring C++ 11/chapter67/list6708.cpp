// Listing 67-8. Output Function for newstring
template<class Char, class Storage, class Traits>
std::basic_ostream<Char, Traits>&
  operator<<(std::basic_ostream<Char, Traits>& stream,
             newstring<Char, Storage, Traits> const& string)
{
   typename std::basic_ostream<Char, Traits>::sentry sentry{stream};
   if (sentry)
   {
      bool needs_fill{stream.width() != 0 and string.size() > stream.width()};
      bool is_left_adjust{
         (stream.flags() & std::ios_base::adjustfield) == std::ios_base::left };
      if (needs_fill and not is_left_adjust)
      {
         for (std::size_t i{stream.width() - string.size()}; i != 0; --i)
            stream.rdbuf()->sputc(stream.fill());
      }
      stream.rdbuf()->sputn(string.data(), string.size());
      if (needs_fill and is_left_adjust)
      {
         for (std::size_t i{stream.width() - string.size()}; i != 0; --i)
            stream.rdbuf()->sputc(stream.fill());
      }
   }
   stream.width(0);
   return stream;
}

