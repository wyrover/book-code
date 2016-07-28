// Templates for Ex4_07

// List all elements
template<typename Container>
void list_elements(const Container& container)
{
    for (const auto& element : container)
        std::cout << element.first << "  " << element.second << std::endl;
}

// List range of elements
template<typename T>
void list_range(const T& pr)
{
    if (pr.first != pr.second) {
        for (auto iter = pr.first; iter != pr.second; ++iter)
            std::cout << "  " << iter->first << "  " << iter->second << std::endl;
    } else
        std::cout << "No records found.\n";
}

// Find elements for a key entered from the keyboard - C++ 14 version
/*
template<typename Container>
auto find_elements(const Container& container)
{
  typename Container::key_type key {};
  std::cin >> key;
  auto pr = container.equal_range(key);
  return pr;
}
*/

// Find elements for a key entered from the keyboard - C++ 11 version
template<typename Container>
auto find_elements(const Container& container) -> std::pair<typename Container::const_iterator, typename Container::const_iterator> {
    typename Container::key_type key {};
    std::cin >> key;
    auto pr = container.equal_range(key);
    return pr;
}