#include "DepGraphUtils.h"
#include "InstrUtils.h"
#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>

void PrintDepGraph(DepGraph *graph) {
  if (graph == NULL) {
    ERROR("graph param cannot be NULL");
    exit(1);
  }

  for(int i = 0; i < graph->count; i++) {
    if (graph->nodes[i] == NULL) {
      ERROR("graph->nodes[i] is NULL - this should never happen!");
      exit(1);
    }
    PrintDepGraphNode(graph->nodes[i], 0);
  }
}

void PrintDepGraphNode(DepGraphNode *node, unsigned int level) {
  if (node == NULL) {
    ERROR("node param cannot be NULL");
    exit(1);
  }

  if (level == 0)
    PrintInstruction(stdout, node->instruction);

  DepGraphEdge *edge = node->edges;
  while (edge != NULL) {
    for (int j = 0; j < level + 1; j++)
      printf("   ");
    printf("->(TRUE):");
    PrintInstruction(stdout, edge->from->instruction);
    PrintDepGraphNode(edge->from, level + 2);

    edge = edge->next;
  }
}

unsigned int InstructionIsInGraph(DepGraph *graph, Instruction *instruction) {
  if (graph == NULL) {
    ERROR("graph param cannot be NULL");
    exit(1);
  }

  if (graph->nodes == NULL) {
    ERROR("graph->nodes cannot be NULL");
    exit(1);
  }

  if (instruction == NULL) {
    ERROR("instruction param cannot be NULL");
    exit(1);
  }

  for(int i = 0; i < graph->count; i++) {
    if(graph->nodes[i] == NULL) {
      ERROR("graph->nodes[i] is NULL - this should never happen!");
      exit(1);
    }

    if(InstructionIsInGraphNode(graph->nodes[i], instruction)) {
      return 1;
    }
  }

  return 0;
}

unsigned int InstructionIsInGraphNode(DepGraphNode *node, Instruction *instruction) {
  if (node == NULL) {
    ERROR("node param cannot be NULL");
    exit(1);
  }

  if (instruction == NULL) {
    ERROR("instruction param cannot be NULL");
    exit(1);
  }

  if(node->instruction == NULL) {
    ERROR("node->instruction param cannot be NULL");
    exit(1);
  }

  if (node->instruction->id == instruction->id)
    return 1;
  
  DepGraphEdge *edge = node->edges;
  while (edge != NULL) {
    if (InstructionIsInGraphNode(edge->from, instruction))
      return 1;
  
    edge = edge->next;
  }

  return 0;
}

DepGraphEdge* CombineDeps(DepGraphEdge* a, DepGraphEdge *b) {
  DepGraphEdge* combined = NULL;
  if (a == NULL && b == NULL) {
    ERROR("a and b are both NULL - this is not possible!");
    exit(1);
  } else if (a == NULL) {
    combined = b;
  } else if (b == NULL) {
    combined = a;
  } else {
    combined = a;
    combined->next = b;
  }

  return combined;
}

DepGraphEdge* CombineDepsUnchecked(DepGraphEdge* a, DepGraphEdge *b) {
  if (a == NULL && b == NULL) return NULL;
  return CombineDeps(a,b);
}
