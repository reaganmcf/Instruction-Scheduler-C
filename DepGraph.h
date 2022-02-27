#ifndef DEPGRAPH_H
#define DEPGRAPH_H

#include "Instr.h"

typedef enum { TRUE, ANTI } DepType;

typedef struct __dg_edge DepGraphEdge;

typedef struct __dg_node DepGraphNode;

struct __dg_node {
  unsigned int id; // line which the ixn is in the original file, starting at 0
  Instruction *instruction;
  DepGraphEdge *edges;
};

struct __dg_edge {
  DepType type;
  unsigned int weight;
  DepGraphNode *from;

  DepGraphEdge *next;
};

/**
 * Build a Dependency Graph from a Doubly Linked list of Instructions
 */
DepGraphNode *BuildDepGraph(Instruction *);
DepGraphNode *CreateNode(Instruction *);
DepGraphEdge *GetDeps(Instruction *);
DepGraphEdge *CreateEdge(unsigned int, DepGraphNode *);
Instruction *GetDepRegisterInstruction(Instruction *, unsigned int);
Instruction *GetDepMemoryInstruction(Instruction *, unsigned int);

#endif
