//////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlstr.h>
#include <map>

namespace cactus
{

//////////////////////////////////////////////////////////////////////////

typedef CString CCmdLineParser_String ;


using std::map;

//////////////////////////////////////////////////////////////////////////

// Example of strings
// /Key1 /Key2 -Key3:Val3 -Key4:"Val 4-with/spaces/and-delimeters" /Key5:Val5
// /Key:"" is equal to /Key: and is equal to /Key
// /Key is equal to -Key
// If getCaseSensitive is false (by default), all keys are made lowercase.

// Examples of use:
// CCmdLineParser parser(_T("/Key1 /Key2: -Key3:Val3 -Key4:\"Val 4-with/spaces/and-delimeters\" /Key5:Val5"));
// ASSERT(parser.HasKey("Key1") == true);
// ASSERT(parser.HasKey("Key10") == false);
// ASSERT(parser.HasVal("Key2") == false);
// parser.GetVal("Key1") -> []; (empty string)
// parser.GetVal("Key2") -> []; (empty string)
// parser.GetVal("Key3") -> [Val3];
// parser.GetVal("Key4") -> [Val 4-with/spaces/and-delimeters];
// CCmdLineParser::POSITION pos = parser.getFirst();
// CString sKey, sVal;
// while(!parser.isLast(pos)) {
//      parser.getNext(pos, sKey, sVal);
//      printf("Key: [%s], Val: [%s]");
// }

//////////////////////////////////////////////////////////////////////////

class CCmdLineParser
{
public:
    class CValsMap : public map<CCmdLineParser_String, CCmdLineParser_String> {};
    typedef CValsMap::const_iterator POSITION;

public:
    CCmdLineParser(
        LPCTSTR sCmdLine = NULL,
        bool bCaseSensitive = false);

    virtual ~CCmdLineParser();

    bool Parse(
        LPCTSTR sCmdLine);

    LPCTSTR getCmdLine() const
    {
        return m_sCmdLine;
    }

    void setCaseSensitive(
        bool bSensitive)
    {
        m_bCaseSensitive = bSensitive;
    }

    bool getCaseSensitive() const
    {
        return m_bCaseSensitive;
    }

    const CValsMap &getVals() const
    {
        return m_ValsMap;
    }

    // Start iterating through keys and values
    POSITION getFirst() const;

    // Get next key-value pair, returns empty sKey if end reached
    POSITION getNext(
        POSITION& pos,
        CCmdLineParser_String &sKey,
        CCmdLineParser_String &sValue) const;

    // just helper ;)
    bool isLast(
        POSITION &pos) const;

    // TRUE if "Key" present in command line
    bool HasKey(
        LPCTSTR sKey) const;

    // Is "key" present in command line and have some value
    bool HasVal(
        LPCTSTR sKey) const;

    // Returns value if value was found or NULL otherwise
    LPCTSTR GetVal(
        LPCTSTR sKey) const;

    // Returns true if value was found
    bool GetVal(
        LPCTSTR sKey,
        CCmdLineParser_String &sValue) const;

private:
    CValsMap::const_iterator findKey(
        LPCTSTR sKey) const;

private:
    CCmdLineParser_String m_sCmdLine;
    CValsMap m_ValsMap;
    bool m_bCaseSensitive;

    static const TCHAR m_sDelimeters[];
    static const TCHAR m_sValueSep[];
    static const TCHAR m_sQuotes[];
};

} // namespace cactus

//////////////////////////////////////////////////////////////////////////