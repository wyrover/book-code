#include <iostream>
#include <boost/format.hpp>

using namespace std;
using boost::format;
using boost::io::str;
using boost::io::format_error;

int main()
{
    try {
        format f("There are %1% ways %2% %3% %4%");
        f % 3;
        f % "to" % "do" % "this.";
        cout << f << endl;
        f.clear();  // Clear buffers to format something else
        f.parse("Those cost $%d.");
        f % 50;
        cout << f << endl;
        int x = 11256099;
        string strx = str(format("%x") % x);
        cout << strx << endl;
    } catch (format_error &e) {
        cout << e.what() << endl;
    }
}
