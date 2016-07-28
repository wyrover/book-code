 1 #include <functional>
 2 ...
 3 std::vector<std::string> names{"Groucho", "Chico", "Harpo"};
 4 std::vector<std::string::size_type> lengths;
 5 using namespace std::placeholders;
 6
 7 std::transform(names.begin(), names.end(), 
 8                std::back_inserter(lengths),
 9                std::bind(&std::string::size, _1));

