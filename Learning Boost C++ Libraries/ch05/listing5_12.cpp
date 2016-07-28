#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>
#include <iostream>
#include <boost/current_function.hpp>

class AbstractJob
{
public:
    virtual ~AbstractJob() {}

    void doJob()
    {
        doStep1();
        doStep2();
    }

private:
    virtual void doStep1() = 0;
    virtual void doStep2() = 0;
};

class JobA : public AbstractJob
{
    void doStep1() override
    {
        std::cout << BOOST_CURRENT_FUNCTION << '\n';
    }

    void doStep2() override
    {
        std::cout << BOOST_CURRENT_FUNCTION << '\n';
    }
};

class JobB : public AbstractJob
{
    void doStep1() override
    {
        std::cout << BOOST_CURRENT_FUNCTION << '\n';
    }

    void doStep2() override
    {
        std::cout << BOOST_CURRENT_FUNCTION << '\n';
    }
};

int main()
{
    boost::ptr_vector<AbstractJob> basePtrVec;
    basePtrVec.push_back(new JobA);
    basePtrVec.push_back(new JobB);
    AbstractJob& firstJob = basePtrVec.front();
    AbstractJob& lastJob = basePtrVec.back();

    for (auto& job : basePtrVec) {
        job.doJob();
    }
}

