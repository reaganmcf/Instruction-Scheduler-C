#ifndef DEPGRAPH_H
#define DEPGRAPH_H

#include "Instr.h"
#include <unistd.h>

typedef enum { TRUE, ANTI } DepType;

typedef struct __dg_edge DepGraphEdge;
typedef struct __dg_node DepGraphNode;
typedef struct __dg_graph DepGraph;

struct __dg_node {
  Instruction *instruction;
  DepGraphEdge *edges;
};

struct __dg_edge {
  DepType type;
  unsigned int weight;
  DepGraphNode *from;

  DepGraphEdge *next;
};

struct __dg_graph {
  DepGraphNode **nodes;
  size_t count;
};

/**
 * Build a Dependency Graph from a Doubly Linked list of Instructions
 */
DepGraph *BuildDepGraph(Instruction *);
DepGraphNode *BuildDepGraphNode(Instruction *);
DepGraphNode *CreateNode(Instruction *);
DepGraphEdge *GetDeps(Instruction *);
DepGraphEdge *CreateEdge(unsigned int, DepGraphNode *);
Instruction *GetDepRegisterInstruction(Instruction *, unsigned int);
Instruction *GetDepMemoryInstruction(Instruction *, unsigned int);

#endif
