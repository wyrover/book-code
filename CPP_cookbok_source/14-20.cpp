#include <exception>
#include <iostream>     // cout
#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XSLT/XSLTResultTarget.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include "xerces_strings.hpp"  // Example 14-4

using namespace std;
using namespace xercesc;
using namespace xalanc;

// RAII utility that initializes the parser and frees resources
// when it goes out of scope
struct XalanInitializer {
    XalanInitializer()
    {
        XMLPlatformUtils::Initialize();
        XalanTransformer::initialize();
    }
    ~XalanInitializer()
    {
        XalanTransformer::terminate();
        XMLPlatformUtils::Terminate();
    }
};

int main()
{
    try {
        XalanInitializer  init;                 // Initialize Xalan.
        XalanTransformer  xslt;                 // XSLT engine.
        XSLTInputSource   xml("animals.xml");   // XML document from
        // Example 14-1.
        XSLTInputSource   xsl("animals.xsl");   // Stylesheet from
        // Example 14-19.
        XSLTResultTarget  html("animals.html"); // xslt's output.

        // Perform transformation.
        if (xslt.transform(xml, xsl, html) != 0) {
            cout << "xml error: " << xslt.getLastError() << "\n";
        }
    } catch (const XMLException& e) {
        cout << "xml error: " << toNative(e.getMessage()) << "\n";
        return EXIT_FAILURE;
    } catch (const exception& e) {
        cout << e.what() << "\n";
        return EXIT_FAILURE;
    }
}