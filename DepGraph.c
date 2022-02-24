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

  DepGraphEdge *edges = NULL;

  // TODO: anti deps
  if (instruction->opcode == ADD) {
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
  } else if (instruction->opcode == LOADI) {
    return NULL;
  } else {
    ERROR("opcode %d not yet implemented", instruction->opcode);
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
    } else if (curr->opcode == ADD) {
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
