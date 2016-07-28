#include <boost/ptr_container/ptr_set.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <iostream>

class Animal : boost::noncopyable
{
public:
    virtual ~Animal()
    {};

    virtual std::string name() const = 0;
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

private:
    std::string name_;
};

bool operator<(const Animal& left, const Animal& right)
{
    return left.name() < right.name();
}

int main()
{
    boost::ptr_set<Animal>animals;
    animals.insert(new Puma("Kaju"));
    animals.insert(new SnowLeopard("Rongi"));
    animals.insert(new Puma("Juki"));

    for (auto&animal : animals) {
        std::cout << animal.name() << '\n';
    }
}

