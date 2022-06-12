// Project 4 - Code Generation
// node.h
// Tan Nguyen
// ntnhmc@umsystem.edu
// May 4, 2022

#ifndef NODE_H
#define NODE_H

#include "token.h"
#define LENGTH 50 //max length of name					

typedef struct node_t {

	char name[LENGTH];
	int level;

	token token1;
	token token2;
	token token3;
	
	struct node_t *child1;
	struct node_t *child2;
	struct node_t *child3;
	struct node_t *child4;
} node_t;


#endif 
