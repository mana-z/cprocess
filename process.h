/*!*****************************************************************************
 * The following file is available under CC0 license. See COPYING for details.
 ******************************************************************************/

#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <sys/types.h>
#include <stdbool.h>

/*!*****************************************************************************
 * @brief Synchronously run command
 * @param[in] cmd see process_spawn
 * @param[in] cleanEnv see process_spawn
 * @return Return value of process
 * @retval -1 Error happened, errno may give more information
 ******************************************************************************/
extern int process_run(char* const cmd[], const bool cleanEnv);


/*!*****************************************************************************
 * @brief Run command asynchronously
 * @note The first value of cmd array is automatically taken as a command, the
 * whole array is a null terminated argument list.
 * @param[in] cmd Command with arguments. The last value shall be NULL,
 * @param[in] cleanEnv True if command shall be run in clean environment
 * @param[out] stdinPipe Optional pointer to the child stdin pipe storage
 * @param[out] stdoutPipe Optional pointer to the child stdout pipe storage
 * @return PID of the child
 * @retval -1 Error happened, errno may give more information
 ******************************************************************************/
extern pid_t process_spawn(char* const cmd[], const bool cleanEnv, int* stdinPipe, int* stdoutPipe);

#endif // __PROCESS_H__

