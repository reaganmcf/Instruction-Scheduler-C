#include "DepGraph.h"
#include "DepGraphUtils.h"
#include "Instr.h"
#include "InstrUtils.h"
#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int id;

DepGraphNode *BuildDepGraph(Instruction *instruction) {
  DepGraphEdge *edges = GetDeps(instruction);
  DepGraphNode *root = CreateNode(instruction);
  root->edges = edges;

  return root;
}

DepGraphNode *CreateNode(Instruction *instruction) {
  if (instruction == NULL) {
    ERROR("instruction param cannot be NULL");
    exit(1);
  }

  DepGraphNode *node = calloc(1, sizeof(DepGraphNode));

  node->id = id++;
  node->instruction = instruction;
  node->edges = NULL;

  return node;
}

DepGraphEdge *GetDeps(Instruction *instruction) {
  if (instruction == NULL) {
    ERROR("instruction param cannot be null");
    exit(1);
  }

  PrintInstruction(stdout,instruction);

  DepGraphEdge *edges = NULL;

  // TODO: anti deps

  OpCode op = instruction->opcode;
  /** ADD, MUL, SUB **/
  if (op == ADD || op == MUL || op == SUB) {
    Instruction *dep1 =
        GetDepRegisterInstruction(instruction, instruction->field1);
    Instruction *dep2 =
        GetDepRegisterInstruction(instruction, instruction->field2);

    if (dep1 == NULL || dep2 == NULL) {
      ERROR("ADD cannot have null register deps!");
      exit(1);
    }

    DepGraphEdge *edge1 = CreateEdge(instruction->field1, BuildDepGraph(dep1));
    DepGraphEdge *edge2 = CreateEdge(instruction->field2, BuildDepGraph(dep2));

    edge1->next = edge2;
    edges = edge1;

  /** LOADI **/
  } else if (op == LOADI) {
    return NULL;

  /** OUTPUTAI **/
  } else if (op == OUTPUTAI) {
    Instruction *dep = GetDepMemoryInstruction(instruction, instruction->field2);

    if (dep == NULL) {
      ERROR("OUTPUTAI cannot have null memory deps!");
      exit(1);
    }

    DepGraphEdge *edge1 = CreateEdge(instruction->field1, BuildDepGraph(dep));

    edges = edge1;
  
  /** STOREAI **/
  } else if (op == STOREAI) {
    Instruction *dep = GetDepRegisterInstruction(instruction, instruction->field1);

    if (dep == NULL) {
      ERROR("STOREAI cannot have null memory deps!");
      exit(1);
    }

    DepGraphEdge *edge1 = CreateEdge(instruction->field1, BuildDepGraph(dep));

    edges = edge1;

  /** LOADAI **/
  } else if(op == LOADAI) {
    Instruction *dep = GetDepMemoryInstruction(instruction, instruction->field2);

    if (dep == NULL) {
      ERROR("LOADAI cannot have null memory deps!");
      exit(1);
    }

    DepGraphEdge *edge1 = CreateEdge(instruction->field2, BuildDepGraph(dep));

    edges = edge1;
  } else {
    ERROR("opcode %d not yet implemented", op);
    exit(1);
  }

  return edges;
}

Instruction *GetDepRegisterInstruction(Instruction *instruction,
                                       unsigned int register_num) {
  Instruction *dep = NULL;
  if (instruction == NULL) {
    ERROR("instruction param cannot be NULL");
    exit(1);
  }

  // TODO: anti deps

  Instruction *curr = instruction->prev;
  //DEBUG("Finding dep register for %d", register_num);
  while (curr != NULL && dep == NULL) {
    if (curr->opcode == LOADI) {
      if (curr->field2 == register_num)
        dep = curr;

    /** ADD, MUL, LOADAI, SUB **/
    } else if (curr->opcode == ADD || curr->opcode == MUL || curr->opcode == LOADAI || curr->opcode == SUB) {
      if (curr->field3 == register_num)
        dep = curr;
    } else {
      ERROR("opcode %d not yet implemented", curr->opcode);
      exit(1);
    }

    curr = curr->prev;
  }
  return dep;
}

Instruction *GetDepMemoryInstruction(Instruction * instruction, unsigned int memory_location) {
  Instruction *dep = NULL;
  if (instruction == NULL) {
    ERROR("instruction param cannot be NULL");
  }

  // TODO: anti deps
  
  DEBUG("Finding dep memory for %d", memory_location);
  Instruction *curr = instruction->prev;
  while(curr != NULL && dep == NULL) {
    PrintInstruction(stdout, curr);
    if (curr->opcode != STOREAI && curr->opcode != LOADAI && curr->opcode != OUTPUTAI) {
      // NOT A MEMORY INSTRUCTION -- SKIP!

    /** STOREAI **/
    } else if (curr->opcode == STOREAI) {
      if (curr->field3 == memory_location)
        dep = curr;
    /** LOADAI **/
    } else if (curr->opcode == LOADAI) {
      if (curr->field2 == memory_location)
        dep = curr;
    } else {
      ERROR("opcode %d not yet implemented", curr->opcode);
      exit(1);
    }

    curr = curr->prev;
  }
  return dep;
}

DepGraphEdge *CreateEdge(unsigned int weight, DepGraphNode *from) {
  if (from == NULL) {
    ERROR("from parameter cannot be null");
    exit(1);
  }

  DepGraphEdge *edge = calloc(1, sizeof(DepGraphEdge));

  edge->weight = weight;
  edge->from = from;
  edge->next = NULL;

  // TODO: anti deps
  edge->type = TRUE;

  return edge;
}
