#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>

// get from the book "advance unix programming environment"
// also see it in the file eio.c of the open source code libeio

void sleep(struct timeval *tvptr)
{
  select(0, NULL, NULL, NULL, tvptr);
}

int main(int argc, char *argv[])
{

  struct timeval tv;
  tv.tv_sec = 0;      // seconds
  tv.tv_usec = 5000;  // microseconds

  // sleep 5 ms
  sleep(&tv); 

  return 0;
}
