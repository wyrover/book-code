// semtest2 - Semaphore test program 2

#include <iostream.h>
#include <ipc.h>

// ****************************************************************************

int main()

{
   // print program title
   cout << "semtest2 - Semaphore test program" << endl << endl;

   // access an event semaphore
   cout << "accessing an event semaphore..." << endl;
   ipcEventSemaphore event_sem("evtsem", semaccess);
   if (!event_sem) {
      cerr << "Error!  state = " << event_sem.rdstate() << endl;
      return 1;
   }

   // show the name, type and id for the semaphore
   cout << "event semaphore name is " << event_sem.Name() << endl
        << "type is " << (int)event_sem.Type() << endl
        << "and id is " << event_sem.ID() << endl << endl;

   // create a mutex semaphore
   cout << "creating a mutex semaphore..." << endl;
   ipcMutexSemaphore mutex_sem("mtxsem", semcreate);
   if (!mutex_sem) {
      cerr << "Error!  state = " << mutex_sem.rdstate() << endl;
      return 2;
   }

   // show the name, type and id for the semaphore
   cout << "mutex semaphore name is " << mutex_sem.Name() << endl
        << "type is " << (int)mutex_sem.Type() << endl
        << "and id is " << mutex_sem.ID() << endl << endl;

   // request the mutex semaphore
   cout << "requesting the mutex semaphore..." << endl;
   mutex_sem.Request();

   // post the event semaphore
   cout << "press any key to post the event semaphore...";
   cin.ignore();
   cout << "posting the event semaphore..." << endl;
   event_sem.Post();

   // wait for a key to release the mutex semaphore
   cout << "press any key to release the mutex semaphore...";
   cin.ignore();
   cout << "releasing the mutex semphore..." << endl;
   mutex_sem.Release();

   return 0;
}
