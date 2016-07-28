
std::string regex, str;

do
{
    std::cout << "Input regex: ";

    if (!std::getline(std::cin, regex) || regex.empty()) {
        return 0;
    }

    // Without 'boost::regex::no_except' flag this
    // constructor may throw
    const boost::regex e(regex, flag);

    if (e.status()) {
        std::cout << "Incorrect regex pattern!\n";
        continue;
    }

    std::cout << "String to match: ";

    while (std::getline(std::cin, str) && !str.empty()) {
        bool matched = boost::regex_match(str, e);
        std::cout << (matched ? "MATCH\n" : "DOES NOT MATCH\n");
        std::cout << "String to match: ";
    } // end of 'while(std::getline(std::cin, str))'

    std::cout << '\n';
    // Restoring std::cin
    std::cin.ignore();
    std::cin.clear();
} while (1);
} // int main()