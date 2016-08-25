#include <boost/serialization/nvp.hpp> // "name-value pair"

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
        ar & make_nvp("phone", phone_);
    }
    ...
};