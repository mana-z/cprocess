#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <sys/types.h>
#include <stdbool.h>

extern int process_run(char* const cmd[], const bool cleanEnv);
extern pid_t process_spawn(char* const cmd[], const bool cleanEnv);
extern pid_t process_spawn4(char* const cmd[], const bool cleanEnv,
    int* stdinPipe, int* stdoutPipe);

#endif // __PROCESS_H__
