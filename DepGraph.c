#include "Instr.h"
#include "InstrUtils.h"
#include "DepGraph.h"
#include <stdlib.h>
#include <stdio.h>

DepGraphNode *BuildDepGraph(Instruction * instructions)
{
	DepGraphNode *root;

	Instruction *curr_ixn, *next;
       	curr_ixn = LastInstruction(instructions);

	unsigned int curr_id = 0;
	
	//while(curr_ixn != NULL) {
		printf("last instruction:\n");
		PrintInstruction(stdout,  curr_ixn);

		DepGraphNode* curr;
		CreateNode(curr, curr_id++, curr_ixn);
		printf("%d, %d, %d\n", curr_ixn->field1, curr_ixn->field2, curr_ixn->field3);

//		next = curr_ixn->prev;
//		curr_ixn = next;
//	}

	return root;
}


DepGraphNode *CreateNode(DepGraphNode* node, unsigned int id, Instruction * instruction)
{
	if(node == NULL) {
		perror("Can't create node with NULL node parameter");
		exit(1);
	}

	if (instruction == NULL) {
		perror("Can't create node with NULL instruction parameter");
		exit(1);
	}

	node->id = id;
	node->instruction = instruction;
	node->edges = NULL;

	return node;
}


