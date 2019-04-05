

//== Operations (Op Codes) ====================================================
/*
    This module defines functions to handle the instructions defined by the LS8
    specification. It also provides functions to place those handlers into
    arrays, indexed by op code.
*/

//-- Run Once (see end of file) ----------------------
#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

//-- Dependencies ------------------------------------
#include "cpu.h"


//== Operation Array Memory Management ========================================
/*
    This section defines some variables to be exported, and two functions to
    allocate and free memory for those variables.
    
    The variables are arrays of pointers to functions used to handle CPU or ALU
    instructions (op codes).
    
    The function load_operations will also load all function pointers into the
    arrays.
*/

//-- Variables to be exported ------------------------
typedef void (*operation)(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
extern operation *operations_cpu;
extern operation *operations_cpu_jump;
extern operation *operations_alu;

//-- Memory freeing function -------------------------
void unload_operations();

//-- Memory allocation and setup function ------------
void load_operations();


//== Operation Handler Declarations ===========================================
/*
    The following are functions handlers for each of the instructions in the
    LS8 instruction set.
    
    All the handlers have the same signature, but not all of them use both
    operands. Where an operand is unused, any unsigned char can be passed, but
    a 0 value should be used to avoid confusion.
*/

void op_00_NOP(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_01_HLT(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_11_RET(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_13_IRET(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_45_PUSH(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_46_POP(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_47_PRN(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_48_PRA(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_50_CALL(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_52_INT(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_54_JMP(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_55_JEQ(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_56_JNE(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_57_JGT(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_58_JLT(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_59_JLE(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_5a_JGE(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_65_INC(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_66_DEC(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_69_NOT(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_82_LDI(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_83_LD(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_84_ST(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_a0_ADD(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_a1_SUB(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_a2_MUL(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_a3_DIV(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_a4_MOD(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_a7_CMP(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_a8_AND(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_aa_OR(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_ab_XOR(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_ac_SHL(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);
void op_ad_SHR(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);

//-- Run Once ----------------------------------------
#endif
