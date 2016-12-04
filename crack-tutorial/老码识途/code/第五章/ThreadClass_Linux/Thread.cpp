#include "Thread.h"

TThread::TThread()
{
  FListener = NULL;
  FThreadID = 0;
}

TThread::TThread(TThreadListener * Listener)
{
  FListener = Listener;
  FThreadID = 0;
}
void * TThread::Wrapper(void * arg)
{
  TThread * Thd;
  Thd = ((TThread *) arg);
  Thd->Execute();
  if (Thd->FListener != NULL)
    Thd->FListener->NotifyThreadFinished(Thd);
  pthread_exit(FReturnValue);
}

void TThread::Run()
{
  pthread_create(&FThreadID, NULL, Wrapper, this);
};

TThread::~TThread()
{
  if (FThreadID != 0)
  {
    WaitFor();
    pthread_detach(FThreadID);
  }
  if (FListener != NULL)
    delete FListener;
};

void TThread::WaitFor()
{
  int Result;
  void * P;
  if (FThreadID == 0)
    return;
  P = &Result;
  pthread_join(FThreadID, &P);
};

int TThread::GetThreadID()
{
  return FThreadID;
};

