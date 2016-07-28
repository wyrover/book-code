// Listing 65-2. Mapping a language Value to and from Strings
#include <array>
#include <istream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>


enum class language { apl, low=apl, c, cpp, forth, haskell, jovial, lisp, scala, high=scala };

std::unordered_map<std::string, language> from_string_map_;
std::array<std::string, 8> to_string_map_;

void store(std::string const& name, language lang)
{
   from_string_map_.emplace(name, lang);
   to_string_map_.at(static_cast<int>(lang)) = name;
}

void initialize_language()
{
   store("apl", language::apl);
   store("c", language::c);
   store("cpp", language::cpp);
   store("forth", language::forth);
   store("haskell", language::haskell);
   store("jovial", language::jovial);
   store("lisp", language::lisp);
   store("scala", language::scala);
}

std::string const& to_string(language lang)
{
   return  to_string_map_.at(static_cast<int>(lang));
}

language from_string(std::string const& text)
{
   return from_string_map_.at(text);
}

std::ostream& operator<<(std::ostream& stream, language lang)
{
   stream << to_string(lang);
   return stream;
}

std::istream& operator>>(std::istream& stream, language& lang)
{
   std::string text;
   if (stream >> text)
      lang = from_string(text);
   return stream;
}

