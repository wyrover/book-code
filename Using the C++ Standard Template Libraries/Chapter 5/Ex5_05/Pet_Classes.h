// Pet_Classes.h for Ex5_05
// Classes that define pets
#ifndef PET_CLASSES_H
#define PET_CLASSES_H
#include <string>                                // For string class
#include <cstring>                               // For strcmp()
using std::string;

class Pet
{
protected:
    string name {};

public:
    virtual ~Pet() {}                              // Virtual destructor for base class
    const string& get_name() const
    {
        return name;
    }

    virtual bool operator<(const Pet& pet) const
    {
        auto result = std::strcmp(typeid(*this).name(), typeid(pet).name());
        return (result < 0) || ((result == 0) && (name < pet.name));
    }
    friend std::ostream& operator<<(std::ostream& out, const Pet& pet);
};

class Cat : public Pet
{
public:
    Cat() = default;
    Cat(const string& cat_name)
    {
        name = cat_name;
    }
};

class Dog : public Pet
{
public:
    Dog() = default;
    Dog(const string& dog_name)
    {
        name = dog_name;
    }
};

class Mouse : public Pet
{
public:
    Mouse() = default;
    Mouse(const string& mouse_name)
    {
        name = mouse_name;
    }
};

inline std::ostream& operator<<(std::ostream& out, const Pet& pet)
{
    return out << "A " <<
           string {typeid(pet).name()} .erase(0, 6) << " called " << pet.name;
}
#endif