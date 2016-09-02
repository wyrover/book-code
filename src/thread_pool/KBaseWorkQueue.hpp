//----------------------------------------------------------------------------
// Filename:      KBaseWorkQueue.hpp
//----------------------------------------------------------------------------
#ifndef KBASEWORKQUEUE_HPP
#define KBASEWORKQUEUE_HPP

#pragma once


#include <deque>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/call_traits.hpp>


//----------------------------------------------------------------------------
// class KBaseWorkQueue
//----------------------------------------------------------------------------
// Description: work queue
// Note       : - see 'Programming with POSIX Threads', Butenhof 7.2: this works with
//                a fixed number of threads
//              - uses a std::deque intead of queue; it allows modification in overflow
//----------------------------------------------------------------------------
template <typename T>
class KBaseWorkQueue
{
   typedef KBaseWorkQueue<T>                          self;
   typedef typename T                                 Type;
   typedef typename boost::call_traits<T>::param_type ParamType;
   
public:
   KBaseWorkQueue(size_t nThreads, size_t nMaxSize = -1, bool bStart = true);
   virtual ~KBaseWorkQueue();

   void              Start       ();
   void              Stop        ();

   void              Add         (ParamType t);
   
protected:  //fct
   virtual void      Process     (ParamType tItem) = 0;
   virtual bool      IsOverflow  () const;
   virtual bool      Overflow    (ParamType tItem);

   virtual void      Run         ();
   
protected:    //data
   const size_t         m_nThreads;    //max number of threads
   const size_t         m_nMaxSize;    //max size queue
   bool                 m_bRun;
   size_t               m_nIdle;
   std::deque<T>        m_deque;
   
   boost::mutex         m_mtx;
   boost::condition     m_cv;
   boost::thread_group  m_thds;
};


//----------------------------------------------------------------------------
// Function KBaseWorkQueue::KBaseWorkQueue
//----------------------------------------------------------------------------
// Description  : constructor
//----------------------------------------------------------------------------
template <typename T>
KBaseWorkQueue<T>::KBaseWorkQueue(size_t nThreads, size_t nMaxSize, bool bStart)
 : m_nThreads(nThreads == 0 ? boost::thread::hardware_concurrency() : nThreads)
 , m_nMaxSize(nMaxSize)
 , m_bRun(true)
 , m_nIdle(0)
{
   if (bStart)
   {
      Start();
   }
}


//----------------------------------------------------------------------------
// Function KBaseWorkQueue<T>::~KBaseWorkQueue
//----------------------------------------------------------------------------
// Description  : destructor
//----------------------------------------------------------------------------
template <typename T>
KBaseWorkQueue<T>::~KBaseWorkQueue()
{
   Stop();
}


//----------------------------------------------------------------------------
// Function KBaseWorkQueue<T>::Start
//----------------------------------------------------------------------------
// Description  : start thread(s)
//----------------------------------------------------------------------------
template <typename T>
void KBaseWorkQueue<T>::Start()
{
   _ASSERT(m_thds.size() == 0);

   //create threads
   for (size_t n = 0; n != m_nThreads; ++n)
   {
      m_thds.create_thread(boost::bind(&self::Run, this));
   }
}


//----------------------------------------------------------------------------
// Function KBaseWorkQueue<T>::Stop
//----------------------------------------------------------------------------
// Description  : stop and wait for thread(s) to be finished
//----------------------------------------------------------------------------
template <typename T>
void KBaseWorkQueue<T>::Stop()
{
   boost::unique_lock<boost::mutex> lck(m_mtx);

   m_bRun = false;

   m_cv.notify_all();

   lck.unlock();

   //wait for the thread(s) to be finished
   m_thds.join_all();
}


//----------------------------------------------------------------------------
// Function KBaseWorkQueue<T>::Add
//----------------------------------------------------------------------------
// Description  : add item
//----------------------------------------------------------------------------
template <typename T>
void KBaseWorkQueue<T>::Add(ParamType tItem)
{
   //algorithm:
   //1) add item
   //2) notify consumer threads

   boost::unique_lock<boost::mutex> lck(m_mtx);

   bool bNotify = true;

   //1) add item
   if (IsOverflow())
   {
      bNotify = Overflow(tItem);
   }
   else
   {
      m_deque.push_back(tItem);
   }

   //2) notify consumer threads
   if (bNotify)
   {
      //notify a thread only if idle (otherwise the item will get picked up automatically)
      if (m_nIdle != 0)
      {
         m_cv.notify_one();
      }
   }
}


//----------------------------------------------------------------------------
// Function KBaseWorkQueue<T>::Run
//----------------------------------------------------------------------------
// Description  : thread run
//----------------------------------------------------------------------------
template <typename T>
void KBaseWorkQueue<T>::Run()
{
   boost::unique_lock<boost::mutex> lck(m_mtx);

   for (;;)
   {
      //wait for an item to be pushed
      while (m_deque.empty() && m_bRun)
      {
         ++m_nIdle;

         m_cv.wait(lck);

         --m_nIdle;
      }

      if (!m_deque.empty())
      {
         const T tItem = m_deque.front();
         m_deque.pop_front();
      
         //release lock during processing
         lck.unlock();

         Process(tItem);

         lck.lock();
      }
      
      //test queue again (because lock is released during a period)
      if (m_deque.empty() && !m_bRun)
      {
         //stop thread (queue is emptied and run flag is false)
         break;
      }
   }
}


//----------------------------------------------------------------------------
// Function KBaseWorkQueue<T>::IsOverflow
//----------------------------------------------------------------------------
// Description  : is overflow
//----------------------------------------------------------------------------
template <typename T>
bool KBaseWorkQueue<T>::IsOverflow() const
{
   //default overflow if max size is specified and it goes over that border
   return (m_nMaxSize != -1) && (m_deque.size() >= m_nMaxSize);
}


//----------------------------------------------------------------------------
// Function KBaseWorkQueue<T>::Overflow
//----------------------------------------------------------------------------
// Description  : overflow handler: called in calling thread with mutex locked
// Return value : true if consumer threads should be notified, false otherwise
//----------------------------------------------------------------------------
template <typename T>
bool KBaseWorkQueue<T>::Overflow(ParamType /*tItem*/)
{
   return false;
}


#endif //KBASEWORKQUEUE_HPP
