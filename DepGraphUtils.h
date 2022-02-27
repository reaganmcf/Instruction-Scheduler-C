#ifndef DEPGRAPH_UTILS_H
#define DEPGRAPH_UTILS_H

#include "DepGraph.h"

void PrintDepGraphNode(DepGraphNode *node, unsigned int level);
void PrintDepGraph(DepGraph *graph);
unsigned int InstructionIsInGraph(DepGraph *graph, Instruction *instruction);
unsigned int InstructionIsInGraphNode(DepGraphNode *node, Instruction *instruction);
#endif
