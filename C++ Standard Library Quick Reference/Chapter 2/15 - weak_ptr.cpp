#include <iostream>
#include <memory>

void DoSomethingWith(std::string& s)
{
	/* ... */
}

int main()
{
	auto s = std::make_shared<std::string>("SharedString");
	auto w = std::weak_ptr<std::string>(s);		//w.use_count() == s.use_count() == 1
	{
		std::shared_ptr<std::string> s2 = w.lock();
		DoSomethingWith(*s2);					// w.use_count() == s.use_count() == 2
	}											// w.use_count() == s.use_count() == 1
	s.reset();									// w.expired() == true 


	std::cout << std::endl;
}
