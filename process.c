
#include "process.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>


int process_run(char* const cmd[], const bool cleanEnv)
{
  pid_t pid = process_spawn(cmd, cleanEnv, 0, 0);
  int wstatus = 0;
  waitpid(pid, &wstatus, 0);
  if (WIFEXITED(wstatus) > 0) {
    return WEXITSTATUS(wstatus);
  }
  return -1;
}

pid_t process_spawn(char* const cmd[], const bool cleanEnv,
    int* stdinPipe, int* stdoutPipe)
{
  if (0 == cmd) { return -1; }
  if (0 == *cmd) { return -1; }

  int stdinfd[2] = { 0 };
  int stdoutfd[2] = { 0 };
  if (0 != stdinPipe) {
    if (-1 == pipe(stdinfd)) { return -1; }
  }
  if (0 != stdoutPipe) {
    if (-1 == pipe(stdoutfd)) {
      if (0 != stdoutPipe) { close(stdinfd[0]); close(stdinfd[1]); }
      return -1;
    }
  }
  pid_t pid = fork();
  if (0 == pid)
  {
    if (0 != stdinPipe) {
      if (-1 == dup2(stdinfd[0], 0)) { exit(errno); }
      close(stdinfd[0]);
      close(stdinfd[1]);
    }
    if (0 != stdoutPipe) {
      if (-1 == dup2(stdoutfd[1], 1)) { exit(errno); }
      close(stdoutfd[0]);
      close(stdoutfd[1]);
    }
    if (true == cleanEnv) { clearenv(); }
    execv(cmd[0], cmd);
  }
  if (0 != stdinPipe) {
    close(stdinfd[0]);
    *stdinPipe = stdinfd[1];
  }
  if (0 != stdoutPipe) {
    close(stdoutfd[1]);
    *stdoutPipe = stdoutfd[0];
  }
  return pid;
}

