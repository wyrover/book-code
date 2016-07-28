#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>



// get from the book "advance unix programming environment, you can also see the 
// exmaple code from many other open source code
// lock a whole file with fcntl function


#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


int lockfile(int fd)
{
  struct flock fl;
  
  fl.l_type = F_WRLCK;
  fl.l_start = 0;
  fl.l_whence = SEEK_SET;
  fl.l_len = 0;
  
  return fcntl(fd, F_SETLK, &fl);
}

int already_running(const char *file)
{
  int fd;
  char buf[16];
 
  fd = open(file, O_RDWR | O_CREAT, LOCKMODE);
 
  if (fd < 0) {
    syslog(LOG_ERR, "can't open %s : %s", file, strerror(errno));
    exit(1);
  } 
  
  if (lockfile(fd) < 0) { 
    if (errno == EAGAIN || errno == EACCES) {
      close(fd);
      return 1;
    }
    syslog(LOG_ERR, "can't lock %s : %s", file, strerror(errno));
    exit(1);
  }
   
  ftruncate(fd, 0); 
  sprintf(buf, "%ld", (long)getpid());
  write(fd, buf, strlen(buf));

  return 0;
}


int main(int arc, char * argv[])
{
  char *cmd;
  char buf[1024] = "/var/run/";
  if ((cmd = strrchr(argv[0], '/')) == NULL) {
    cmd = argv[0];
  } else {
    ++cmd;
  }

  strcat(buf, cmd);  
  strcat(buf, ".pid");

  if (already_running(buf)) {
    printf("process %s is running\n", cmd);
    exit(1);
  } 

  while(1); 
  return 0;
}
