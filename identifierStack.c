// Project 4 - Code Generation
// identifierStack.c
// Tan Nguyen
// ntnhmc@umsystem.edu
// Apr 18, 2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "identifierStack.h"

//array of tokens 
token stack[MAX_STACK_SIZE];
static int stackVars = 0;

//Init Stack
void stackInit(){
	int i;
	for(i = 0; i < MAX_STACK_SIZE + 1; i++){
		stack[i].instance = " ";
	}
}


//Push function
void push(token stack_tk){
	int i;
	for(i = 0; i < MAX_STACK_SIZE; i++){
		if(strcmp(stack[i].instance, stack_tk.instance) == 0){
			printf("ERROR: IDENTIFIER '%s': Redefining a variable found on line: %d (IDENTIFIER '%s' already define on line: %d)\n", stack_tk.instance,stack_tk.lineNum, stack[i].instance, stack[i].lineNum);
			exit(EXIT_FAILURE);
		}		
	}	
	stack[stackVars] = stack_tk;
	stackVars++;
}

//Verify
bool verify(token check){
	int i;
	for(i = 0; i < MAX_STACK_SIZE; i++){
		if(strcmp(stack[i].instance, check.instance) == 0){
			return true;
		}
	}
	return false;
}

//Print
void print_stack(){
	printf("The symbol table: IDENTIFIER: %s %s %s\n\n", stack[0].instance, stack[1].instance, stack[2].instance);

}

//Stop function
void stop(FILE *outFilePtr){
	fprintf(outFilePtr, "STOP \n");
	if(stack[0].instance != NULL){
		fprintf(outFilePtr, "%s %d\n", stack[0].instance, 0);
	}
	if(stack[1].instance != NULL){
		fprintf(outFilePtr, "%s %d\n", stack[1].instance, 0);
	}
	if(stack[2].instance != NULL){
		fprintf(outFilePtr, "%s %d\n", stack[2].instance, 0);
	}
}






