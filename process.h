#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <sys/types.h>

extern int process_run(char* const cmd[]);
extern pid_t process_spawn(int* stdinPipe, int* stdoutPipe);

#endif // __PROCESS_H__
