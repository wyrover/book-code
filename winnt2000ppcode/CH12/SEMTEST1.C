// semtest1 - Semaphore test program 1

#include <iostream.h>
#include <ipc.h>

// ****************************************************************************

int main()

{
   // print program title
   cout << "semtest1 - Semaphore test program" << endl << endl;

   // allocate an event semaphore
   cout << "creating an event semaphore..." << endl;
   ipcEventSemaphore event_sem("evtsem", semcreate);
   if (!event_sem) {
      cerr << "Error!  state = " << event_sem.rdstate() << endl;
      return 1;
   }

   // show the name, type and id for the semaphore
   cout << "event semaphore name is " << event_sem.Name() << endl
        << "type is " << (int)event_sem.Type() << endl
        << "and id is " << event_sem.ID() << endl << endl;

   // prompt user to start semtest2, reset the event semaphore and wait for
   // the semtest2 program to post the semaphore so this program can continue
   cout << "start the semtest2 program..." << endl;
   cout << "waiting for semtest2 to post the event semaphore..." << endl;
   event_sem.Reset();
   event_sem.Wait();

   // access a mutex semaphore created by semtest2
   cout << "accessing a mutex semaphore..." << endl;
   ipcMutexSemaphore mutex_sem("mtxsem", semaccess);
   if (!mutex_sem) {
      cerr << "Error !  state = " << mutex_sem.rdstate() << endl;
      return 2;
   }

   // show the name, type and id for the semaphore
   cout << "mutex semaphore name is " << mutex_sem.Name() << endl
        << "type is " << (int)mutex_sem.Type() << endl
        << "and id is " << mutex_sem.ID() << endl << endl;

   // request the mutex semaphore and prompt user to hit a key to release it
   cout << "requesting the mutex semaphore..." << endl;
   mutex_sem.Request();
   cout << "press any key to release the mutex semaphore...";
   cin.ignore();
   mutex_sem.Release();

   return 0;
}
