#include <exception>
#include <iostream>     // cout
#include <stdexcept>    // runtime_error
#include <cstdlib>      // EXIT_FAILURE
#include <cstring>      // strcmp
#include <vector>
#include <tinyxml.h>
#include "animal.hpp"

using namespace std;

// Extracts the content of an XML element that contains only text
const char* textValue(TiXmlElement* e)
{
    TiXmlNode* first = e->FirstChild();

    if (first != 0 &&
        first == e->LastChild() &&
        first->Type() == TiXmlNode::TEXT) {
        // the element e has a single child, of type TEXT;
        // return the child's
        return first->Value();
    } else {
        throw runtime_error(string("bad ") + e->Value() + " element");
    }
}

// Constructs a Contact from a "veterinarian" or "trainer" element
Contact nodeToContact(TiXmlElement* contact)
{
    using namespace std;
    const char *name, *phone;

    if (contact->FirstChild() == 0 &&
        (name = contact->Attribute("name")) &&
        (phone = contact->Attribute("phone"))) {
        // The element contact is childless and has "name"
        // and "phone" attributes; use these values to
        // construct a Contact
        return Contact(name, phone);
    } else {
        throw runtime_error(string("bad ") + contact->Value() + " element");
    }
}

// Constructs an Animal from an "animal" element
Animal nodeToAnimal(TiXmlElement* animal)
{
    using namespace std;

    // Verify that animal corresponds to an "animal" element
    if (strcmp(animal->Value(), "animal") != 0) {
        throw runtime_error(string("bad animal: ") + animal ->Value());
    }

    Animal result; // Return value
    TiXmlElement* element = animal->FirstChildElement();

    // Read name
    if (element && strcmp(element->Value(), "name") == 0) {
        // The first child element of animal is a "name"
        // element; use its text value to set the name of result
        result.setName(textValue(element));
    } else {
        throw runtime_error("no name attribute");
    }

    // Read species
    element = element->NextSiblingElement();

    if (element && strcmp(element->Value(), "species") == 0) {
        // The second child element of animal is a "species"
        // element; use its text value to set the species of result
        result.setSpecies(textValue(element));
    } else {
        throw runtime_error("no species attribute");
    }

    // Read date of birth
    element = element->NextSiblingElement();

    if (element && strcmp(element->Value(), "dateOfBirth") == 0) {
        // The third child element of animal is a "dateOfBirth"
        // element; use its text value to set the date of birth
        // of result
        result.setDateOfBirth(textValue(element));
    } else {
        throw runtime_error("no dateOfBirth attribute");
    }

    // Read veterinarian
    element = element->NextSiblingElement();

    if (strcmp(element->Value(), "veterinarian") == 0) {
        // The fourth child element of animal is a "veterinarian"
        // element; use it to construct a Contact object and
        // set result's veterinarian
        result.setVeterinarian(nodeToContact(element));
    } else {
        throw runtime_error("no veterinarian attribute");
    }

    // Read trainer
    element = element->NextSiblingElement();

    if (strcmp(element->Value(), "trainer") == 0) {
        // The fifth child element of animal is a "trainer"
        // element; use it to construct a Contact object and
        // set result's trainer
        result.setTrainer(nodeToContact(element));
    } else {
        throw runtime_error("no trainer attribute");
    }

    // Check that there are no more children
    element = element->NextSiblingElement();

    if (element != 0) {
        throw runtime_error(
            string("unexpected element:") +
            element->Value()
        );
    }

    return result;
}

int main()
{
    using namespace std;

    try {
        vector<Animal> animalList;
        // Parse "animals.xml"
        TiXmlDocument doc("animals.xml");

        if (!doc.LoadFile())
            throw runtime_error("bad parse");

        // Verify that root is an animal-list
        TiXmlElement* root = doc.RootElement();

        if (strcmp(root->Value(), "animalList") != 0) {
            throw runtime_error(string("bad root: ") + root->Value());
        }

        // Traverse children of root, populating the list
        // of animals
        for (TiXmlElement* animal = root->FirstChildElement();
             animal;
             animal = animal->NextSiblingElement()) {
            animalList.push_back(nodeToAnimal(animal));
        }

        // Print the animals' names
        for (vector<Animal>::size_type i = 0,
             n = animalList.size();
             i < n;
             ++i) {
            cout << animalList[i] << "\n";
        }
    } catch (const exception& e) {
        cout << e.what() << "\n";
        return EXIT_FAILURE;
    }
}