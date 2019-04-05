

//== LS8 RAM ==================================================================
/*
    See header file for documentation
*/

//-- Dependencies ------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

//-- Memory Access -----------------------------------
void ram_read(struct cpu *cpu)
{
    cpu->MDR = cpu->ram[cpu->MAR];
}
void ram_write(struct cpu *cpu)
{
    cpu->ram[cpu->MAR] = cpu->MDR;
}

//-- Load RAM ----------------------------------------
void ram_load(struct cpu *cpu, FILE *program)
{
    // Iterate over reach line of the file
    cpu->MAR = 0;
    char line[0x100];
    while(fgets(line, 0x100, program))
    {
        // Convert lines (strings) into instructions
        char *index_error;
        cpu->MDR = strtoul(line, &index_error, 2);
        if(line == index_error)
        {
            continue;
        }
        // Store instructions sequentially in RAM
        ram_write(cpu);
        cpu->MAR++;
    }
}
