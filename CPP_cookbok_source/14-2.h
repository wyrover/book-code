#ifndef ANIMALS_HPP_INCLUDED
#define ANIMALS_HPP_INCLUDED

#include <ostream>
#include <string>
#include <stdexcept> // runtime_error
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/regex.hpp>

// Represents a veterinarian or trainer
class Contact
{
public:
    Contact() { }
    Contact(const std::string& name, const std::string& phone)
        : name_(name)
    {
        setPhone(phone);
    }
    std::string name() const
    {
        return name_;
    }
    std::string phone() const
    {
        return phone_;
    }
    void setName(const std::string& name)
    {
        name_ = name;
    }
    void setPhone(const std::string& phone)
    {
        using namespace std;
        using namespace boost;
        // Use Boost.Regex to verify that phone
        // has the form (ddd)ddd-dddd
        static regex pattern("\\([0-9]{3}\\)[0-9]{3}-[0-9]{4}");

        if (!regex_match(phone, pattern)) {
            throw runtime_error(string("bad phone number:") + phone);
        }

        phone_ = phone;
    }
private:
    std::string name_;
    std::string phone_;
};

// Compare two Contacts for equality; used in Recipe 14.9
// (for completeness, you should also define operator!=)
bool operator==(const Contact& lhs, const Contact& rhs)
{
    return lhs.name() == rhs.name() && lhs.phone() == rhs.phone();
}

// Writes a Contact to an ostream
std::ostream& operator<<(std::ostream& out, const Contact& contact)
{
    out << contact.name() << " " << contact.phone();
    return out;
}

// Represents an animal
class Animal
{
public:
    // Default constructs an Animal; this is
    // the constructor you'll use most
    Animal() { }

    // Constructs an Animal with the given properties;
    // you'll use this constructor in Recipe 14.9
    Animal(const std::string& name,
           const std::string& species,
           const std::string& dob,
           const Contact& vet,
           const Contact& trainer)
        : name_(name),
          species_(species),
          vet_(vet),
          trainer_(trainer)
    {
        setDateOfBirth(dob);
    }

    // Getters
    std::string             name() const
    {
        return name_;
    }
    std::string             species() const
    {
        return species_;
    }
    boost::gregorian::date  dateOfBirth() const
    {
        return dob_;
    }
    Contact                 veterinarian() const
    {
        return vet_;
    }
    Contact                 trainer() const
    {
        return trainer_;
    }

    // Setters
    void setName(const std::string& name)
    {
        name_ = name;
    }
    void setSpecies(const std::string& species)
    {
        species_ = species;
    }
    void setDateOfBirth(const std::string& dob)
    {
        dob_ = boost::gregorian::from_string(dob);
    }
    void setVeterinarian(const Contact& vet)
    {
        vet_ = vet;
    }
    void setTrainer(const Contact& trainer)
    {
        trainer_ = trainer;
    }
private:
    std::string             name_;
    std::string             species_;
    boost::gregorian::date  dob_;
    Contact                 vet_;
    Contact                 trainer_;
};

// Compare two Animals for equality; used in Recipe 14.9
// (for completeness, you should also define operator!=)
bool operator==(const Animal& lhs, const Animal& rhs)
{
    return lhs.name() == rhs.name() &&
           lhs.species() == rhs.species() &&
           lhs.dateOfBirth() == rhs.dateOfBirth() &&
           lhs.veterinarian() == rhs.veterinarian() &&
           lhs.trainer() == rhs.trainer();
}

// Writes an Animal to an ostream
std::ostream& operator<<(std::ostream& out, const Animal& animal)
{
    out << "Animal {\n"
        << "  name=" << animal.name() << ";\n"
        << "  species=" << animal.species() << ";\n"
        << "  date-of-birth=" << animal.dateOfBirth() << ";\n"
        << "  veterinarian=" << animal.veterinarian() << ";\n"
        << "  trainer=" << animal.trainer() << ";\n"
        << "}";
    return out;
}

#endif // #ifndef ANIMALS_HPP_INCLUDED