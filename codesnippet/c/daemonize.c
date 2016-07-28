#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/resource.h>
#include <unistd.h>


// get from the book "advance unix programming environment, you can also see the 
// exmaple code from many other open source code (such as memcache)
// how to write a daemon process program

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


int main(int arc, char * argv[])
{
  char *cmd;
  if ((cmd = strrchr(argv[0], '/')) == NULL) {
    cmd = argv[0];
  } else {
    ++cmd;
  }

  daemonize(cmd);
  
  // work do for daemonize
  //
  //
  
  return 0;
}
