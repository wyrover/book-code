class Person
{
public:
    virtual void eat()   = 0;
    virtual void sleep() = 0;
    virtual void walk()  = 0;
    virtual void jump()  = 0;
};

class IAirborne
{
public:
    virtual void fly()  = 0;
    virtual void up()   = 0;
    virtual void down() = 0;
};

class Superhero : public Person,     // A superhero *is* a person
    public IAirborne   // and flies
{
public:
    virtual void eat();
    virtual void sleep();
    virtual void walk();
    virtual void jump();
    virtual void fly();
    virtual void up();
    virtual void down();
    virtual ~Superhero();
};

void Superhero::fly()
{
    // ...
}

// Implement all of the pure virtuals in Superhero's superclasses...

int main()
{
    Superhero superman;
    superman.walk();  // Superman can walk like a person
    superman.fly();   // or fly like a bird
}