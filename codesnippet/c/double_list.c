#include <stdio.h>

struct list_head {
  struct list_head *next, *prev;
};

// embeded link into data 
struct timer {
  int seconds;
  int useconds;
  struct list timer_list;
};

// embeded data into link
struct timer1 {
 struct timer1 *next *prev;
 int seconds;
 int useconds;
}; 



