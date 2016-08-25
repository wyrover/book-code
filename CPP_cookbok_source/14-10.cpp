#include <exception>
#include <iostream>     // cout
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include "animal.hpp"
#include "xerces_strings.hpp"

using namespace std;
using namespace xercesc;

/*
 * Define XercesInitializer as in Example 14-8
 */

// RAII utility that releases a resource when it goes out of scope.
template<typename T>
class DOMPtr
{
public:
    DOMPtr(T* t) : t_(t) { }
    ~DOMPtr()
    {
        t_->release();
    }
    T* operator->() const
    {
        return t_;
    }
private:
    // prohibit copying and assigning
    DOMPtr(const DOMPtr&);
    DOMPtr& operator=(const DOMPtr&);
    T* t_;
};

// Reports errors encountered while parsing using a DOMBuilder.
class CircusErrorHandler : public DOMErrorHandler
{
public:
    bool handleError(const DOMError& e)
    {
        std::cout << toNative(e.getMessage()) << "\n";
        return false;
    }
};

// Returns the value of the "name" child of an "animal" element.
const XMLCh* getAnimalName(const DOMElement* animal)
{
    static XercesString name = fromNative("name");
    // Iterate though animal's children
    DOMNodeList* children = animal->getChildNodes();

    for (size_t i = 0,
         len = children->getLength();
         i < len;
         ++i) {
        DOMNode* child = children->item(i);

        if (child->getNodeType() == DOMNode::ELEMENT_NODE &&
            static_cast<DOMElement*>(child)->getTagName() == name) {
            // We've found the "name" element.
            return child->getTextContent();
        }
    }

    return 0;
}

int main()
{
    try {
        // Initialize Xerces and retrieve a DOMImplementation;
        // specify that you want to  use the Load and Save (LS)
        // feature
        XercesInitializer   init;
        DOMImplementation*  impl =
            DOMImplementationRegistry::getDOMImplementation(
                fromNative("LS").c_str()
            );

        if (impl == 0) {
            cout << "couldn't create DOM implementation\n";
            return EXIT_FAILURE;
        }

        // Construct a DOMBuilder to parse animals.xml.
        DOMPtr<DOMBuilder>  parser =
            static_cast<DOMImplementationLS*>(impl)->
            createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
        // Enable namespaces (not needed in this example)
        parser->setFeature(XMLUni::fgDOMNamespaces, true);
        // Register an error handler
        CircusErrorHandler  err;
        parser->setErrorHandler(&err);
        // Parse animals.xml; you can use a URL here
        // instead of a file name
        DOMDocument* doc =
            parser->parseURI("animals.xml");
        // Search for Herby the elephant: first, obtain a pointer
        // to the "animalList" element.
        DOMElement*  animalList = doc->getDocumentElement();

        if (animalList->getTagName() != fromNative("animalList")) {
            cout << "bad document root: "
                 << toNative(animalList->getTagName())
                 << "\n";
            return EXIT_FAILURE;
        }

        // Next, iterate through the "animal" elements, searching
        // for Herby the elephant.
        DOMNodeList* animals =
            animalList->getElementsByTagName(fromNative("animal").c_str());

        for (size_t i = 0,
             len = animals->getLength();
             i < len;
             ++i) {
            DOMElement* animal =
                static_cast<DOMElement*>(animals->item(i));
            const XMLCh* name = getAnimalName(animal);

            if (name != 0 && name == fromNative("Herby")) {
                // Found Herby -- remove him from document.
                animalList->removeChild(animal);
                animal->release();  // optional.
                break;
            }
        }

        // Construct a DOMWriter to save animals.xml.
        DOMPtr<DOMWriter> writer =
            static_cast<DOMImplementationLS*>(impl)->createDOMWriter();
        writer->setErrorHandler(&err);
        // Save animals.xml.
        LocalFileFormatTarget file("animals.xml");
        writer->writeNode(&file, *animalList);
    } catch (const SAXException& e) {
        cout << "xml error: " << toNative(e.getMessage()) << "\n";
        return EXIT_FAILURE;
    } catch (const DOMException& e) {
        cout << "xml error: " << toNative(e.getMessage()) << "\n";
        return EXIT_FAILURE;
    } catch (const exception& e) {
        cout << e.what() << "\n";
        return EXIT_FAILURE;
    }
}