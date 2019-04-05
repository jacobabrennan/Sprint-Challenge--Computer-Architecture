

//== Operations (Op Codes) ====================================================
/*
    See header file for documentation
*/

//-- Dependencies ------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"
#include "operations.h"


//== Operation Array Memory Management ========================================

//-- Variables to be exported ------------------------
operation *operations_cpu = NULL;
operation *operations_cpu_jump = NULL;
operation *operations_alu = NULL;

//-- Memory freeing function -------------------------
void unload_operations()
{
    free(operations_cpu);
    free(operations_cpu_jump);
    free(operations_alu);
}

//-- Memory allocation and setup function ------------
void load_operations()
{
    // Load base CPU instructions (no jumps)
    if(NULL == operations_cpu)
    {
        operations_cpu = calloc(0x10, sizeof(operation));
        operations_cpu[0x0] = op_00_NOP;
        operations_cpu[0x1] = op_01_HLT;
        operations_cpu[0x2] = op_82_LDI;
        operations_cpu[0x3] = op_83_LD;
        operations_cpu[0x4] = op_84_ST;
        operations_cpu[0x5] = op_45_PUSH;
        operations_cpu[0x6] = op_46_POP;
        operations_cpu[0x7] = op_47_PRN;
        operations_cpu[0x8] = op_48_PRA;
    }
    // Load CPU instructions with jumps
    if(NULL == operations_cpu_jump)
    {
        operations_cpu_jump = calloc(0x10, sizeof(operation));
        operations_cpu_jump[0x0] = op_50_CALL;
        operations_cpu_jump[0x1] = op_11_RET;
        // operations_cpu_jump[0x2] = op_52_INT;
        operations_cpu_jump[0x3] = op_13_IRET;
        operations_cpu_jump[0x4] = op_54_JMP;
        operations_cpu_jump[0x5] = op_55_JEQ;
        // operations_cpu_jump[0x6] = op_56_JNE;
        operations_cpu_jump[0x7] = op_57_JGT;
        operations_cpu_jump[0x8] = op_58_JLT;
        // operations_cpu_jump[0x9] = op_59_JLE;
        // operations_cpu_jump[0xa] = op_5a_JGE;
    }
    // Load ALU instructions
    if(NULL == operations_alu)
    {
        operations_alu = calloc(0x10, sizeof(operation));
        operations_alu[0x0] = op_a0_ADD;
        // operations_alu[0x1] = op_a1_SUB;
        operations_alu[0x2] = op_a2_MUL;
        // operations_alu[0x3] = op_a3_DIV;
        // operations_alu[0x4] = op_a4_MOD;
        operations_alu[0x5] = op_65_INC;
        operations_alu[0x6] = op_66_DEC;
        operations_alu[0x7] = op_a7_CMP;
        // operations_alu[0x8] = op_a8_AND;
        // operations_alu[0x9] = op_69_NOT;
        // operations_alu[0xa] = op_aa_OR;
        // operations_alu[0xb] = op_ab_XOR;
        operations_alu[0xc] = op_ac_SHL;
        // operations_alu[0xd] = op_ad_SHR;
    }
}


//== Operation Handlers =======================================================
/* 
    All the handlers have the same signature, but not all of them use all the
    available arguments. (void) statement are used to prevent the compiler from
    generating warnings.
*/

//-- NOP: Do nothing ---------------------------------
void op_00_NOP(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(cpu);
    (void)(operand_1);
    (void)(operand_2);   
}

//-- HLT: Halt execution -----------------------------
void op_01_HLT(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_1);
    (void)(operand_2);
    cpu->running = 0;
}

//-- RET: Return from subroutine ---------------------
void op_11_RET(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_1);
    (void)(operand_2);
    cpu->PC = cpu->ram[cpu->registers[REGISTER_STACK_POINTER]];
    cpu->registers[REGISTER_STACK_POINTER]++;
}

//-- IRET: Return from interrupt ---------------------
void op_13_IRET(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_1);
    (void)(operand_2);
    // The following steps are executed:
    // Registers R6-R0 are popped off the stack in that order.
    for(int index_register=REGISTER_INTERRUPT_STATUS-1; index_register >= 0; index_register--)
    {
        op_46_POP(cpu, index_register, 0);
    }
    // The FL register is popped off the stack.
    cpu->FL = cpu->ram[cpu->registers[REGISTER_STACK_POINTER]];
    cpu->registers[REGISTER_STACK_POINTER] = (cpu->registers[REGISTER_STACK_POINTER]+1) & BYTE;
    // The return address is popped off the stack and stored in PC.
    cpu->PC = cpu->ram[cpu->registers[REGISTER_STACK_POINTER]];
    cpu->registers[REGISTER_STACK_POINTER] = (cpu->registers[REGISTER_STACK_POINTER]+1) & BYTE;
    // Interrupts are re-enabled
    cpu->interruptable = 1;
}

//-- PUSH: Push value from register onto stack -------
void op_45_PUSH(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    cpu->registers[REGISTER_STACK_POINTER]--;
    cpu->ram[cpu->registers[REGISTER_STACK_POINTER]] = cpu->registers[operand_1];
}

