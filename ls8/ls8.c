

//== LS8 Eight Bit computer emulation =========================================
/*
    This module emulates an LS8 eight bit CPU. When invoked from the command
    prompt, it must be supplied a file path as the only parameter:

        $ ./ls8 example/printstr.ls8
*/

//-- Dependencies ------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include "cpu.h"

//-- Error Handling ----------------------------------
int handle_error(int error_code)
{
    switch(error_code)
    {
        case ERROR_USAGE_ARGUMENTS:
            printf("Usage: ls8 FilePath\n");
            break;
        case ERROR_FILE_ACCESS:
            printf("The specified file could not be opened\n");
            break;
        case ERROR_TERMIOS:
            printf("Couldn't configure terminal for noncanonical I/O\n");
            break;
        case ERROR_CPU_RUNNING:
            printf("An attempt was made to boot the CPU while already running.\n");
    }
    return error_code;
}

//-- Main --------------------------------------------
int main(int argc, char *argv[])
{
    // Get file path from command line, and handle errors
    if(2 != argc)
    {
        return handle_error(ERROR_USAGE_ARGUMENTS);
    }
    char *file_path = argv[1];
    // Open file for reading. Handle errors.
    FILE *program = fopen(file_path, "r");
    if(NULL == program)
    {
        return handle_error(ERROR_FILE_ACCESS);
    }
    // Configure terminal for non-canonical input / output
    struct termios termios_p;
    int success = tcgetattr(0, &termios_p);
    if(0 != success)
    {
        return handle_error(ERROR_TERMIOS);
    }
    struct termios termios_sane = termios_p;
    cfmakeraw(&termios_p);
    termios_p.c_cc[VMIN] = 0;
    termios_p.c_cc[VTIME] = 0;
    success = tcsetattr(0, TCSANOW, &termios_p);
    if(0 != success)
    {
        tcsetattr(0, TCSANOW, &termios_sane);
        return handle_error(ERROR_TERMIOS);
    }
    // Boot and run CPU
    struct cpu cpu;
    cpu_init(&cpu, program);
    int error_code = cpu_run(&cpu);
    // Cleanup and Exit
    tcsetattr(0, TCSANOW, &termios_sane);
    return handle_error(error_code);
}
