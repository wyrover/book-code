This file is a part of 1590598318-1.zip containing example source code for the 
Foundations of Qt Development book available from APress (ISBN 1590598318).

These are the examples for chapter 12 - Doing Things in Parallel

simplethreads

  Listings 12-1, 12-2, 12-3
  
  Introduces the QThread class.


inorderthreads

  Listing 12-5 
  
  Orders two QThread classes using a QMutex.


orderedthreads

  Listings 12-6, 12-7, 12-8, 12-9
  
  Two ordered QThreads are used with a device with a shared counter protected by
  a QMutex.


readwritethreads

  Listings 12-11, 12-12, 12-13, 12-14, 12-15
  
  Protects the shared counter using a QReadWriteLock instead of a mutex.


basicsemaphore

  Listing 12-17 
  
  Introduces the QSemaphore class.


semaphorethreads

  Listings 12-18, 12-19, 12-20, 12-21
  
  Passes data between two threads (one producer and one consumer) using a buffer
  controlled by two semaphores.


competingsemaphore

  Listings 12-22, 12-23, 12-24
  
  Passes data between several threads (many producers and one consumer) using a
  bugger controlled by sempahores.


signallingthreads

  Listings 12-26, 12-27, 12-28, 12-29, 12-30
  
  Passes QString objects between threads using signals and slots.


customsignals

  Listings 12-32, 12-33, 12-34, 12-35
  
  Passes a custom type between threads using signals and slots.


uithread

  Listings 12-37, 12-38, 12-39

  Shows how to use threads in conjunction with a graphical user interface.


processes

  Listings 12-40, 12-41, 12-42

  Shows how to use the QProcess object to run and monitor an external process.