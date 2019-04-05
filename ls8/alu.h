

//== LS8 ALU Emulator =========================================================
/*
    This module emulates an ALU. The program as a whole could be simplified by
    removing it. The goal of the project, however, is to emulate the structure
    and function of a CPU, of which the ALU is a discrete structure within the
    CPU. That structure is emulated by this module.

    This module provides one function, alu, which executes arithmetical
    instructions on behalf of the CPU. It accepts the following arguments:
        *cpu: The CPU executing this operation
        op: The instruction's OP code
        regA, regB: Numbers 0-8, specifying which registers to operate on
*/

//-- Dependencies ------------------------------------
#include "cpu.h"

//-- ALU Instruction Handler -------------------------
void alu(struct cpu *cpu, unsigned char op, unsigned char regA, unsigned char regB);
