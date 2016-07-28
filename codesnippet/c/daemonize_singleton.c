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
// exmaple code from many other open source code (such as memcache)
// how to write a singleton daemon process program


#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void daemonize(const char *cmd)
{
  int i, fd0, fd1, fd2;
  pid_t pid;
  struct rlimit rl;
  struct sigaction sa;

  umask(0);
  if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
    printf("error : can't get file limit\n");
    exit(1);
  }

  if ((pid = fork()) < 0) {
    printf("error : can't fork\n");
    exit(1);
  } else if (pid != 0) { // parent
    exit(0);
  }

  setsid();
  
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGHUP, &sa, NULL) < 0) {
    printf("error : can't ignor SIGHUP\n");
    exit(1);
  }

  if ((pid = fork()) < 0) {
    printf("error : can't fork\n");
    exit(1);
  } else if (pid != 0) { // parent
   exit(0);
  }
  /* change the current working directory to "/" */
  if (chdir("/") < 0) {
    printf("error : can't chdir\n");
    exit(1);
  }

  if (rl.rlim_max == RLIM_INFINITY) {
    rl.rlim_max = 1024;
  }

  for (i = 0; i < rl.rlim_max; i++) {
    close(i);
  }
 
  fd0 = open("/dev/null", O_RDWR); 
  fd1 = dup(0);
  fd2 = dup(0);

  // initialize the log file
  openlog(cmd, LOG_CONS, LOG_DAEMON);
  if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
    syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
    exit(1);
  }
}

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

  // daemonize function must be set before already_running 
  // function. because if daemonize is set after the already_running
  // function. when the daemonize is run. the parent process is exit.
  // and the child process will inherit the lock. but why we can't do this.
  // unforunately we close the fd in the child process. so the lock is released. 
  daemonize(cmd); 

  if (already_running(buf)) {
    printf("process %s is running\n", cmd);
    exit(1);
  }

  for (;;) {
    // to do work
  }

  return 0;
}
