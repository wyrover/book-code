// qtest2 - ipcMessageQueue test program 2

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <ipc.h>

#include "qtest.h"

// ****************************************************************************

int main(int argc, char *argv[])

{
   struct QMsg data;

   // print program title
   cout << "qtest2 - Queue client test program" << endl << endl;

   // check command line arguments
   if (argc != 2) {
      cout << "the PID of the queue owner must be on the command line" << endl;
      return 1;
   }
   unsigned long pid = atol(argv[1]);   // process id of queue owner

   // access an existing queue
   cout << "accessing a queue..." << endl;
   ipcMessageQueue que("myque", pid);
   if (!que) {
      cerr << "access error # " << que.rdstate() << endl;
      return 2;
   }

   // set up data to write
   data.cvalue = 'P';
   data.dvalue = 3.14159;

   // write 10 items to the queue
   for (int i = 0; i < 10; i++) {
      data.ivalue = i;
      cout << "writing ["
           << data.cvalue << ", "
           << data.ivalue << ", "
           << data.dvalue << "] to queue..." << endl;

      int rc = que.Write( &data, sizeof(struct QMsg) );
      cout << "return code from Write() is " << rc << endl;
   }
   cout << "done...press any key to exit" << endl;
   cin.ignore();

   return 0;
}
