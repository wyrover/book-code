#include <boost/iterator/filter_iterator.hpp>
#include <boost/assign.hpp>
#include <vector>
#include <string>
#include <iostream>

struct Person {
    std::string name;
    int age;
    std::string bank_ac_no;

    Person(const std::string& name, int years,
           const std::string& ac_no) :
        name(name), age(years), bank_ac_no(ac_no) {}
};

void payout(double sum, const std::string& ac_no)
{
    std::cout << "Credited a sum of " << sum
              << " to bank account number " << ac_no << '\n';
}

template<typename Itertype>
void creditSum(Itertype first, Itertype last, double sum)
{
    while (first != last) {
        payout(sum, first->bank_ac_no);
        first++;
    }
}

bool seventyOrOlder(const Person& person)
{
    return person.age >= 70;
}

int main()
{
    std::vector<Person> people{{"A Smith", 71, "5702750"},
        {"S Bates", 56, "3920774"},
        {"L Townshend", 73, "9513914"},
        {"L Milford", 68, "1108419"},
        {"F Cornthorpe", 81, "8143919"}};
    auto first = boost::make_filter_iterator(seventyOrOlder,
                 people.begin(), people.end());
    auto last = boost::make_filter_iterator(seventyOrOlder,
                                            people.end(), people.end());
    creditSum(first, last, 100);
}

