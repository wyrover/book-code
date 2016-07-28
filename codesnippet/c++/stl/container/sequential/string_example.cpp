#include <iostream>
#include <string>

int main(int argc, const char *argv[])
{

    // Item 15. Be aware of variations in string implementations. [ effective stl ]   
    // Item 13. Prefer vector and string to dynamically allocated arrays. [ effective stl ]
    // Many string implementations employ reference counting behind the scenes (see Item 15), 
    // a strategy that eliminates some unnecessary memory allocations and copying of characters and 
    // that can improve performance for many applications. In fact, the ability to optimize strings 
    // via reference counting was considered so important, the C++ Standardization Committee took specific 
    // steps to make sure it was a valid implementation.
    std::string s1("abcdefg");      
    std::string s2("abcdefg", 4);      
    std::string s3(10, 'a');
    std::string s4(s3);
    std::string s5(s3, 5);
    std::string s6(s3, 2, 10);
    std::string s7(s1.begin(), s1.begin() + 3);
    
    std::cout << s1 << std::endl;        
    std::cout << s2 << std::endl;        
    std::cout << s3 << std::endl;        
    std::cout << s4 << std::endl;        
    std::cout << s5 << std::endl;        
    std::cout << s6 << std::endl;        
    std::cout << s7 << std::endl;        

    // string compare

    if (s5.compare(s4) == 0) {
        std::cout << "s5 == s4" << std::endl;
    }

    if (s5 == s4) { // <, <=, >=, >
        std::cout << "s5 == s4" << std::endl;
    }

    if (strcmp(s5.c_str(), s4.c_str()) == 0) {
        std::cout << "s5 == s4" << std::endl;
    }     

    return 0;
}
