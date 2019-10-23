
#include "process.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


int process_run(char* const cmd[], const bool cleanEnv)
{
  pid_t pid = process_spawn(cmd, cleanEnv);
  int wstatus = 0;
  waitpid(pid, &wstatus, 0);
  if (WIFEXITED(wstatus) > 0) {
    return WEXITSTATUS(wstatus);
  }
  return -1;
}

pid_t process_spawn(char* const cmd[], const bool cleanEnv)
{
  if (0 == cmd) { return -1; }
  if (0 == *cmd) { return -1; }
  pid_t pid = fork();
   if (0 == pid) {
    if (true == cleanEnv) { clearenv(); }
    execv(cmd[0], cmd);
  }
  return pid;
}

pid_t process_spawn4(char* const cmd[], const bool cleanEnv,
    int* stdinPipe, int* stdoutPipe)
{
  // TODO
  int stdinBackup = 0;
  int stdinfd[2] = { 0 };
  int stdoutBackup = 0;
  int stdoutfd[2] = { 0 };
  if (0 != stdinPipe) {
    stdinBackup = dup(STDIN_FILENO);
  }
}

/* inspiration for spawn:
int main(int argc, char** argv)
{
    int stdoutBackup = dup(STDOUT_FILENO);
    int stdinfd[2] = { 0 };
    pipe(stdinfd);
    dup2(stdinfd[0], STDIN_FILENO);
    int stdoutfd[2] = { 0 };
    pipe(stdoutfd);
    dup2(stdoutfd[1], STDOUT_FILENO);
    int fd = fork();
    if(0 == fd)
    {
        close(stdinfd[1]);
        close(stdoutfd[0]);
        execl("/bin/bash", "/bin/bash", 0);
    }
    else if(fd > 0)
    {
        dup2(stdoutBackup, STDOUT_FILENO);
        close(stdinfd[0]);
        close(stdoutfd[1]);
        char cmd[] = "for i in {0..10}; do echo $i; done; exit\n";
        write(stdinfd[1], cmd, sizeof(cmd));
        wait(0);
        char buf[128] = { 0 };
        read(stdoutfd[0], buf, sizeof(buf));
        printf("we got %s\n", buf);
        close(stdinfd[1]);
        close(stdoutfd[0]);
        return 0;
    }
    else
    {
        dup2(stdoutBackup, STDOUT_FILENO);
        fprintf(stderr, "error!\n");
        return 1;
    }
}
*/
