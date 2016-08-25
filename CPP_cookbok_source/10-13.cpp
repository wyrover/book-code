#include <iostream>
#include <cstdio>

int main()
{
    FILE* pf = NULL;
    char buf[256];
    pf = tmpfile();   // Create and open a temp file

    if (pf) {
        fputs("This is a temp file", pf);  // Write some data to it
    }

    fseek(pf, 5, SEEK_SET);  // Reset the file position
    fgets(buf, 255, pf);     // Read a string from it
    fclose(pf);
    std::cout << buf << '\n';
}