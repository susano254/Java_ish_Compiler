#include "VMTranslator.h"

void returnHandler(FILE* assemblyFile) {
	printf("\n\t\t\t\t\t found a return instruction\n");
	//if ( counter == returnNum ) strcpy(returnLabel, previousReturnLabel);
	//if ( counter > returnNum) { 
	//	printf("address %p is %d and is greater than returnNum which is %d\n", &counter, counter, returnNum); 
	//}
	//counter++;
	//printf("address %p is being incremented and it's value now is %d\n", &counter, counter);
	fprintf(assemblyFile, "\n\t\t\t\t// performing a return instruction\n");

	fprintf(assemblyFile, "\n// storing LCL in a temp register\n");
	fprintf(assemblyFile, "@LCL\nD=M\n@R13\nM=D\n");

	fprintf(assemblyFile, "\n// storing the return address in a temp register\n");
	fprintf(assemblyFile, "@R13\nD=M\n@5\nD=D-A\nA=D\nD=M\n@R14\nM=D\n");

	fprintf(assemblyFile, "\n// poping the return Value to argument 0\n");
	fprintf(assemblyFile, "@SP\nAM=M-1\nD=M\n@ARG\nA=M\nM=D\n");

	fprintf(assemblyFile, "\n// SP = ARG+1\n");
	fprintf(assemblyFile, "@ARG\nD=M\n@1\nD=D+A\n@SP\nM=D\n");

	fprintf(assemblyFile, "\n// THAT = *(LCL - 1)\n");
	fprintf(assemblyFile, "@R13\nD=M\n@1\nD=D-A\nA=D\nD=M\n@THAT\nM=D\n");
	
	fprintf(assemblyFile, "\n// THIS = *(LCL - 2)\n");
	fprintf(assemblyFile, "@R13\nD=M\n@2\nD=D-A\nA=D\nD=M\n@THIS\nM=D\n");
	
	fprintf(assemblyFile, "\n// ARG = *(LCL - 3)\n");
	fprintf(assemblyFile, "@R13\nD=M\n@3\nD=D-A\nA=D\nD=M\n@ARG\nM=D\n");
	
	fprintf(assemblyFile, "\n// LCL = *(LCL - 4)\n");
	fprintf(assemblyFile, "@R13\nD=M\n@4\nD=D-A\nA=D\nD=M\n@LCL\nM=D\n");

	fprintf(assemblyFile, "\n// Jumping to the return address that is in R14\n");
	fprintf(assemblyFile, "@R14\nA=M\n0;JMP\n");

	fprintf(assemblyFile, "\n\t\t\t\t// End of  return instruction\n");


}
