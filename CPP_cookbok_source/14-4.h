#ifndef XERCES_STRINGS_HPP_INCLUDED
#define XERCES_STRINGS_HPP_INCLUDED

#include <string>
#include <boost/scoped_array.hpp>
#include <xercesc/util/XMLString.hpp>

typedef std::basic_string<XMLCh> XercesString;

// Converts from a narrow-character string to a wide-character string.
inline XercesString fromNative(const char* str)
{
    boost::scoped_array<XMLCh> ptr(xercesc::XMLString::transcode(str));
    return XercesString(ptr.get());
}

// Converts from a narrow-character string to a wide-charactr string.
inline XercesString fromNative(const std::string& str)
{
    return fromNative(str.c_str());
}

// Converts from a wide-character string to a narrow-character string.
inline std::string toNative(const XMLCh* str)
{
    boost::scoped_array<char> ptr(xercesc::XMLString::transcode(str));
    return std::string(ptr.get());
}

// Converts from a wide-character string to a narrow-character string.
inline std::string toNative(const XercesString& str)
{
    return toNative(str.c_str());
}

#endif // #ifndef XERCES_STRINGS_HPP_INCLUDED