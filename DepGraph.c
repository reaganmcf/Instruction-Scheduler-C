#include "DepGraph.h"
#include "DepGraphUtils.h"
#include "Instr.h"
#include "InstrUtils.h"
#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>

DepGraph *BuildDepGraph(Instruction *instruction) {
  DepGraph *graph = calloc(1, sizeof(DepGraph));
  graph->nodes = calloc(50, sizeof(DepGraphNode *));
  graph->count = 0;

  Instruction *curr = instruction;
  while (curr != NULL) {
    if (!InstructionIsInGraph(graph, curr)) {
      graph->nodes[graph->count++] = BuildDepGraphNode(curr);
    }

    curr = curr->prev;
  }

  return graph;
}

DepGraphNode *BuildDepGraphNode(Instruction *instruction) {
  if (instruction == NULL)
    return NULL;
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

  node->instruction = instruction;
  node->edges = NULL;

  return node;
}

DepGraphEdge *GetDeps(Instruction *instruction) {
  if (instruction == NULL) {
    ERROR("instruction param cannot be null");
    exit(1);
  }

  // PrintInstruction(stdout, instruction);

  DepGraphEdge *edges = NULL;

  // TODO: anti deps

  OpCode op = instruction->opcode;
  /** ADD, MUL, SUB **/
  if (op == ADD || op == MUL || op == SUB) {
    Instruction *dep1 = GetRegisterDeps(instruction, instruction->field1);
    Instruction *dep2 = GetRegisterDeps(instruction, instruction->field2);

    if (dep1 == NULL || dep2 == NULL) {
      ERROR("ADD,SUB,MUL cannot have null register deps!");
      exit(1);
    }

    DepGraphEdge *edge1 = CreateEdge(0, BuildDepGraphNode(dep1));
    DepGraphEdge *edge2 = CreateEdge(0, BuildDepGraphNode(dep2));

    if (dep1->id == dep2->id) {
      // same dep only gets added once not twice
      edges = edge1;
    } else {
      edge1->next = edge2;
      edges = edge1;
    }

    /** LOADI **/
  } else if (op == LOADI) {
    return NULL;

    /** OUTPUTAI **/
  } else if (op == OUTPUTAI) {
    Instruction *dep = GetMemoryDeps(instruction, instruction->field2);
    DepGraphEdge *storeDeps = GetStoreDeps(0, instruction);
    if (dep == NULL) return storeDeps;
    edges = CombineDeps(CreateEdge(0, BuildDepGraphNode(dep)), storeDeps);

    /** STOREAI **/
  } else if (op == STOREAI) {
    Instruction *dep = GetRegisterDeps(instruction, instruction->field1);
    DepGraphEdge *storeDeps = GetStoreDeps(0, instruction);
    DepGraphEdge *loadDeps = GetLoadDeps(0, instruction);

    DepGraphEdge *loadAndStoreDeps = CombineDepsUnchecked(loadDeps, storeDeps);

    if (dep == NULL) {
      ERROR("STOREAI cannot have null memory deps!");
      exit(1);
    }

    edges = CombineDeps(CreateEdge(0, BuildDepGraphNode(dep)), loadAndStoreDeps);

    /** LOADAI **/
  } else if (op == LOADAI) {
    Instruction *dep = GetMemoryDeps(instruction, instruction->field2);
    DepGraphEdge *storeDeps = GetStoreDeps(0, instruction);

    if (dep == NULL) {
      // LOADAI can have optionally null deps
      // For example:
      //  loadI 1024 => r0
      //  loadAI r0, 4 => r1
      //
      // Here, r0, 4 has no dep (since loadI 1024 => r0 is the dep for all
      // instructions)
      return storeDeps;
    }

    edges = CombineDeps(CreateEdge(0, BuildDepGraphNode(dep)), storeDeps);

    /** STORE **/
  } else if (op == STORE) {
    // Store depends on field1
    // as well as all previous memory operations (besides loadI)
    Instruction *dep = GetRegisterDeps(instruction, instruction->field1);
    DepGraphEdge *loadDeps = GetLoadDeps(0, instruction);
    DepGraphEdge *storeDeps = GetStoreDeps(0, instruction);

    DepGraphEdge *loadAndStoreDeps = CombineDepsUnchecked(loadDeps, storeDeps);

    if (dep == NULL) {
      ERROR("STORE cannot have null memory deps!");
      exit(1);
    }

    edges = CombineDeps(CreateEdge(0, BuildDepGraphNode(dep)), loadAndStoreDeps);

    /** LOAD  **/
  } else if (op == LOAD) {
    Instruction *dep = GetRegisterDeps(instruction, instruction->field1);
    DepGraphEdge *storeDeps = GetStoreDeps(0, instruction);

    if (dep == NULL) {
      ERROR("LOAD can't have null dep");
      exit(1);
    }

    edges = CombineDeps(CreateEdge(0, BuildDepGraphNode(dep)), storeDeps);
  } else {
    ERROR("opcode %d not yet implemented", op);
    exit(1);
  }

  return edges;
}

