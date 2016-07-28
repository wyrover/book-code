#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#endif

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/global_fun.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <iostream>
#include <string>

using boost::multi_index_container;
namespace bm = boost::multi_index;

struct name_record
{
	name_record(const std::string& given_name, const std::string& family_name) :
		given_name_(given_name), family_name_(family_name)
	{
	}

	std::string name() const
	{
		std::string s = family_name_;
		s += " ";
		s += given_name_;
		return s;
	}

private:
	std::string given_name_;
	std::string family_name_;
};

std::string::size_type name_record_length(const name_record& r)
{
	return r.name().size();
}


typedef multi_index_container<
	name_record,
	bm::indexed_by<
		bm::ordered_unique<BOOST_MULTI_INDEX_CONST_MEM_FUN(name_record, std::string, name)>,
		bm::ordered_non_unique<bm::global_fun<const name_record&, std::string::size_type, name_record_length>>
	>
> name_record_set;

int main()
{
	name_record_set ns;

	ns.insert(name_record("Joe", "Smith"));
	ns.insert(name_record("Robert", "Nightingale"));
	ns.insert(name_record("Robert", "Brown"));
	ns.insert(name_record("Marc", "Tuxedo"));

	std::cout << "Phonebook order" << std::endl;
	std::cout << "---------------" << std::endl;
	for (name_record_set::iterator it = ns.begin(); it != ns.end(); ++it)
	{
		std::cout << it->name() << std::endl;
	}

	std::cout << "\nLength order" << std::endl;
	std::cout << "--------------" << std::endl;
	for (bm::nth_index<name_record_set, 1>::type::iterator it1 = bm::get<1>(ns).begin(); it1 != bm::get<1>(ns).end(); ++it1)
	{
		std::cout << it1->name() << std::endl;
	}

	return 0;
}

