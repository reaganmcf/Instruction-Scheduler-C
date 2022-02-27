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

typedef enum {
  LOADI,    // 0
  LOADAI,   // 1
  LOAD,     // 2
  STOREAI,  // 3
  STORE,    // 4
  ADD,      // 5
  SUB,      // 6
  MUL,      // 7
  DIV,      // 8
  LSHIFTI,  // 9
  RSHIFTI,  // 10
  OUTPUTAI, // 11
} OpCode;

typedef struct InstructionInfo Instruction;

struct InstructionInfo {
  unsigned int id;
  OpCode opcode;
  int field1;
  int field2;
  int field3;
  Instruction *prev; /* previous instruction */
  Instruction *next; /* next instruction */
};

#endif