Instruction *GetRegisterDeps(Instruction *instruction,
                             unsigned int register_num) {
  Instruction *dep = NULL;
  if (instruction == NULL) {
    ERROR("instruction param cannot be NULL");
    exit(1);
  }

  // TODO: anti deps

  Instruction *curr = instruction->prev;
  DEBUG("Finding dep register for %d", register_num);
  PrintInstruction(stdout, curr);
  while (curr != NULL && dep == NULL) {
    if (curr->opcode == LOADI) {
      if (curr->field2 == register_num)
        dep = curr;

      /** ADD, MUL, LOADAI, SUB **/
    } else if (curr->opcode == ADD || curr->opcode == MUL ||
               curr->opcode == LOADAI || curr->opcode == SUB) {
      if (curr->field3 == register_num)
        dep = curr;

      /** LOAD **/
    } else if (curr->opcode == LOAD) {
      if (curr->field2 == register_num)
        dep = curr;
    } else {
      ERROR("opcode %d not yet implemented", curr->opcode);
      exit(1);
    }

    curr = curr->prev;
  }
  return dep;
}

Instruction *GetMemoryDeps(Instruction *instruction,
                           unsigned int memory_location) {
  Instruction *dep = NULL;
  if (instruction == NULL) {
    ERROR("instruction param cannot be NULL");
  }

  // TODO: anti deps

  DEBUG("Finding dep memory for %d", memory_location);
  Instruction *curr = instruction->prev;
  while (curr != NULL && dep == NULL) {
    PrintInstruction(stdout, curr);
    if (curr->opcode != STOREAI && curr->opcode != LOADAI &&
        curr->opcode != OUTPUTAI) {
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

DepGraphEdge *GetStoreDeps(unsigned int weight, Instruction *instruction) {
  // All memory instructions also depend on all previous STORE operations
  if (instruction == NULL) {
    ERROR("instruction param cannot be NULL");
    exit(1);
  }

  DepGraphEdge *edges = NULL;

  Instruction *curr = instruction->prev;
  DepGraphEdge *head = edges;
  while (curr != NULL) {
    if (curr->opcode == STORE) {
      DepGraphEdge *newEdge = CreateEdge(0, BuildDepGraphNode(curr));
      head = newEdge;
      if (edges == NULL) {
        edges = head;
      }
      head = newEdge->next;
    }
    curr = curr->prev;
  }

  return edges;
}

DepGraphEdge *GetLoadDeps(unsigned int weight, Instruction *instruction) {
  // All memory instructions also depend on all previous STORE operations
  if (instruction == NULL) {
    ERROR("instruction param cannot be NULL");
    exit(1);
  }

  DepGraphEdge *edges = NULL;

  Instruction *curr = instruction->prev;
  DepGraphEdge *head = edges;
  while (curr != NULL) {
    if (curr->opcode == LOAD) {
      DepGraphEdge *newEdge = CreateEdge(0, BuildDepGraphNode(curr));
      head = newEdge;
      if (edges == NULL) {
        edges = head;
      }
      head = newEdge->next;
    }
    curr = curr->prev;
  }

  return edges;
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
