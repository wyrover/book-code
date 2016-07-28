#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

class Person {
public:
	Person() = default;
	explicit Person(const std::string& first,
		const std::string& last = "", bool isVIP = false)
		: m_first(first), m_last(last), m_isVIP(isVIP) {}

	const std::string& GetFirstName() const { return m_first; }
	void SetFirstName(const std::string& first) { m_first = first; }

	const std::string& GetLastName() const { return m_last; }
	void SetLastName(const std::string& last) { m_last = last; }

	bool IsVIP() const { return m_isVIP; }

private:
	friend bool operator<(const Person&, const Person&);
	std::string m_first;
	std::string m_last;
	bool m_isVIP = false;
};

// Comparison operator.
bool operator<(const Person& lhs, const Person& rhs) {
	if (lhs.IsVIP() != rhs.IsVIP()) return rhs.IsVIP();
	if (lhs.GetLastName() != rhs.GetLastName())
		return lhs.GetLastName() < rhs.GetLastName();
	return lhs.GetFirstName() < rhs.GetFirstName();
}

// Equality operator.
bool operator==(const Person& lhs, const Person& rhs) {
	return lhs.IsVIP() == rhs.IsVIP() &&
		lhs.GetFirstName() == rhs.GetFirstName() &&
		lhs.GetLastName() == rhs.GetLastName();
}

std::ostream& operator<<(std::ostream& os, const Person& person) {
	os << std::quoted(person.GetFirstName()) << ' '
		<< std::quoted(person.GetLastName());
	return os;
}

std::istream& operator>>(std::istream& is, Person& person) {
	std::string firstName, lastName;
	is >> std::quoted(firstName) >> std::quoted(lastName);
	person.SetFirstName(firstName); person.SetLastName(lastName);
	return is;
}

int main()
{
	Person kurt("Kurt", "von Strohm");
	std::stringstream ss;
	ss << kurt;
	std::cout << ss.str() << '\n';   // "Kurt" "von Strohm"
	ss.seekg(0);                     // Seek back to beginning of stream
	Person readBack;
	ss >> readBack;
	std::cout << readBack << '\n';   // "Kurt" "von Strohm"


	std::cout << std::endl;
}
