
#include "process.h"

int main(int argc, char** argv)
{
    process_run((char* const[]){ "/usr/bin/echo", "Hello World!", 0 });
    return 0;
}
