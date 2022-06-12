// Project 4 - Code Generation
// statSem.c
// Tan Nguyen
// ntnhmc@umsystem.edu
// May 4, 2022


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "statSem.h"
#include "identifierStack.h"



void semanticCheck(node_t *node){

	if(node == NULL){
		return;
	}
	
	if(strcmp(node->name, "<S>") == 0){
		//Name Identifier
		if(node->token1.type == ID_TK){
			push(node->token1);
		}
		//Spot Identifier
		if(node->token2.type == ID_TK){
			push(node->token2);
		}
		if(node->child1 != NULL){
			semanticCheck(node->child1);}
		if(node->child2 != NULL){
			semanticCheck(node->child2);}
	}
	else if(strcmp(node->name, "<R>") == 0){
		if(node->child1 != NULL){
			semanticCheck(node->child1);}
		if(node->child2 != NULL){
			semanticCheck(node->child2);}
	}
	else if(strcmp(node->name , "<A>") == 0){
		if(node->token1.type == ID_TK){
			push(node->token1);
		}	
	}
	else if(strcmp(node->name, "<E>") == 0){
		if(node->token1.type == ID_TK){
			if(!verify(node->token1)){
				printf("ERROR: IDENTIFIER '%s' On Line: %d Was Not Declared In The Global Scope.(Using an undefined variable)\n", node->token1.instance, node->token1.lineNum);
			}
		}
	}
	else if(strcmp(node->name, "<B>") == 0 || strcmp(node->name, "<C>") == 0 || strcmp(node->name, "<D>") == 0 || strcmp(node->name, "<F>") == 0 || strcmp(node->name, "<G>") == 0
		|| strcmp(node->name, "<T>") == 0 || strcmp(node->name, "<V>") == 0 || strcmp(node->name, "<H>") == 0 || strcmp(node->name, "<J>") == 0 || strcmp(node->name, "<K>") == 0
		|| strcmp(node->name, "<L>") == 0 || strcmp(node->name, "<W>") == 0 || strcmp(node->name, "<Z>") == 0){
		if(node->token1.type == ID_TK){
			if(!verify(node->token1)){
				printf("ERROR: IDENTIFIER '%s' On Line: %d Was Not Declared In The Global Scope.(Using an undefined variable)\n", node->token1.instance, node->token1.lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else if(node->token2.type == ID_TK){
			if(!verify(node->token2)){
				printf("ERROR: IDENTIFIER '%s' On Line: %d Was Not Declared In The Global Scope.(Using an undefined variable)\n", node->token2.instance, node->token2.lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else if(node->token3.type == ID_TK){
			if(!verify(node->token3)){
				printf("ERROR: IDENTIFIER '%s' On Line: %d Was Not Declared In The Global Scope.(Using an undefined variable)\n", node->token3.instance, node->token3.lineNum);
				exit(EXIT_FAILURE);
			}
		}
		if(node->child1 != NULL){
			semanticCheck(node->child1);}
		if(node->child2 != NULL){
			semanticCheck(node->child2);}
		if(node->child3 != NULL){
			semanticCheck(node->child3);}
		if(node->child4 != NULL){
			semanticCheck(node->child4);}
	}
	else{
		if(node->child1 != NULL){
			semanticCheck(node->child1);}
		if(node->child2 != NULL){
			semanticCheck(node->child2);}
		if(node->child3 != NULL){
			semanticCheck(node->child3);}
		if(node->child4 != NULL){
			semanticCheck(node->child4);}
	}
	
}
