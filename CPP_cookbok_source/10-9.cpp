#include <iostream>
#include <fstream>

const static int BUF_SIZE = 4096;

using std::ios_base;

int main(int argc, char** argv)
{
    std::ifstream in(argv[1],
                     ios_base::in | ios_base::binary);  // Use binary mode so we can
    std::ofstream out(argv[2],            // handle all kinds of file
                      ios_base::out | ios_base::binary); // content.
    // Make sure the streams opened okay...
    char buf[BUF_SIZE];

    do {
        in.read(&buf[0], BUF_SIZE);      // Read at most n bytes into
        out.write(&buf[0], in.gcount());  // buf, then write the buf to
    } while (in.gcount() > 0);           // the output.

    // Check streams for problems...
    in.close();
    out.close();
}