// Listing 55-3. The main Program for Testing is_palindrome
int main()
{
  std::locale::global(std::locale{""});
  std::wcin.imbue(std::locale{});
  std::wcout.imbue(std::locale{});

  std::wstring line{};
  while (std::getline(std::wcin, line))
    if (is_palindrome(line))
      std::wcout << line << L'\n';
}
