#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

volatile int i=0; // use this to determine next image

void TODO(char *task)
{
  fprintf(stderr,"TODO: %s\n", task);
  exit(1);
}

void shutdown(int sig)
{
  fprintf(stderr,"\nSIGINT received\n");
  TODO("shutdown");
}

void download(int sig)
{
  fprintf(stderr,"\nSIGUSR1 received\n");
  TODO("download");
}

int main()
{
  TODO("main");
  return 0;
}

