#include <string>
#include <iostream>

int main()
{
    std::string s = "Charles Darwin";
    std::cout << s.find("ar") << '\n';            // Search from the
    // beginning
    std::cout << s.rfind("ar") << '\n';           // Search from the end
    std::cout << s.find_first_of("swi")           // Find the first of
              << '\n';                            // any of these chars
    std::cout << s.find_first_not_of("Charles")   // Find the first
              << '\n';                            // that's not in this
    // set
    std::cout << s.find_last_of("abg") << '\n';   // Find the first of
    // any of these chars
    // starting from the
    // end
    std::cout << s.find_last_not_of("aDinrw")     // Find the first
              << '\n';                            // that's not in this
    // set, starting from
    // the end
}
