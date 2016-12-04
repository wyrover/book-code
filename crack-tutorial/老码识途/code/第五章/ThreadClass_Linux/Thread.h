#ifndef XXX_Thread_H
#define XXX_Thread_H

#include <pthread.h>

class TThreadListener;

class TThread
{
  private:
    int FThreadID;
    void * FReturnValue;
    TThreadListener * FListener;
    static void * Wrapper(void * arg);
  protected:
    virtual void Execute() = 0;
  public:
    TThread();
    TThread(TThreadListener * Listener);
    ~TThread();
    void Run();
    void WaitFor();
    int GetThreadID();
    void * GetReturnValue(){return FReturnValue};
};

class TThreadListener
{
public:
  virtual void NotifyThreadFinished(TThread * Sender) = 0;
};


#endif