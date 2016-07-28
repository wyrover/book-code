#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <cassert>
#include <iostream>
#include <string>

namespace nature
{
class Animal : boost::noncopyable
{
public:
    virtual ~Animal()
    {};

    virtual std::string name() const = 0;
    virtual Animal *clone() const = 0;
};

class SnowLeopard : public Animal
{
public:
    SnowLeopard(const std::string& name) : name_(name) {}

    virtual ~SnowLeopard()
    {
        std::cout << "~SnowLeopard\n";
    }

    std::string name() const override
    {
        return name_ + ", the snow leopard";
    }

    SnowLeopard *clone() const override
    {
        return new SnowLeopard(name_);
    }

private:
    std::string name_;
};

class Puma : public Animal
{
public:
    Puma(const std::string& name) : name_(name) {}
    virtual ~Puma()
    {
        std::cout << "~Puma\n";
    }

    virtual std::string name() const
    {
        return name_ + ", the puma";
    }

    Puma *clone() const override
    {
        return new Puma(name_);
    }

private:
    std::string name_;
};

Animal *new_clone(const Animal& animal)
{
    return animal.clone();
}

bool operator<(const Animal& left, const Animal& right)
{
    return left.name() < right.name();
}


}

int main()
{
    boost::ptr_vector<nature::Animal> mountA;
    boost::ptr_vector<nature::Animal> mountB;
    boost::ptr_list<nature::Animal> mountC;
    mountA.push_back(new nature::Puma("Kaju"));
    mountA.push_back(new nature::SnowLeopard("Rongi"));
    mountA.push_back(new nature::Puma("Juki"));
    mountA.push_back(new nature::SnowLeopard("Turo"));
    size_t num_animals = mountA.size();

    for (auto&animal : mountA) {
        std::cout << "MountA: " << animal.name() << '\n';
    }

    // Move all contents
    mountB = mountA.release();
    assert(mountA.size() == 0);
    assert(mountB.size() == num_animals);
    // move one element
    mountC.transfer(mountC.begin(), mountB.begin() + 1, mountB);
    assert(mountB.size() == num_animals - 1);
    assert(mountC.size() == 1);
    // move one element, second way
    auto popped = mountB.pop_back();
    mountC.push_back(popped.release());
    assert(mountB.size() + mountC.size() == num_animals);
    assert(mountC.size() == 2);
    // move a range of elements
    mountC.transfer(mountC.end(), mountB.begin(),
                    mountB.end(), mountB);
    assert(mountB.size() + mountC.size() == num_animals);
    assert(mountC.size() == num_animals);

    for (auto&animal : mountC) {
        std::cout << "MountC: " << animal.name() << '\n';
    }
}

