

//== LS8 CPU Emulator =========================================================
/*
    See header file for documentation
*/

//-- Dependencies ------------------------------------
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "cpu.h"
#include "operations.h"
#include "alu.h"
#include "ram.h"
#include "interrupts.h"

//-- CPU Initialization ------------------------------
void cpu_init(struct cpu *cpu, FILE *program)
{
    // When the LS-8 is booted, the following steps occur:
    // R0-R6 are cleared to 0.
    cpu->registers = calloc(8, sizeof(char));
    // R7 is set to 0xF4.
    cpu->registers[REGISTER_STACK_POINTER] = ADDRESS_STACK_START;
    // PC and FL registers are cleared to 0.
    cpu->PC = 0;
    cpu->FL = 0;
    // RAM is cleared to 0.
    cpu->ram = calloc(0x100, sizeof(char));
    //
    cpu->IR = 0;
    cpu->MAR = 0;
    cpu->MDR = 0;
    cpu->running = 0;
    cpu->interruptable = 1;
    // Subsequently, the program can be loaded into RAM starting at address 0x00.
    ram_load(cpu, program);
}

//-- Execute Program in RAM --------------------------
int cpu_run(struct cpu *cpu)
{
    // Cancel if this CPU is already running
    if(cpu->running)
    {
        return ERROR_CPU_RUNNING;
    }
    // Load instruction handlers
    load_operations();
    // Setup Timer for recurring interrupt
    struct timespec time_current;
    clock_gettime(CLOCK_MONOTONIC, &time_current);
    // Execute instructions
    int error_code = 0;
    cpu->running = 1;
    while(cpu->running)
    {
        // Advance Timer
        timer_advance(cpu, &time_current);
        // Check OS for keyboard input, translate into interrupt
        check_keyboard(cpu);
        // Check for and handle interrupts
        handle_interrupts(cpu);
        // Load instruction (op code) from RAM
        cpu->MAR = cpu->PC;
        ram_read(cpu);
        cpu->IR = cpu->MDR;
        // Load operand 1 from RAM
        cpu->MAR = cpu->PC+1;
        ram_read(cpu);
        unsigned char operand_1 = cpu->MDR;
        // Load operand 2 from RAM
        cpu->MAR = cpu->PC+2;
        ram_read(cpu);
        unsigned char operand_2 = cpu->MDR;
        // Determine operand count
        unsigned char count_operand = cpu->IR >> 6;
        // Handle ALU instructions
        if(cpu->IR & INSTRUCTION_ALU)
        {
            alu(cpu, cpu->IR, operand_1, operand_2);
            if(!(cpu->running))
            {
                error_code = ERROR_UNKNOWN_INSTRUCTION;
                break;
            }
            cpu->PC += (1 + count_operand);
        }
        // Handle CPU instructions that set the PC
        else if(cpu->IR & INSTRUCTION_PC_MOVE)
        {
            operation instruction = operations_cpu_jump[cpu->IR & INSTRUCTION_IDENTIFIER];
            if(NULL == instruction)
            {
                error_code = ERROR_UNKNOWN_INSTRUCTION;
                break;
            }
            instruction(cpu, operand_1, operand_2);
        }
        // Handle all other CPU instructions
        else
        {
            operation instruction = operations_cpu[cpu->IR & INSTRUCTION_IDENTIFIER];
            if(NULL == instruction)
            {
                error_code = ERROR_UNKNOWN_INSTRUCTION;
                break;
            }
            instruction(cpu, operand_1, operand_2);
            cpu->PC += (1 + count_operand);
        }
    }
    // Free memory allocated for instruction handlers before returning
    unload_operations();
    // Return any error codes
    return error_code;
}
