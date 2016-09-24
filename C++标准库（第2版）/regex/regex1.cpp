/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012
 *
 * (C) Copyright Nicolai M. Josuttis 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <regex>
#include <iostream>
using namespace std;

void out(bool b)
{
    cout << (b ? "found" : "not found") << endl;
}

int main()
{
    // find XML/HTML-tagged value (using default syntax):
    regex reg1("<.*>.*</.*>");
    bool found = regex_match("<tag>value</tag>",    // data
                             reg1);                // regular expression
    out(found);
    // find XML/HTML-tagged value (tags before and after the value must match):
    regex reg2("<(.*)>.*</\\1>");
    found = regex_match("<tag>value</tag>",         // data
                        reg2);                     // regular expression
    out(found);
    // find XML/HTML-tagged value (using grep syntax):
    regex reg3("<\\(.*\\)>.*</\\1>", regex_constants::grep);
    found = regex_match("<tag>value</tag>",         // data
                        reg3);                     // regular expression
    out(found);
    // use C-string as regular expression (needs explicit cast to regex):
    found = regex_match("<tag>value</tag>",         // data
                        regex("<(.*)>.*</\\1>"));  // regular expression
    out(found);
    cout << endl;
    // regex_match() versus regex_search():
    found = regex_match("XML tag: <tag>value</tag>",
                        regex("<(.*)>.*</\\1>"));         // fails to match
    out(found);
    found = regex_match("XML tag: <tag>value</tag>",
                        regex(".*<(.*)>.*</\\1>.*"));     // matches
    out(found);
    found = regex_search("XML tag: <tag>value</tag>",
                         regex("<(.*)>.*</\\1>"));        // matches
    out(found);
    found = regex_search("XML tag: <tag>value</tag>",
                         regex(".*<(.*)>.*</\\1>.*"));    // matches
    out(found);
}
