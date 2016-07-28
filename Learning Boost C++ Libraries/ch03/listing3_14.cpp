#include <boost/smart_ptr.hpp>
#include <boost/current_function.hpp>
#include <iostream>
#include <cassert>

class CanBeShared
    : public boost::enable_shared_from_this<CanBeShared>
{
public:
    ~CanBeShared()
    {
        std::cout << BOOST_CURRENT_FUNCTION << '\n';
    }

    boost::shared_ptr<CanBeShared> share()
    {
        return shared_from_this();
    }
};

typedef boost::shared_ptr<CanBeShared> CanBeSharedPtr;

void doWork(CanBeShared& obj)
{
    CanBeSharedPtr sp = obj.share();
    std::cout << "Usage count in doWork " << sp.use_count() << '\n';
    assert(sp.use_count() == 2);
    assert(&obj == sp.get());
}

int main()
{
    CanBeSharedPtr cbs = boost::make_shared<CanBeShared>();
    doWork(*cbs.get());
    std::cout << cbs.use_count() << '\n';
    assert(cbs.use_count() == 1);
}

