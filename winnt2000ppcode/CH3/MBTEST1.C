// mbtest1 - test program 1 to allocate shared memory

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
   // allocate a shared memory block
   cout << "mbtest1:  allocating shared memory..." << endl;
   ipcSharedMemory mb( "myblock", sizeof(SharedVariables) );

   // test for error
   if (!mb) {
      cerr << "error # " << mb.rdstate() << endl;
      return 1;
   }

   // get pointer to block and fill in structure
   SharedVariables *p = (SharedVariables *)mb.Pointer();
   if (!p) {
      cerr << "could not get pointer" << endl;
      return 2;
   }

   // put version number into the shared memory
   p->major_ver = 2;
   p->minor_ver = 1;
   print_version(p->major_ver, p->minor_ver);

   // wait for user to hit a key and print version again
   cout << "press Enter to continue...";
   cin.ignore();
   print_version(p->major_ver, p->minor_ver);

   return 0;
}
