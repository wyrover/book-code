#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#endif

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

using boost::multi_index_container;
namespace bm = boost::multi_index;

struct employee
{
	int id;
	std::string name;
	int age;

	employee(int id_, const std::string& name_, int age_) : id(id_), name(name_), age(age_) {}

	friend std::ostream& operator<<(std::ostream& os, const employee& e)
	{
		os << e.id << " " << e.name << " " << e.age << std::endl;
		return os;
	}
};

// tags
struct id {};
struct name {};
struct age {};

// Define a multi_index_container of employees with following indics:
//  - a unique index sorted by employee::int,
//  - a non-unique index sorted by employee::name,
//  - a non-unique index sorted by employee::age.
typedef multi_index_container<
	employee,
	bm::indexed_by<
		bm::ordered_unique<bm::tag<id>, BOOST_MULTI_INDEX_MEMBER(employee, int, id)>,
		bm::ordered_non_unique<bm::tag<name>, BOOST_MULTI_INDEX_MEMBER(employee, std::string, name)>,
		bm::ordered_non_unique<bm::tag<age>, BOOST_MULTI_INDEX_MEMBER(employee, int, age)>
	>
> employee_set;

template <typename Tag, typename MultiIndexContainer>
void print_out_by(const MultiIndexContainer& s)
{
	const typename bm::index<MultiIndexContainer, Tag>::type& i = bm::get<Tag>(s);
	typedef typename MultiIndexContainer::value_type value_type;
	std::copy(i.begin(), i.end(), std::ostream_iterator<value_type>(std::cout));
}

int main()
{
	employee_set es;

	es.insert(employee(0, "Joe", 31));
	es.insert(employee(1, "Robert", 27));
	es.insert(employee(2, "John", 40));

	es.insert(employee(2, "Aristotle", 2387));  // fail, same ID

	es.insert(employee(3, "Albert", 20));
	es.insert(employee(4, "John", 57));


	std::cout << "by ID" << std::endl;
	print_out_by<id>(es);
	std::cout << std::endl;

	std::cout << "by name" << std::endl;
	print_out_by<name>(es);
	std::cout << std::endl;

	std::cout << "by age" << std::endl;
	print_out_by<age>(es);
	std::cout << std::endl;

	return 0;
}

