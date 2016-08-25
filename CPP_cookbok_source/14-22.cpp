/*
 * Same includes as Example 14-20
 */

using namespace std;
using namespace xercesc;
using namespace xalanc;

/*
 * Define XalanInitializer as in Example 14-20
 */

int main()
{
    try {
        XalanInitializer    init;     // Initialize Xalan.
        XalanTransformer    xslt;     // XSLT engine.
        XSLTResultTarget    html("animals.html"); // xslt's output.
        // Parse source
        XSLTInputSource     xml("animals.xml");
        XalanParsedSource*  parsedXml = 0;

        if (xslt.parseSource(xml, parsedXml) != 0) {
            cout << "xml error: " << xslt.getLastError() << "\n";
        }

        // Compile stylesheet.
        XSLTInputSource           xsl("animals.xsl");
        XalanCompiledStylesheet*  compiledXsl = 0;

        if (xslt.compileStylesheet(xsl, compiledXsl) != 0) {
            cout << "xml error: " << xslt.getLastError() << "\n";
        }

        // Perform transformation.
        if (xslt.transform(xml, xsl, html)) {
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