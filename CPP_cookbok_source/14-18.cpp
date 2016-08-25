/*
 * Same includes as in Example 14-14
 */

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
        // Enable validation
        parser.setValidationScheme(XercesDOMParser::Val_Always);
        parser.setDoSchema(true);
        parser.setDoNamespaces(true);
        parser.setExternalNoNamespaceSchemaLocation(
            fromNative("animals.xsd").c_str()
        );
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