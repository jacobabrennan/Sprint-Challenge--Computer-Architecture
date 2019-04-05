

//== Interrupts ===============================================================
/*
    See header file for documentation
*/

//-- Dependencies ------------------------------------
#include <time.h>
#include <unistd.h>
#include "cpu.h"
#include "ram.h"
#include "operations.h"
#include "interrupts.h"

//-- Generate a time interrupt every second ----------
void timer_advance(struct cpu *cpu, struct timespec *time_current)
{
    time_t seconds_current = time_current->tv_sec;
    clock_gettime(CLOCK_MONOTONIC, time_current);
    if(time_current->tv_sec > seconds_current)
    {
        cpu->registers[REGISTER_INTERRUPT_STATUS] |= INTERRUPT_TIMER;
    }
}

//-- Generate keyboard interrupts --------------------
void check_keyboard(struct cpu *cpu)
{
    char keyboard_input;
    int success_read = read(0, &keyboard_input, 1);
    if(success_read != -1)
    {
        cpu->MAR = ADDRESS_KEYBOARD_INPUT;
        cpu->MDR = keyboard_input;
        ram_write(cpu);
        cpu->registers[REGISTER_INTERRUPT_STATUS] |= INTERRUPT_KEYBOARD;
    }
}

//-- Handle Interrupts -------------------------------
void handle_interrupts(struct cpu *cpu)
{
    // Prior to instruction fetch, the following steps occur:
    // The IM register is bitwise AND-ed with the IS register and the results stored as maskedInterrupts.
    unsigned char maskedInterrupts = cpu->registers[REGISTER_INTERRUPT_MASK] & cpu->registers[REGISTER_INTERRUPT_STATUS];
    // Each bit of maskedInterrupts is checked, starting from 0 and going up to the 7th bit, one for each interrupt.
    for(unsigned char mask_bit = 0; mask_bit < 8; mask_bit++)
    {
        unsigned char mask = 1 << mask_bit;
        // If a bit is found to be set, follow the next sequence of steps. Stop further checking of maskedInterrupts. Note: See break statement.
        if(maskedInterrupts & mask)
        {
            // Disable further interrupts.
            cpu->interruptable = 0;
            // Clear the bit in the IS register.
            cpu->registers[REGISTER_INTERRUPT_STATUS] &= ~mask;
            // The PC register is pushed on the stack.
            cpu->registers[REGISTER_STACK_POINTER]--;
            cpu->MAR = cpu->registers[REGISTER_STACK_POINTER];
            cpu->MDR = cpu->PC;
            ram_write(cpu);
            // The FL register is pushed on the stack.
            cpu->registers[REGISTER_STACK_POINTER]--;
            cpu->MAR = cpu->registers[REGISTER_STACK_POINTER];
            cpu->MDR = cpu->FL;
            ram_write(cpu);
            // Registers R0-R6 are pushed on the stack in that order.
            for(int index_register=0; index_register < REGISTER_INTERRUPT_STATUS; index_register++)
            {
                op_45_PUSH(cpu, index_register, 0);
            }
            // The address (vector in interrupt terminology) of the appropriate handler is looked up from the interrupt vector table.
            cpu->MAR = ADDRESS_INTERRUPTS + mask_bit;
            ram_read(cpu);
            // Set the PC is set to the handler address.
            cpu->PC = cpu->MDR;
            // Stop further checking of maskedInterrupts. (see note above)
            break;
        }
    }
}
