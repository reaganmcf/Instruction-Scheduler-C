#include "DepGraphUtils.h"
#include "InstrUtils.h"
#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>

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
