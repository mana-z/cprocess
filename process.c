
#include "process.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


int process_run(char* const cmd[])
{
  if (0 == cmd) { return -1; }
  if (0 == *cmd) { return -1; }
  pid_t pid = fork();
  if (pid < 0) { return pid; } // error
  else if (0 == pid) {
    clearenv();
    execv(cmd[0], cmd);
  }
  int wstatus = 0;
  waitpid(pid, &wstatus, 0);
  if (WIFEXITED(wstatus) > 0) {
    return WEXITSTATUS(wstatus);
  }
  return -1;
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
