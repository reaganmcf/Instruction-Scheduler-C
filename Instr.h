/*
 *********************************************
 *  415 Compilers                            *
 *  Spring 2022                              *
 *  Students                                 *
 *  Authors: Ulrich Kremer                   *
 *********************************************
 */

#ifndef INSTR_H
#define INSTR_H

typedef enum {LOADI, LOADAI, LOADAO, STOREAI, STOREAO, ADD, SUB, MUL, DIV, LSHIFTI, RSHIFTI, OUTPUTAI} OpCode;

typedef struct InstructionInfo Instruction;

struct InstructionInfo {
	OpCode opcode;
	int field1;
	int field2;
	int field3;
	Instruction *prev;	/* previous instruction */
	Instruction *next;	/* next instruction */
};

#endif
