// mbtest2 - MemBlock test program 2 to access shared memory

#include <iostream.h>
#include <ipc.h>
#include "mbtest.h"

// ****************************************************************************

// print_version - prints a version number

void print_version(int major, int minor)

{
   cout << "block contains version number = "
        << major
        << "."
        << minor
        << endl;
}

// ****************************************************************************

// main routine

int main()

{
   // access an existing shared memory block
   cout << "mbtest2:  accessing shared memory..." << endl;
   ipcSharedMemory mb("myblock");

   // test for error
   if (!mb) {
      cerr << "error # " << mb.rdstate() << endl;
      return 1;
   }

   // get pointer to block and print values
   SharedVariables *p = (SharedVariables *)mb.Pointer();
   if (!p) {
      cerr << "could not get pointer" << endl;
      return 2;
   }
   print_version(p->major_ver, p->minor_ver);

   // change values and wait for user to hit a key
   p->major_ver = 3;
   p->minor_ver = 2;

   cout << endl << "values are now changed:" << endl;
   print_version(p->major_ver, p->minor_ver);

   cout << "press Enter to continue...";
   cin.ignore();

   return 0;
}
