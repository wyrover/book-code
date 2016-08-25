...
// Include serialization support for boost::gregorian::date
#include <boost/date_time/gregorian/greg_serialize.hpp>
...
class Contact
{
    ...
private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        // Write (or read) each data-member as a name-value pair
        using boost::serialization::make_nvp;
        ar & make_nvp("name", name_);
        ar & make_nvp("species", species_);
        ar & make_nvp("dateOfBirth", dob_);
        ar & make_nvp("veterinarian", vet_);
        ar & make_nvp("trainer", trainer_);
    }
    ...
};