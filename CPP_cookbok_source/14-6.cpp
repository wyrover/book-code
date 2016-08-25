#include <stdexcept>                       // runtime_error
#include <vector>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp> // Contains no-op 
// implementations of
// the various handlers
#include "xerces_strings.hpp"              // Example 14-4
#include "animal.hpp"

using namespace std;
using namespace xercesc;

// Returns an instance of Contact based
// on the given collection of attributes
Contact contactFromAttributes(const Attributes &attrs)
{
    // For efficiency, store frequently used string
    // in static variables
    static XercesString name = fromNative("name");
    static XercesString phone = fromNative("phone");
    Contact result;    // Contact to be returned.
    const XMLCh* val;  // Value of name or phone attribute.

    // Set Contact's name.
    if ((val = attrs.getValue(name.c_str())) != 0) {
        result.setName(toNative(val));
    } else {
        throw runtime_error("contact missing name attribute");
    }

    // Set Contact's phone number.
    if ((val = attrs.getValue(phone.c_str())) != 0) {
        result.setPhone(toNative(val));
    } else {
        throw runtime_error("contact missing phone attribute");
    }

    return result;
}

// Implements callbacks that receive character data and
// notifications about the beginnings and ends of elements
class CircusContentHandler : public DefaultHandler
{
public:
    CircusContentHandler(vector<Animal>& animalList)
        : animalList_(animalList)
    { }

    // If the current element represents a veterinarian or trainer,
    // use attrs to construct a Contact object for the current
    // Animal; otherwise, clear currentText_ in preparation for the
    // characters( ) callback
    void startElement(
        const XMLCh *const uri,        // namespace URI
        const XMLCh *const localname,  // tagname w/ out NS prefix
        const XMLCh *const qname,      // tagname + NS pefix
        const Attributes &attrs)       // elements's attributes
    {
        static XercesString animalList = fromNative("animalList");
        static XercesString animal = fromNative("animal");
        static XercesString vet = fromNative("veterinarian");
        static XercesString trainer = fromNative("trainer");
        static XercesString xmlns =
            fromNative("http://www.feldman-family-circus.com");

        // Check namespace URI
        if (uri != xmlns)
            throw runtime_error(
                string("wrong namespace uri: ") + toNative(uri)
            );

        if (localname == animal) {
            // Add an Animal to the list; this is the new
            // "current Animal"
            animalList_.push_back(Animal());
        } else if (localname != animalList) {
            Animal& animal = animalList_.back();

            if (localname == vet) {
                // We've encountered a "veterinarian" element.
                animal.setVeterinarian(contactFromAttributes(attrs));
            } else if (localname == trainer) {
                // We 've encountered a "trainer" element.
                animal.setTrainer(contactFromAttributes(attrs));
            } else {
                // We've encountered a "name" , "species", or
                // "dateOfBirth" element. Its content will be supplied
                // by the callback function characters( ).
                currentText_.clear();
            }
        }
    }

    // If the current element represents a name, species, or date
    // of birth, use the text stored in currentText_ to set the
    // appropriate property of the current Animal.
    void endElement(
        const XMLCh *const uri,        // namespace URI
        const XMLCh *const localname,  // tagname w/ out NS prefix
        const XMLCh *const qname)      // tagname + NS pefix
    {
        static XercesString animalList = fromNative("animal-list");
        static XercesString animal = fromNative("animal");
        static XercesString name = fromNative("name");
        static XercesString species = fromNative("species");
        static XercesString dob = fromNative("dateOfBirth");

        if (localname != animal && localname != animalList) {
            // currentText_ contains the content of the element
            // which has ended. Use it to set the current Animal's
            // properties.
            Animal& animal = animalList_.back();

            if (localname == name) {
                animal.setName(toNative(currentText_));
            } else if (localname == species) {
                animal.setSpecies(toNative(currentText_));
            } else if (localname == dob) {
                animal.setDateOfBirth(toNative(currentText_));
            }
        }
    }
    // Receives notifications when character data is encountered
    void characters(const XMLCh* const chars,
                    const unsigned int length)
    {
        // Append characters to currentText_ for processing by
        // the method endElement( )
        currentText_.append(chars, length);
    }
private:
    vector<Animal>&  animalList_;
    XercesString     currentText_;
};