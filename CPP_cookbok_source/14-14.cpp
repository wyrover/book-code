#include <exception>
#include <iostream>     // cout
#include <stdexcept>    // runtime_error
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include "xerces_strings.hpp"  // Example 14-4

using namespace std;
using namespace xercesc;

/*
 * Define XercesInitializer as in Example 14-8
 * and CircusErrorHandler as in Example 14-7
 */

int main()
{
    try {
        // Initialize Xerces and construct a DOM parser.
        XercesInitializer        init;
        XercesDOMParser          parser;
        // Enable DTD validation
        parser.setValidationScheme(XercesDOMParser::Val_Always);
        // Register an error handler to receive notifications
        // of schema violations
        CircusErrorHandler       handler;
        parser.setErrorHandler(&handler);
        // Parse and validate.
        parser.parse("animals.xml");
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