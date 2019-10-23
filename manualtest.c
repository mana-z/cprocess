
#include "process.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
  process_run((char* const[]){ "/usr/bin/echo", "Hello World!", 0 }, true);
  int inpipe = 0;
  int outpipe = 0;
  pid_t pid = process_spawn((char* const[]){ "/bin/bash", 0 }, true, &inpipe,
      &outpipe);
  char cmd[] = "for i in {0..10}; do echo $i; done; exit\n";
  write(inpipe, cmd, sizeof(cmd));
  waitpid(pid, 0, 0);
  char buf[128] = { 0 };
  read(outpipe, buf, sizeof(buf));
  printf("we got %s\n", buf);
  close(inpipe);
  close(outpipe);
  return 0;
}
