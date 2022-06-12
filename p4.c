// Project 4 - Code Generation
// p4.c
// Tan Nguyen
// ntnhmc@umsystem.edu
// May 4, 2022


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "scanner.h"
#include "testTree.h"
#include "parser.h"
#include "node.h"
#include "statSem.h"
#include "identifierStack.h"
#include "generator.h"

void help();  // display information about how to run program

int main(int argc, char **argv) {

	// ERROR if there are more than 2 arguments
	if (argc > 2) {
		printf("Error: Expected 1 or no arguments\n");
		printf("Try './frontEnd -h' for more information.\n");
		return EXIT_FAILURE;
	}

	int opt;
        while((opt = getopt(argc, argv, "h")) != -1){
        	switch(opt)
        	{
			case 'h':
                		help();
                		exit(EXIT_SUCCESS);
                		break;
			case '?':
				printf("Unknown option: %c\n", opt);
                		break;
            		default:
                		perror("oss.c: error: invalid argument");
                		help();                		
				break;
       		}
        }

   	FILE *inFilePtr = NULL;
	FILE *outFilePtr = NULL;
    	char *fileName = NULL;
	const char outFileExt[] = ".asm";  //Target file extension
	node_t *tree;

	//Read from file if argument = 2 and Read from the keyboard
	if (argc == 2) {
        	const char fileExt[] = ".txt";
        	fileName = (char *) malloc(sizeof(argv[1]));
        	strcpy(fileName, argv[1]);
        	char inFile[strlen(fileName) + strlen(fileExt)];	// add ".txt" into fileName : fileName.txt
        	strcpy(inFile, fileName);
        	strcat(inFile, fileExt);

		inFilePtr = fopen(inFile, "r");
		
		// if file error, abort with appropriate message 
		if (!inFilePtr) {
			printf("ERROR - P4: File %s could not be opened to READ\n", inFile);
			return (EXIT_FAILURE);
		}

		if(!feof(inFilePtr)){
			tree = parser(inFilePtr);
		}
		printf("Semantic Check Started\n");
		stackInit();
		semanticCheck(tree);
 	  	printf("Semantic Check Success!\n");
		print_stack();

		//Open file and write
		char outFile[strlen(fileName) + strlen(outFileExt)]; // add ".asm" into outFile | outFile = fileName.asm
    		strcpy(outFile, fileName);
    		strcat(outFile, outFileExt);
    		free(fileName); // Remove allocated memory storage
    		outFilePtr = fopen(outFile, "w"); // Open outFile for writing permission and assign to outFilePtr
		
		if (!outFilePtr) {
        		printf("ERROR - P4: File '%s' could not be opened to WRITE\n", outFile);
        		return (EXIT_FAILURE);
    		}
		generator(tree, outFilePtr);
		stop(outFilePtr);
		temp_stop(outFilePtr);
		printf("Code Generator Success!\n");
		preOrderPrint(tree);
	}
	//Read from Keyboard until EOF, the CTRL + D
	else {
		inFilePtr = stdin;
		const char outFileName[] = "out";
		strcpy(fileName, outFileName);
		if (inFilePtr == NULL){
			fprintf(stderr, "Unable To Start stdin: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		if(!feof(inFilePtr)){
			tree = parser(inFilePtr);
		}
		printf("Semantic Check Started\n");
		stackInit();
		semanticCheck(tree);
 	  	printf("Semantic Check Success!\n");
		print_stack();
		
		//Open file and Write
		char outFile[strlen(fileName) + strlen(outFileExt)]; // add ".asm" into outFile | outFile = out.asm
    		strcpy(outFile, fileName);
    		strcat(outFile, outFileExt);
    		free(fileName); // Remove allocated memory storage
    		outFilePtr = fopen(outFile, "w"); // Open outFile for writing permission and assign to outFilePtr
		
		if (!outFilePtr) {
        		printf("ERROR - P4: File '%s' could not be opened to WRITE\n", outFile);
        		return (EXIT_FAILURE);
    		}
		generator(tree, outFilePtr);
		stop(outFilePtr);
		temp_stop(outFilePtr);
		printf("Code Generator Success!\n");
		
		preOrderPrint(tree);
	}

	//testScanner(inFilePtr);
    	fclose(inFilePtr);
	fclose(outFilePtr);
	free(fileName);
	return 0;
}

//Print how to run program
void help(){
	printf("\n------------------------------------------------------------------------------------\n");
	
	printf("How to implement program::  \n");
	printf("Invocation:  ./genCode [file]  \n");
	printf("There are three ways to run the program: \n");
	printf("1. ./genCode to prompt user to enter from keyboard. \n");
	printf("2. ./genCode < [filename] to direct inout file to scanner exectuable program. \n");
	printf("3. ./genCode [filename] // without .input suffix in file name. \n");

	printf("--------------------------------------------------------------------------------------\n\n");

}
