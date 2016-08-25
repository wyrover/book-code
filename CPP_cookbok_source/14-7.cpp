#include <stdexcept> // runtime_error
#include <xercesc/sax2/DefaultHandler.hpp>

// Receives Error notifications.
class CircusErrorHandler : public DefaultHandler
{
public:
    void warning(const SAXParseException& e)
    {
        /* do nothing */
    }
    void error(const SAXParseException& e)
    {
        throw runtime_error(toNative(e.getMessage()));
    }
    void fatalError(const SAXParseException& e)
    {
        error(e);
    }
};