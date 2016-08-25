#include <exception>
#include <iostream>     // cout
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <pathan/XPathNamespace.hpp>
#include <pathan/XPathResult.hpp>
#include <pathan/XPathEvaluator.hpp>
#include <pathan/XPathExpression.hpp>
#include "xerces_strings.hpp"  // Example 14-4

using namespace std;
using namespace xercesc;

/*
 * Define XercesInitializer as in Example 14-8, and
 * CircusErrorHandler and DOMPtr as in Example 14-10
 */

int main()
{
    try {
        // Initialize Xerces and retrieve a DOMImplementation.
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
            createDOMBuilder(
                DOMImplementationLS::MODE_SYNCHRONOUS, 0
            );
        CircusErrorHandler  err;
        parser->setErrorHandler(&err);
        // Parse animals.xml.
        DOMDocument*  doc =
            parser->parseURI("animals.xml");
        DOMElement*   animalList = doc->getDocumentElement();
        // Create XPath expression.
        auto_ptr<XPathEvaluator>
        evaluator(XPathEvaluator::createEvaluator());
        auto_ptr<XPathNSResolver>
        resolver(evaluator->createNSResolver(animalList));
        auto_ptr<XPathExpression>
        xpath(
            evaluator->createExpression(
                fromNative(
                    "animalList/animal[child::name='Herby']"
                ).c_str(),
                resolver.get()
            )
        );
        auto_ptr<XPathEvaluator>   evaluator(XPathEvaluator::createEvaluator());
        auto_ptr<XPathNSResolver>  resolver(evaluator->createNSResolver(animalList));
        auto_ptr<XPathExpression>  xpath(
            evaluator->createExpression(
                fromNative("animalList/animal[child::name='Herby']").c_str(),
                resolver.get()
            ));
        // Evaluate the expression.
        XPathResult* result =
            xpath->evaluate(
                doc,
                XPathResult::ORDERED_NODE_ITERATOR_TYPE,
                0
            );
        DOMNode* herby;

        if (herby = result->iterateNext()) {
            animalList->removeChild(herby);
            herby->release();  // optional.
        }

        // Construct a DOMWriter to save animals.xml.
        DOMPtr<DOMWriter> writer =
            static_cast<DOMImplementationLS*>(impl)->createDOMWriter();
        writer->setErrorHandler(&err);
        // Save animals.xml.
        LocalFileFormatTarget file("circus.xml");
        writer->writeNode(&file, *animalList);
    } catch (const DOMException& e) {
        cout << toNative(e.getMessage()) << "\n";
        return EXIT_FAILURE;
    } catch (const XPathException &e) {
        cout << e.getString() << "\n";
        return EXIT_FAILURE;
    } catch (const exception& e) {
        cout << e.what() << "\n";
        return EXIT_FAILURE;
    }
}