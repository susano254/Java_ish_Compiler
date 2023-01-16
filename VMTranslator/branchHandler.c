#include "VMTranslator.h"

void branchHandler(FILE* assemblyFile, char* instruction, char* label){
	char branchLabel[30];
	strcpy(branchLabel, returnLabel);
	strcat(branchLabel, "$");
	strcat(branchLabel, label);
	printf(" instruction is %s\t label is %s\n", instruction, label);
	printf(" the written Label is %s\n", branchLabel);
	if ( compare(instruction, "label") ) { 
		fprintf(assemblyFile,"(%s)\n", branchLabel);
	
	}
	else if ( compare(instruction, "goto") ) { 
		fprintf(assemblyFile, "@%s\n0;JMP\n", branchLabel);
	
	}
	else if ( compare(instruction, "if-goto") ) { 
		fprintf(assemblyFile, "@SP\nM=M-1\nA=M\nD=M\n@%s\nD;JNE\n", branchLabel);
	
	}
}
