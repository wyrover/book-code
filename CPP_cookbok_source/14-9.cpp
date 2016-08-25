// Implements callbacks which receive character data and
// notifications about the beginnings and ends of elements
class CircusContentHandler : public DefaultHandler
{
public:
    CircusContentHandler(vector<Animal>& animalList)
        : animalList_(animalList),   // list to be populated
          parsingAnimalList_(false), // parsing state
          parsingAnimal_(false),     // parsing state
          parsingAnimalChild_(false) // parsing state
    { }

    // Receives notifications from the parser each time
    // beginning of an element is encountered
    void startElement(
        const XMLCh *const uri,       // Namespace uri
        const XMLCh *const localname, // simple tag name
        const XMLCh *const qname,     // qualified tag name
        const Attributes &attrs)      // Collection of attributes
    {
        static XercesString animalList = fromNative("animalList");
        static XercesString animal = fromNative("animal");
        static XercesString xmlns =
            fromNative("http://www.feldman-family-circus.com");

        // Validate the namespace uri
        if (uri != xmlns)
            throw runtime_error(
                string("wrong namespace uri: ") + toNative(uri)
            );

        // (i) Update the flags parsingAnimalList_, parsingAnimal_,
        //     and parsingAnimalChild_, which indicate where we are
        //     within the document
        // (ii) verify that the elements are correctly
        //      nested;
        // (iii) Delegate most of the work to the method
        //       startAnimalChild( )
        if (!parsingAnimalList_) {
            // We've just encountered the document root
            if (localname == animalList) {
                parsingAnimalList_ = true; // Update parsing state.
            } else {
                // Incorrect nesting
                throw runtime_error(
                    string("expected 'animalList', got ") +
                    toNative(localname)
                );
            }
        } else if (!parsingAnimal_) {
            // We've just encountered a new animal
            if (localname == animal) {
                parsingAnimal_ = true;           // Update parsing state.
                animalList_.push_back(Animal());  // Add an Animal to the list.
            } else {
                // Incorrect nesting
                throw runtime_error(
                    string("expected 'animal', got ") +
                    toNative(localname)
                );
            }
        } else {
            // We're in the middle of parsing an animal element.
            if (parsingAnimalChild_) {
                // Incorrect nesting
                throw runtime_error("bad animal element");
            }

            // Update parsing state.
            parsingAnimalChild_ = true;
            // Let startAnimalChild( ) do the real work
            startAnimalChild(uri, localname, qname, attrs);
        }
    }


    void endElement(
        const XMLCh *const uri,       // Namespace uri
        const XMLCh *const localname, // simple tag name
        const XMLCh *const qname)     // qualified tag name
    {
        static XercesString animalList = fromNative("animal-list");
        static XercesString animal = fromNative("animal");

        // Update the flags parsingAnimalList, parsingAnimal_,
        // and parsingAnimalChild_; delegate most of the work
        // to endAnimalChild( )
        if (localname == animal) {
            parsingAnimal_ = false;
        } else if (localname == animalList) {
            parsingAnimalList_ = false;
        } else {
            endAnimalChild(uri, localname, qname);
            parsingAnimalChild_ = false;
        }
    }

    // Receives notifications when character data is encountered
    void characters(const XMLCh* const chars, const unsigned int length)
    {
        // Append characters to currentText_ for processing by
        // the method endAnimalChild( )
        currentText_.append(chars, length);
    }
private:
    // If the current element represents a veterinarian or trainer,
    // use attrs to construct a Contact object for the current
    // Animal; otherwise, clear currentText_ in preparation for the
    // characters( ) callback
    void startAnimalChild(
        const XMLCh *const uri,       // Namespace uri
        const XMLCh *const localname, // simple tag name
        const XMLCh *const qname,     // qualified tag name
        const Attributes &attrs)      // Collection of attributes
    {
        static XercesString vet = fromNative("veterinarian");
        static XercesString trainer = fromNative("trainer");
        Animal& animal = animalList_.back();

        if (localname == vet) {
            // We've encountered a "veterinarian" element.
            animal.setVeterinarian(contactFromAttributes(attrs));
        } else if (localname == trainer) {
            // We've encountered a "trainer" element.
            animal.setTrainer(contactFromAttributes(attrs));
        } else {
            // We've encountered a "name" , "species", or
            // "dateOfBirth" element. Its content will be supplied
            // by the callback function characters( ).
            currentText_.clear();
        }
    }

    // If the current element represents a name, species, or date
    // of birth, use the text stored in currentText_ to set the
    // appropriate property of the current Animal.
    void endAnimalChild(
        const XMLCh *const uri,       // Namespace uri
        const XMLCh *const localname, // simple tag name
        const XMLCh *const qname)     // qualified tag name
    {
        static XercesString name = fromNative("name");
        static XercesString species = fromNative("species");
        static XercesString dob = fromNative("dateOfBirth");
        // currentText_ contains the content of the element which has
        // just ended. Use it to set the current Animal's properties.
        Animal& animal = animalList_.back();

        if (localname == name) {
            animal.setName(toNative(currentText_));
        } else if (localname == species) {
            animal.setSpecies(toNative(currentText_));
        } else if (localname == dob) {
            animal.setDateOfBirth(toNative(currentText_));
        }
    }

    vector<Animal>&  animalList_;         // list to be populated
    bool             parsingAnimalList_;  // parsing state
    bool             parsingAnimal_;      // parsing state
    bool             parsingAnimalChild_; // parsing state
    XercesString     currentText_;        // character data of the
    // current text node
};