#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <iostream>

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


} // end of namespace nature

int main()
{
    boost::ptr_vector<nature::Animal> animals, animals2;
    animals.push_back(new nature::Puma("Kaju"));
    animals.push_back(new nature::SnowLeopard("Rongi"));
    animals.push_back(new nature::Puma("Juki"));
    animals2 = animals.clone();

    for (auto&animal : animals2) {
        std::cout << animal.name() << '\n';
    }
}

