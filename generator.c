// Project 4 - Code Generation
// generator.c
// Tan Nguyen
// ntnhmc@umsystem.edu
// May 4, 2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "testTree.h"
#include "generator.h"
#include "token.h"
#include "node.h"
#include "identifierStack.h"
#include "statSem.h"
#include "parser.h"

static int count = 1; //Count TEMP Variable
static char *digit_assign; //DIGIT for ASSIGN IDENTIFIER NUMBER

void generator(node_t *node, FILE *outFilePtr){
	//printf("check_e %d check_j %d check_l %d operator_t %d operator_c %d\n", check_e, check_j, check_l, operator_t, operator_v);	
	if(node == NULL){
		return;
	}
	//<S>      ->     Name Identifier Spot Identifier <R> <E>  
	if(strcmp(node->name, "<S>") == 0){
		//'Name identifier’ allocates memory for given identifier and initializes its value to zero 
		if(node->token1.type == ID_TK){
			fprintf(outFilePtr, "LOAD %d\n", 0);
			fprintf(outFilePtr, "STORE %s\n", node->token1.instance);
		}
		//‘Spot identifier’ means to read in an integer from the user and store the value for the given new identifier 
		if(node->token2.type == ID_TK){
			fprintf(outFilePtr, "READ %s\n", node->token2.instance);
		}
		if(node->child1 != NULL){
			generator(node->child1, outFilePtr);}
		if(node->child2 != NULL){
			generator(node->child2, outFilePtr);}
	}
	//<R>     ->     Place <A> <B> Home 
	else if(strcmp(node->name, "<R>") == 0){
		if(node->child1 != NULL){
			generator(node->child1, outFilePtr);}
		if(node->child2 != NULL){
			generator(node->child2, outFilePtr);}
	}
	//<E>     ->     Show Identifier 
	else if(strcmp(node->name , "<E>") == 0){
		//‘Show identifier’ means to write out the value of the given identifier to the monitor
		if(node->token1.type == ID_TK){
			fprintf(outFilePtr, "WRITE %s\n", node->token1.instance);
		}
	}
	//<A>     ->    Name Identifier  
	else if(strcmp(node->name , "<A>") == 0){
		//'Name identifier’ allocates memory for given identifier and initializes its value to zero 
		if(node->token1.type == ID_TK){
			fprintf(outFilePtr, "LOAD %d\n", 0);
			fprintf(outFilePtr, "STORE %s\n", node->token1.instance);
		}
	}
	//<B>     ->    empty |  .  <C> . <B> | <D> <B>  
	else if(strcmp(node->name, "<B>") == 0){
		//In case . <C> . <B>
		if(node->token1.type == DOTS_TK){
			generator(node->child1, outFilePtr);
			generator(node->child2, outFilePtr);
		}
		//In case <D> <B>
		//<H> <B>
		else if(node->token1.type == DIVISION_TK){
			generator(node->child1, outFilePtr);
			generator(node->child2, outFilePtr);
		}
		//<J> <B>
		else if(node->token1.type == ID_TK && check_j == 3){
			generator(node->child1, outFilePtr);
			//fprintf(outFilePtr, "LOAD %s\n", digit_assign);
			fprintf(outFilePtr, "STORE %s\n", node->token1.instance);
			generator(node->child2, outFilePtr);
			
		}
		//<K><B>
		else if(node->token1.type == DIGIT_TK &&  node->token1.type == DIGIT_TK){
			//‘Spot number’ means to load the immediate number into the ACCumulator 
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
			//						
			//‘Show number’ means to write out the Number to the monitor 
			fprintf(outFilePtr, "WRITE %s\n", node->token2.instance);
			generator(node->child1, outFilePtr);	
		}
		else if(node->token1.type == ID_TK && node->token2.type == ID_TK){
			//‘Move identifier’ means to load the given identifier’s value into the ACCumulator 
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
			//						
			//‘Show identifier’ means to write out the value of the given identifier to the monitor 
			fprintf(outFilePtr, "WRITE %s\n", node->token2.instance);
			generator(node->child1, outFilePtr);
		}
		//<L><B> || <E><B>
		else if(node->token1.type == ID_TK && check_l == 1){
			//‘Flip identifier’ means to multiply the value for the given identifier by -1 and store the value  	
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
			fprintf(outFilePtr, "MULT %d\n", -1);
			fprintf(outFilePtr, "STORE %s\n", node->token1.instance);
			generator(node->child1, outFilePtr);
		}
		else if(node->token1.type == ID_TK && check_e == 2){
			//‘Show identifier’ means to write out the value of the given identifier to the monitor
			fprintf(outFilePtr, "WRITE %s\n", node->token1.instance);
			generator(node->child1, outFilePtr);
		}
		//<F> <B>
		else if(node->token1.type == BRACKET_RIGHT_TK && node->token2.type == ID_TK){
			//IF IDENTIFIER
			char *TEMP = node->token2.instance;
			//CALL <T>
			generator(node->child1, outFilePtr);
			int check_operator_t = operator_t;
			//CALL <W>
			generator(node->child2, outFilePtr);
	
			fprintf(outFilePtr, "STORE TEMP%d\n", count);
			fprintf(outFilePtr, "LOAD %s\n", TEMP);
			if(check_operator_t == 1){
				fprintf(outFilePtr, "SUB TEMP%d\n", count);
				fprintf(outFilePtr, "BRNEG DONE%d\n", count);
			}
			else if(check_operator_t == 2){
				fprintf(outFilePtr, "SUB TEMP%d\n", count);
				fprintf(outFilePtr, "BRZPOS DONE%d\n",count );
			}
			// DO <D>
			generator(node->child3, outFilePtr);
			fprintf(outFilePtr, "DONE%d: NOOP\n", count);
			count++;
		}
		else if(node->token1.type == BRACKET_RIGHT_TK){
			fprintf(outFilePtr, "REPEAT%d: NOOP\n", count);
			//DO <D> 
			generator(node->child1, outFilePtr);
			generator(node->child2, outFilePtr);
			int check_operator_t = operator_t;
			generator(node->child3, outFilePtr);
			
			if(check_operator_t == 1){
				fprintf(outFilePtr, "BRNEG REPEAT%d\n", count);
			}
			else if(check_operator_t == 2){
				fprintf(outFilePtr, "BRZPOS REPEAT%d\n",count);
			}
			count++;			
		}
		else{
			return;
		}	
	
	}
	//<C>     ->    <F> | <G> 
	else if(strcmp(node->name, "<C>") == 0){
		//In case <G>
		if(node->token1.type == DIGIT_TK){
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
			fprintf(outFilePtr, "STORE TEMP%d\n", count);
			fprintf(outFilePtr, "BRZNEG Done%d\n", count);
			fprintf(outFilePtr, "Repeat%d: WRITE TEMP%d\n", count, count);
			fprintf(outFilePtr, "SUB %d\n", 1);
			fprintf(outFilePtr, "BRPOS Repeat%d\n", count);
			fprintf(outFilePtr, "Done%d: NOOP\n", count);
			count++;
		}
		//In case <F>
		//<F> -> IF IDENTIFIER <T><W><D>
		else if(node->token1.type == BRACKET_RIGHT_TK && node->token2.type == ID_TK){
			//IF IDENTIFIER
			char *TEMP = node->token2.instance;
			//CALL <T>
			generator(node->child1, outFilePtr);
			int check_operator_t = operator_t;
			//CALL <W>
			generator(node->child2, outFilePtr);
	
			fprintf(outFilePtr, "STORE TEMP%d\n", count);
			fprintf(outFilePtr, "LOAD %s\n", TEMP);
			if(check_operator_t == 1){
				fprintf(outFilePtr, "SUB TEMP%d\n", count);
				fprintf(outFilePtr, "BRNEG DONE%d\n", count);
			}
			else if(check_operator_t == 2){
				fprintf(outFilePtr, "SUB TEMP%d\n", count);
				fprintf(outFilePtr, "BRZPOS DONE%d\n",count );
			}
			// DO <D>
			generator(node->child3, outFilePtr);
			fprintf(outFilePtr, "DONE%d: NOOP\n", count);
			count++;
		}
		else if(node->token1.type == BRACKET_RIGHT_TK){
			fprintf(outFilePtr, "REPEAT%d: NOOP\n", count);
			// DO <D>
			generator(node->child1, outFilePtr);
			generator(node->child2, outFilePtr);
			int check_operator_t = operator_t;
			generator(node->child3, outFilePtr);
			
			if(check_operator_t == 1){
				fprintf(outFilePtr, "BRNEG REPEAT%d\n", count);
			}
			else if(check_operator_t == 2){
				fprintf(outFilePtr, "BRZPOS REPEAT%d\n",count);
			}
	
			count++;			
		}

	}
	//<D>     ->    <H> | <J> | <K> | <L>  | <E> | <F> 
	else if(strcmp(node->name, "<D>") == 0 ){
		//<H> -> / <Z> -> / IDENTIFIER | NUMBER
		if(node->token1.type == DIVISION_TK){	
			generator(node->child1, outFilePtr);
		}
		//<K> -> SPOT NUMBER SHOW NUMBER | MOVE ID SHOW ID
		else if(node->token1.type == DIGIT_TK && node->token2.type == DIGIT_TK){
			//‘Spot number’ means to load the immediate number into the ACCumulator 
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
			
			//‘Show number’ means to write out the Number to the monitor 
			fprintf(outFilePtr, "WRITE %s\n", node->token2.instance);	
	
		}
		else if(node->token1.type == ID_TK && node->token2.type == ID_TK){
			//‘Move identifier’ means to load the given identifier’s value into the ACCumulator 
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
			
			//‘Show identifier’ means to write out the value of the given identifier to the monitor 
			fprintf(outFilePtr, "WRITE %s\n", node->token2.instance);
				
		}
		//<L> -> FLIP IDENTIFIER or <E> ->SHOW IDENTIFIER
		//<J> -> ASSIGN IDENTIFIER NUMBER(NUMBER FROM <D>)
		else if(node->token1.type == ID_TK){
			if(check_l == 1){
				//'Flip identifier’ means to multiply the value for the given identifier by -1 and store the value  
				fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
				fprintf(outFilePtr, "MULT %d\n", -1);
				fprintf(outFilePtr, "STORE %s\n", node->token1.instance);
			}
			else if(check_e == 2){
				//‘Show identifier’ means to write out the value of the given identifier to the monitor
				fprintf(outFilePtr, "WRITE %s\n", node->token1.instance);
				
			}
			else if(check_j == 3){
				char *TEMP = node->token1.instance;
				generator(node->child1, outFilePtr);
				//fprintf(outFilePtr, "LOAD %s\n", digit_assign);
				fprintf(outFilePtr, "STORE %s\n", TEMP);	
			}
		}
		//<F> -> IF IDENTIFIER <T><W><D>
		else if(node->token1.type == BRACKET_RIGHT_TK && node->token2.type == ID_TK){
			//IF IDENTIFIER
			char *TEMP = node->token2.instance;
			//CALL <T>
			generator(node->child1, outFilePtr);
			int check_operator_t = operator_t;
			//CALL <W>
			generator(node->child2, outFilePtr);

			fprintf(outFilePtr, "STORE TEMP%d\n", count);
			fprintf(outFilePtr, "LOAD %s\n", TEMP);
			if(check_operator_t == 1){
				fprintf(outFilePtr, "SUB TEMP%d\n", count);
				fprintf(outFilePtr, "BRNEG DONE%d\n", count);
			}
			else if(check_operator_t == 2){
				fprintf(outFilePtr, "SUB TEMP%d\n", count);
				fprintf(outFilePtr, "BRZPOS DONE%d\n",count );
			}
			//CALL <D> || DO <D> IF <T><W> TRUE
			generator(node->child3, outFilePtr);
			fprintf(outFilePtr, "DONE%d: NOOP\n", count);
			count++;
			
		}
		//<F> -> DO AGAIN <D><T><W>
		else if(node->token1.type == BRACKET_RIGHT_TK){
			fprintf(outFilePtr, "REPEAT%d: NOOP\n", count);
		
			generator(node->child1, outFilePtr);
			generator(node->child2, outFilePtr);
			int check_operator_t = operator_t;
			generator(node->child3, outFilePtr);
			
			if(check_operator_t == 1){
				fprintf(outFilePtr, "BRNEG REPEAT%d\n", count);
			}
			else if(check_operator_t == 2){
				fprintf(outFilePtr, "BRZPOS REPEAT%d\n",count);
			}
	
			count++;	
		}
	}
	//<L>   ->    Flip Identifier
	else if(strcmp(node->name, "<L>") == 0){  
		//‘Flip identifier’ means to multiply the value for the given identifier by -1 and store the value  
		if(node->token1.type == ID_TK){
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
			fprintf(outFilePtr, "MULT %d\n", -1);
			fprintf(outFilePtr, "STORE %s\n", node->token1.instance);
			digit_assign = node->token1.instance;
		}
	}
	//<G>    ->     Here Number There  
	else if(strcmp(node->name, "<G>") == 0){
		//‘Here number There’ means print the given number to the screen the given number of times
		if(node->token1.type == DIGIT_TK || node->token2.type == DIGIT_TK){
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
			fprintf(outFilePtr, "STORE TEMP%d\n", count);
			fprintf(outFilePtr, "BRZNEG Done%d\n", count);
			fprintf(outFilePtr, "Repeat%d: WRITE TEMP%d\n", count, count);
			fprintf(outFilePtr, "SUB %d\n", 1);
			fprintf(outFilePtr, "BRPOS Repeat%d\n", count);
			fprintf(outFilePtr, "Done%d: NOOP\n", count);
			count++;
		}
	}
	//<K>   ->    Spot Number Show Number |  Move Identifer Show Identifier 
	else if(strcmp(node->name , "<K>") == 0){
		//‘Spot number’ means to load the immediate number into the ACCumulator 
		if(node->token1.type == DIGIT_TK){
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
		}
		//‘Show number’ means to write out the number to the monitor 
		else if(node->token2.type == DIGIT_TK){
			fprintf(outFilePtr, "WRITE %s\n", node->token2.instance);	
		}
		//‘Move identifier’ means to load the given identifier’s value into the ACCumulator 
		else if(node->token1.type == ID_TK){
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
		}
		//‘Show identifier’ means to write out the value of the given identifier to the monitor 
		else if(node->token2.type == ID_TK){
			fprintf(outFilePtr, "WRITE %s\n", node->token2.instance);
		}	
	}
	//<Z>   ->     Identifier  |  Number   
	else if(strcmp(node->name, "<Z>") == 0){
		if(node->token1.type == DIGIT_TK){
			count++;
			digit_assign = node->token1.instance;
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
			fprintf(outFilePtr, "SUB %d\n", 1);
			fprintf(outFilePtr, "STORE TEMP%d\n", count);
			//count++;
		}
		else if(node->token1.type == ID_TK){
			fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
			fprintf(outFilePtr, "SUB %d\n", 1);
			fprintf(outFilePtr, "STORE %s\n", node->token1.instance);
			
		}		
	}
	//<W>   ->   Number  <V>  Number  |   Number . 
	else if(strcmp(node->name, "<W>") == 0){
		fprintf(outFilePtr, "LOAD %s\n", node->token1.instance);
		if(operator_v == 1){
			fprintf(outFilePtr, "ADD %s\n", node->token3.instance);
		}
		else if(operator_v == 2){
			fprintf(outFilePtr, "DIV %s\n", node->token3.instance);
		}
		else if(operator_v == 3){
			fprintf(outFilePtr, "MULT %s\n", node->token3.instance);
		}
		else{	
			return;
		}
	}
	else{
		if(node->child1 != NULL){
			generator(node->child1, outFilePtr);}
		if(node->child2 != NULL){
			generator(node->child2, outFilePtr);}
		if(node->child3 != NULL){
			generator(node->child3, outFilePtr);}
		if(node->child4 != NULL){
			generator(node->child4, outFilePtr);}
	}
}	

void temp_stop(FILE *outFilePtr){
	int i;
	for(i = 1; i < count ; i++){
		fprintf(outFilePtr, "TEMP%d 0\n", i);	
	}
}
	

