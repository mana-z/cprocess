
#include "process.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Synchronous example - use echo to print something. With clean environment
    int retval = process_run((char* const[]){ "/usr/bin/echo", "Hello World!", 0 }, true);
    printf("Echo returned status %i.\n", retval);

    // Asynchronous example with both pipes - write a script to bash and read what it outputs
    //
    // Note: if pipes are not needed, e.g. we want the output to go directly to stdout, a null may
    // be passed instead of fd pointers.
    int inpipe = 0;
    int outpipe = 0;

    pid_t pid = process_spawn((char* const[]){ "/bin/bash", 0 }, true, &inpipe, &outpipe);

    char cmd[] = "for i in {0..10}; do echo $i; done\n";
    write(inpipe, cmd, sizeof(cmd));
    close(inpipe);

    // we can read the status here, see waitpid(3) manpage.
    waitpid(pid, 0, 0);

    // read the result and just put an arbitrary prefix, so the user can see it
    // may be further processed
    char buf[128] = { 0 };
    read(outpipe, buf, sizeof(buf));
    printf("we got %s\n", buf);
    close(outpipe);

    return 0;
}
