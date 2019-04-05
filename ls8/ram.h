

//== LS8 RAM ==================================================================
/*
    This module provides functions for accessing an LS8 CPU's RAM.

    In order to read or write, the MAR and MDR registers must be set. The MAR
    register specifies the memory address. When writing, the MDR holds the
    value to be written; when reading, the value read will be written to the
    MDR.

    The ram_load function attempts to load an LS8 program from a file into RAM.
    It will exit the emulation with an exit code if it was unable to read from
    the file.
*/

//-- Dependencies ------------------------------------
#include "cpu.h"
#include <stdio.h>

//-- Memory Access -----------------------------------
// Read from CPU RAM
void ram_read(struct cpu *cpu);
// Write to CPU RAM
void ram_write(struct cpu *cpu);


//-- Load RAM ----------------------------------------
// Load the binary bytes from a .ls8 source file into a RAM array
void ram_load(struct cpu *cpu, FILE *program);
