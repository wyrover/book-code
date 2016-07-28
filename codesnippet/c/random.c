#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//get from the file util.c of open source code siege 

int urandom()
{
  int  rand = -1;
  int  fd;
  
  if ((fd = open("/dev/urandom", O_RDONLY)) >= 0) {
    read(fd, &rand, sizeof(rand));
    close(fd);
  } else {
    // other random function
  }
  return rand;
}


int main(int argc, char *argv[])
{
  printf("%d\n", urandom());
  return 0;
}
