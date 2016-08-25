#include <cstddef>      // size_t
#include <exception>
#include <iostream>     // cout
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/DOMSupport/XalanDocumentPrefixResolver.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XPathEvaluator.hpp>
#include "animal.hpp"
#include "xerces_strings.hpp"

using namespace std;
using namespace xercesc;
using namespace xalanc;

// RAII utility that initializes the parser and the XPath engine
// and frees resources when it goes out of scope
class XPathInitializer
{
public:
    XPathInitializer()
    {
        XMLPlatformUtils::Initialize();
        XPathEvaluator::initialize();
    }
    ~XPathInitializer()
    {
        XPathEvaluator::terminate();
        XMLPlatformUtils::Terminate();
    }
private:
    // Prohibit copying and assignment
    XPathInitializer(const XPathInitializer&);
    XPathInitializer& operator=(const XPathInitializer&);
};

// Receives Error notifications
class CircusErrorHandler : public DefaultHandler
{
public:
    void error(const SAXParseException& e)
    {
        throw runtime_error(toNative(e.getMessage()));
    }
    void fatalError(const SAXParseException& e)
    {
        error(e);
    }
};

int main()
{
    try {
        // Initialize Xerces and XPath and construct a DOM parser.
        XPathInitializer    init;
        XercesDOMParser     parser;
        // Register error handler
        CircusErrorHandler error;
        parser.setErrorHandler(&error);
        // Parse animals.xml.
        parser.parse(fromNative("animals.xml").c_str());
        DOMDocument* doc = parser.getDocument();
        DOMElement*  animalList = doc->getDocumentElement();
        // Create a XalanDocument based on doc.
        XercesDOMSupport              support;
        XercesParserLiaison           liaison(support);
        XercesDOMWrapperParsedSource  src(doc, liaison, support);
        XalanDocument*                xalanDoc = src.getDocument();
        // Evaluate an XPath expression to obtain a list
        // of text nodes containing animals' names
        XPathEvaluator                evaluator;
        XalanDocumentPrefixResolver   resolver(xalanDoc);
        XercesString                  xpath =
            fromNative("animalList/animal/name/child::text( )");
        XObjectPtr                    result =
            evaluator.evaluate(
                support,        // DOMSupport
                xalanDoc,       // context node
                xpath.c_str(),   // XPath expr
                resolver);      // Namespace resolver
        const NodeRefListBase&        nodeset = result->nodeset();

        // Iterate through the node list, printing the animals' names
        for (size_t i = 0,
             len = nodeset.getLength();
             i < len;
             ++i) {
            const XMLCh* name =
                nodeset.item(i)->getNodeValue().c_str();
            std::cout << toNative(name) << "\n";
        }
    } catch (const DOMException& e) {
        cout << "xml error: " << toNative(e.getMessage()) << "\n";
        return EXIT_FAILURE;
    } catch (const exception& e) {
        cout << e.what() << "\n";
        return EXIT_FAILURE;
    }
}