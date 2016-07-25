// qtest1 - ipcMessageQueue test program 1

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <ipc.h>

#include "qtest.h"

extern "C" unsigned int sleep(unsigned int);

// ****************************************************************************

int main()

{
   struct QMsg data;

   // print program title
   cout << "qtest1 - Queue Server test program" << endl << endl;

   // create a queue
   cout << "creating a queue..." << endl;
   ipcMessageQueue que("myque");
   if (!que) {
      cerr << "creation error # " << que.rdstate() << endl;
      return 1;
   }

   // peek at queue and try to read
   cout << "there are " << que.Peek() << " items in the queue" << endl;

   cout << "reading from queue with waiting off..." << endl;
   int rc = que.Read( &data, sizeof(struct QMsg) );
   cout << "return code from Read is " << rc << endl << endl;

   // kick off the qtest2 process
   cout << "starting client process which will write to the queue..." << endl;
   unsigned long pid = que.Pid();
   char stmp[256];

#ifdef _AIX
   sprintf(stmp, "aixterm -e /u/rblam/projects/ipc/test/qtest2 %ld |&", pid);
#else
   sprintf(stmp, "start /C qtest2 %ld", pid);
#endif
   system(stmp);

   // wait for item to appear in queue and read it
   cout << "waiting for a queue item to appear..." << endl;
   rc = que.Read(&data, sizeof(struct QMsg), 1);
   cout << "return code from Read is " << rc << endl << endl;

   // output the item that was read
   cout << "data in queue was ["
        << data.cvalue << ", "
        << data.ivalue << ", "
        << data.dvalue << "]" << endl;

   // wait for user to press a key
   cout << "Press a key to continue...";
   cin.ignore();

   // purge the queue
   cout << "there are now " << que.Peek()
        << " items in the queue" << endl;

   cout << "purging queue..." << endl;
   que.Purge();
   cout << "after purging, there are " << que.Peek()
        << " items left in the queue" << endl;

   return 0;
}
