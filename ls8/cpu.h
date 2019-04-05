

//== LS8 CPU Emulator =========================================================
/*
    This module defines a CPU structure, functions to initialize that structure
    and execute programs, and various constants.

    cpu_init() initializes the CPU struct to default values, and then loads an
    LS8 assembly program into emulated RAM. It accepts the following arguments:
        *cpu: The structure to initialize
        file_path: A string representing a path to a file on the file system
            from which to load the LS8 program
    
    cpu_run() will execute a program previously loaded into emulated RAM. It
    accepts one argument:
        *cpu: The emulated CPU to run
*/

//-- Run Once (see end of file) ----------------------
#ifndef _CPU_H_
#define _CPU_H_

//-- Dependencies ------------------------------------
#include <stdio.h>

//-- Module Constants --------------------------------
// Mask for truncating values to bytes
#define BYTE 0xff
// Important Addresses
#define ADDRESS_STACK_START 0xf4
#define ADDRESS_KEYBOARD_INPUT 0xf4
#define ADDRESS_INTERRUPTS 0xf8
// Instruction flag masks
#define INSTRUCTION_IDENTIFIER 0b00001111
#define INSTRUCTION_OPERAND_COUNT 0b11000000
#define INSTRUCTION_ALU 0b00100000
#define INSTRUCTION_PC_MOVE 0b00010000
// Comparison flag masks
#define CMP_FLAG_EQUAL 0b00000001
#define CMP_FLAG_GREATER 0b00000010
#define CMP_FLAG_LESS 0b00000100
// Interrupt flag masks
#define INTERRUPT_TIMER 0b00000001
#define INTERRUPT_KEYBOARD 0b00000010
// Registers
#define REGISTER_INTERRUPT_MASK 5
#define REGISTER_INTERRUPT_STATUS 6
#define REGISTER_STACK_POINTER 7
// Error Codes
#define ERROR_UNKNOWN_INSTRUCTION 1
#define ERROR_USAGE_ARGUMENTS 2
#define ERROR_FILE_ACCESS 3
#define ERROR_TERMIOS 4
#define ERROR_CPU_RUNNING 5

//-- CPU Struct --------------------------------------
struct cpu {
    // Holds all information about the CPU
    unsigned char PC;
    unsigned char IR;
    unsigned char MAR;
    unsigned char MDR;
    unsigned char FL;
    unsigned char *registers;
    unsigned char *ram;
    unsigned char running;
    unsigned char interruptable;
};

//-- CPU Initialization ------------------------------
void cpu_init(struct cpu *cpu, FILE *program);

//-- Execute Program in RAM --------------------------
int cpu_run(struct cpu *cpu);

//-- Run Once ----------------------------------------
#endif
