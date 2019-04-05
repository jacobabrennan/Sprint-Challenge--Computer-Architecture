

//== LS8 ALU Emulator =========================================================
/*
    See header file for documentation
*/

//-- Dependencies ------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"
#include "operations.h"
#include "alu.h"

//-- ALU Instruction Handler -------------------------
void alu(struct cpu *cpu, unsigned char op, unsigned char regA, unsigned char regB)
{
    // Retrieve specific instruction handler
    unsigned char op_identifier = op & INSTRUCTION_IDENTIFIER;
    operation instruction = operations_alu[op_identifier];
    // Handle bad instructions
    if(NULL == instruction)
    {
        cpu->running = 0;
    }
    // Execute the instruction
    instruction(cpu, regA, regB);
}
