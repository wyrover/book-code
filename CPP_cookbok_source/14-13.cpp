/*
 * Same includes as Example14-8, except <vector> is not needed
 */

#include <stdexcept> // runtime_error
#include <xercesc/sax2/DefaultHandler.hpp>

using namespace std;
using namespace xercesc;

/*
 * Define XercesInitializer as in Example 14-8
 * and CircusErrorHandler as in Example 14-7
 */

int main()
{
    try {
        // Initialize Xerces and obtain a SAX2 parser
        XercesInitializer init;
        auto_ptr<SAX2XMLReader>
        parser(XMLReaderFactory::createXMLReader());
        // Enable validation
        parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
        // Register error handler to receive notifications
        // of DTD violations
        CircusErrorHandler error;
        parser->setErrorHandler(&error);
        parser->parse("animals.xml");
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