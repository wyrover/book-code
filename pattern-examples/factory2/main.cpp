#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

enum ProductID {AProduct_ID, BProduct_ID};
// Product
class Product
{
public:
    virtual std::string getName() = 0;
    virtual ~Product() {};
    //static Product* CreateProduct(ProductID id);
};

// ConcreteProductA
class ConcreteProductA : public Product
{
public:
    std::string getName()
    {
        return "ConcreteProductA";
    }
    ~ConcreteProductA() {}
};

// ConcreteProductB
class ConcreteProductB : public Product
{
public:
    std::string getName()
    {
        return "ConcreteProductB";
    }
    ~ConcreteProductB() {};
};


//Product* Product::CreateProduct(ProductID id)
//{
//  Product* product;
//  switch(id)
//  {
//  case AProduct_ID:
//      product = new ConcreteProductA();
//      break;
//  case BProduct_ID:
//      product = new ConcreteProductB();
//      break;
//  }
//  return product;
//}


class Creator
{
public:
    virtual Product* factoryMethod() = 0;
};

// ConcreteCreatorA
class ConcreteCreatorA : public Creator
{
public:
    Product* factoryMethod()
    {
        return new ConcreteProductA();
    };
};

// ConcreteCreatorB
class ConcreteCreatorB : public Creator
{
public:
    Product* factoryMethod()
    {
        return new ConcreteProductB();
    }
};

int main()
{
    std::vector<Product*> creators;
    ConcreteCreatorA* productA = new ConcreteCreatorA;
    ConcreteCreatorB* productB = new ConcreteCreatorB;
    creators.push_back(productA->factoryMethod());
    creators.push_back(productB->factoryMethod());
    creators.push_back(productA->factoryMethod());
    creators.push_back(productB->factoryMethod());
    std::for_each(creators.begin(), creators.end(), [](Product * i) {
        std::cout << i->getName()  << std::endl;
    });
    /*std::vector<Creator*> creators;
    creators.push_back(new ConcreteCreatorA());
    creators.push_back(new ConcreteCreatorB());

    std::for_each(creators.begin(), creators.end(), [](Creator* i)
    {
        std::cout <<i->factoryMethod()->getName()  << std::endl;
    });

    creators.clear();*/
    return 0;
}