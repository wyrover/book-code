#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <iostream>
#include <cassert>

namespace NS
{
class Bar : public boost::intrusive_ref_counter<Bar>
{
public:
    Bar() {}
    ~Bar()
    {
        std::cout << "~Bar invoked" << '\n';
    }
};
} // end NS

int main()
{
    boost::intrusive_ptr<NS::Bar> pi(new NS::Bar);
    boost::intrusive_ptr<NS::Bar> pi2(pi);
    assert(pi.get() == pi2.get());
    std::cout << "pi: " << pi.get() << '\n'
              << "pi2: " << pi2.get() << '\n';
    assert(pi->use_count() == pi2->use_count()
           && pi2->use_count() == 2);
    std::cout << "pi->use_count() : " << pi->use_count() << '\n'
              << "pi2->use_count() : " << pi2->use_count() << '\n';
}


