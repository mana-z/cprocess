/*!*****************************************************************************
 * The following file is available under CC0 license. See COPYING for details.
 ******************************************************************************/

#include "process.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

extern char** environ;

/*!*****************************************************************************
 * @see [process.h]
 ******************************************************************************/
int process_run(char* const cmd[], const bool cleanEnv)
{
    // just reuse spawn and wait for finish
    pid_t pid = process_spawn(cmd, cleanEnv, 0, 0);
    if (pid < 0) { return pid; }
    int wstatus = 0;
    waitpid(pid, &wstatus, 0);
    if (WIFEXITED(wstatus) > 0) {
        return WEXITSTATUS(wstatus);
    }
    return -1;
}

/*!*****************************************************************************
 * @see [process.h]
 ******************************************************************************/
pid_t process_spawn(char* const cmd[], const bool cleanEnv, int* stdinPipe, int* stdoutPipe)
{
    if (0 == cmd) { return -1; }
    if (0 == *cmd) { return -1; }

    int stdinfd[2] = { 0 };
    int stdoutfd[2] = { 0 };
    if (0 != stdinPipe) {
        if (-1 == pipe(stdinfd)) { goto error; }
    }
    if (0 != stdoutPipe) {
        if (-1 == pipe(stdoutfd)) { goto error_inpipeclose; }
    }

    pid_t pid = fork();

    // Do not worry about the closes here, pipes will survive, only an in-kernel ref count will
    // lower and the child will have a clean fd state
    if (0 == pid)
    {
        // Note: yes, the error exits leak, but when a dup2 fails, it is the least of your problems.
        // And the leak does not propagate into parent, so it is not a big deal.
        if (0 != stdinPipe) {
            // Replace child stdin with read end of spawned pipe and close unused fds
            if (-1 == dup2(stdinfd[0], 0)) { exit(errno); }
            close(stdinfd[0]);
            close(stdinfd[1]);
        }
        if (0 != stdoutPipe) {
            // Replace child stdout with write end of spawned pipe and close unused fds
            if (-1 == dup2(stdoutfd[1], 1)) { exit(errno); }
            close(stdoutfd[0]);
            close(stdoutfd[1]);
        }
        if (true == cleanEnv) { environ = 0; }
        execv(cmd[0], cmd);
    }

    // close pipe ends unused by a parent and pass requested pipe fds
    if (0 != stdinPipe) {
        close(stdinfd[0]);
        if (pid > 0) { *stdinPipe = stdinfd[1]; }
        else { close(stdinfd[1]); }
    }
    if (0 != stdoutPipe) {
        close(stdoutfd[1]);
        if (pid > 0) { *stdoutPipe = stdoutfd[0]; }
        else { close(stdoutfd[0]); }
    }

    return pid;
error_inpipeclose:
    if (0 != stdinPipe) { close(stdinfd[0]); close(stdinfd[1]); }
error:
    return -1;
}

