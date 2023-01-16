#include "JackCompiler.h"


void vmWriteOp(FILE *outputFile, char op, uint8_t unary){
	char operation[20] = { 0 };
	switch(op){
		case '+':	strcpy(operation, "add");	break;
		case '-': 	
			if( unary) strcpy(operation, "neg");
			else strcpy(operation, "sub");
			break;
		case '~': 	strcpy(operation, "not");	break;
		case '&': 	strcpy(operation, "and");	break;
		case '|': 	strcpy(operation, "or");	break;
		case '>': 	strcpy(operation, "gt");	break;
		case '<': 	strcpy(operation, "lt");	break;
		case '=': 	strcpy(operation, "eq");	break;
		case '*':	fprintf(outputFile, "call Math.multiply 2" ); break;
		case '/':	fprintf(outputFile, "call Math.divide 2" ); break;

	}

	fprintf(outputFile, "%s\n", operation);
}

void vmPushToken(FILE *outputFile, char *token){
	fprintf(outputFile, "push %s\n", token);
}

void vmPopToken(FILE *outputFile, char *token){
	fprintf(outputFile, "pop %s\n", token);
}
