#ifndef DEPGRAPH_H
#define DEPGRAPH_H

typedef enum {
	TRUE,
	ANTI
} EdgeType;

typedef struct __dg_edge DepGraphEdge;
struct __dg_edge {
	EdgeType type;
	unsigned int weight;
	Instruction* to;

	DepGraphEdge* next;
};

typedef struct __dg_node DepGraphNode;
struct __dg_node {
	unsigned int id; // line which the ixn is in the original file, starting at 0
	Instruction* instruction;
	DepGraphEdge* edges;
};

/**
 * Build a Dependency Graph from a Doubly Linked list of Instructions
 */
DepGraphNode *BuildDepGraph(Instruction * instructions);
DepGraphNode *CreateNode(DepGraphNode*, unsigned int, Instruction *);
#endif
