#include <exception>
#include <iostream>     // cout
#include <memory>       // auto_ptr
#include <vector>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include "animal.hpp"
#include "xerces_strings.hpp"  // Example 14-4

using namespace std;
using namespace xercesc;

// RAII utility that initializes the parser and frees resources
// when it goes out of scope
class XercesInitializer
{
public:
    XercesInitializer()
    {
        XMLPlatformUtils::Initialize();
    }
    ~XercesInitializer()
    {
        XMLPlatformUtils::Terminate();
    }
private:
    // Prohibit copying and assignment
    XercesInitializer(const XercesInitializer&);
    XercesInitializer& operator=(const XercesInitializer&);
};

int main()
{
    try {
        vector<Animal> animalList;
        // Initialze Xerces and obtain parser
        XercesInitializer        init;
        auto_ptr<SAX2XMLReader>  ]
        parser(XMLReaderFactory::createXMLReader());
        // Register handlers
        CircusContentHandler     content(animalList);
        CircusErrorHandler       error;
        parser->setContentHandler(&content);
        parser->setErrorHandler(&error);
        // Parse the XML document
        parser->parse("animals.xml");

        // Print animals' names
        for (vector<Animal>::size_type i = 0,
                                       n = animalList.size();
        i < n;
        ++i) {
            cout << animalList[i] << "\n";
        }
    } catch (const SAXException& e) {
        cout << "xml error: " << toNative(e.getMessage()) << "\n";
        return EXIT_FAILURE;
    } catch (const XMLException& e) {
        cout << "xml error: " << toNative(e.getMessage()) << "\n";
        return EXIT_FAILURE;
    } catch (const exception& e) {
        cout << e.what() << "\n";
        return EXIT_FAILURE;
    }
}