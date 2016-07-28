#include <boost/ptr_container/ptr_map.hpp>
#include <iostream>
#include <string>
// include definitions of Animal, SnowLeopard, Puma

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
    boost::ptr_multimap<std::string, Animal> animals;
    std::string kj = "Puma";
    std::string br = "Snow Leopard";
    animals.insert(kj, new Puma("Kaju"));
    animals.insert(br, new SnowLeopard("Rongi"));
    animals.insert(kj, new Puma("Juki"));

    for (const auto&entry : animals) {
        std::cout << "[" << entry.first << "]->"
                  << entry.second->name() << '\n';
    }
}