//-- POP: Pop value from stack into register ---------
void op_46_POP(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    cpu->registers[operand_1] = cpu->ram[cpu->registers[REGISTER_STACK_POINTER]];
    cpu->registers[REGISTER_STACK_POINTER] = (cpu->registers[REGISTER_STACK_POINTER]+1) & BYTE;
}

//-- PRN: Print numeric value from register ----------
void op_47_PRN(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    fprintf(stdout, "\r%d\n", cpu->registers[operand_1]);
}

//-- PRA: Print character from register --------------
void op_48_PRA(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    fprintf(stdout, "%c", cpu->registers[operand_1]);
    fflush(stdout);
}

//-- CALL: Call subroutine at given register ---------
void op_50_CALL(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    cpu->registers[REGISTER_STACK_POINTER]--;
    cpu->ram[cpu->registers[REGISTER_STACK_POINTER]] = cpu->PC+2;
    cpu->PC = cpu->registers[operand_1];
}

//-- INT: Issue interrupt number from register--------
// operations[0x52] = op_52_INT;

//-- JMP: Set Program counter from register ----------
void op_54_JMP(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    cpu->PC = cpu->registers[operand_1];
}

//-- JEQ: Jump to register value, if equal -----------
void op_55_JEQ(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    if(cpu->FL & CMP_FLAG_EQUAL)
    {
        cpu->PC = cpu->registers[operand_1];
    }
    else
    {
        cpu->PC += 2;
    }
}

//-- JNE: Jump to register value, if not equal -------
// operations[0x56] = op_56_JNE;

//-- JGT: Jump to register value, if greater than ----
void op_57_JGT(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    if(cpu->FL & CMP_FLAG_GREATER)
    {
        cpu->PC = cpu->registers[operand_1];
    }
    else
    {
        cpu->PC += 2;
    }
    
}

//-- JLT: Jump to register value, if less than -------
void op_58_JLT(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    if(cpu->FL & CMP_FLAG_LESS)
    {
        cpu->PC = cpu->registers[operand_1];
    }
    else
    {
        cpu->PC += 2;
    }
    
}

//-- JLE: Jump to register value, if less or equal ---
// operations[0x59] = op_59_JLE;

//-- JGE: Jump to reg. value, if greater or equal ----
// operations[0x5a] = op_5a_JGE;

//-- INC: Increment value in register ----------------
void op_65_INC(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    cpu->registers[operand_1] = BYTE & (cpu->registers[operand_1]+1);
}

//-- DEC: Decrement value in register ----------------
void op_66_DEC(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    cpu->registers[operand_1] = BYTE & (cpu->registers[operand_1]-1);
}

//-- NOT: Perform bitwise NOT value on register ------
// operations[0x69] = op_69_NOT;

//-- LDI: Set value of register to an integer --------
void op_82_LDI(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    cpu->registers[operand_1] = operand_2;
}

//-- LD: Load registerA with value from registerB ----
void op_83_LD(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    cpu->registers[operand_1] = cpu->ram[cpu->registers[operand_2]];
}

//-- ST: Store value of registerB in registerA -------
void op_84_ST(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    cpu->ram[cpu->registers[operand_1]] = cpu->registers[operand_2];
}

//-- ADD: Store sum of registerA and B in registerA --
void op_a0_ADD(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    unsigned char result = BYTE & (cpu->registers[operand_1] + cpu->registers[operand_2]);
    cpu->registers[operand_1] = result;   
}

//-- SUB: Store difference of reg.B from A in reg.A --
// operations[0xa1] = op_a1_SUB;

//-- MUL: Store product of reg.A and B in reg.A ------
void op_a2_MUL(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    unsigned char result = BYTE & (cpu->registers[operand_1] * cpu->registers[operand_2]);
    cpu->registers[operand_1] = result;
}

//-- DIV: Store quotient of reg.A and B in reg.A -----
// operations[0xa3] = op_a3_DIV;

//-- MOD: Store modulus of reg.A and B in reg.A ------
// operations[0xa4] = op_a4_MOD;

//-- CMP: Compare registerA and registerB ------------
void op_a7_CMP(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    if(cpu->registers[operand_1] == cpu->registers[operand_2])
    {
        cpu->FL = CMP_FLAG_EQUAL; 
    }
    else if(cpu->registers[operand_1] >= cpu->registers[operand_2])
    {
        cpu->FL = CMP_FLAG_GREATER;
    }
    else if(cpu->registers[operand_1] <= cpu->registers[operand_2])
    {
        cpu->FL = CMP_FLAG_LESS;
    }
}

//-- AND: Bitwise AND on registers -------------------
// operations[0xa8] = op_a8_AND;

//-- OR: Bitwise OR on registers ---------------------
// operations[0xaa] = op_aa_OR;

//-- XOR: Bitwise XOR on registers -------------------
// operations[0xab] = op_ab_XOR;

//-- SHL: Left shift bits on reg.A by reg.B ----------
void op_ac_SHL(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    cpu->registers[operand_1] = BYTE & (cpu->registers[operand_1] << cpu->registers[operand_2]);
}

//-- SHR: Right shift bits on reg.A by reg.B ---------
// operations[0xad] = op_ad_SHR;
