#include "VMTranslator.h"



void callHandler(FILE *assemblyFile, char* funcLabel, char* nArgs){
	static int i = 0;
	char returnLabelString[30];
	strcpy(returnLabelString, funcLabel);
	strcat(returnLabelString, "$ret");
	printf("funcLabel is %s\n", funcLabel);
	fprintf(assemblyFile, "\n\t\t\t\t\t // performing a call instruction\n");
	
	fprintf(assemblyFile, "\n // pushing the return address to the stack\n");
	fprintf(assemblyFile, "@%s.%d\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", returnLabelString, i);

	fprintf(assemblyFile, "\n // pushing the LCL pointer value to the stack\n");
	fprintf(assemblyFile, "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");

	fprintf(assemblyFile, "\n // pushing the ARG pointer value to the stack\n");
	fprintf(assemblyFile, "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");

	fprintf(assemblyFile, "\n // pushing the THIS pointer value to the stack\n");
	fprintf(assemblyFile, "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");

	fprintf(assemblyFile, "\n // pushing the THAT pointer value to the stack\n");
	fprintf(assemblyFile, "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");

	fprintf(assemblyFile, "\n // setting the argument pointer of the function to be called\n // ARG = SP - 5 - nArgs\n");
	fprintf(assemblyFile, "@SP\nD=M\n@5\nD=D-A\n@%s\nD=D-A\n@ARG\nM=D\n", nArgs);


	fprintf(assemblyFile, "\n // setting the local pointer of the function to be called\n // LCL = SP\n");
	fprintf(assemblyFile, "@SP\nD=M\n@LCL\nM=D\n");

	fprintf(assemblyFile, "\n // (calling) jumping to %s\n", funcLabel);
	fprintf(assemblyFile, "@%s\n0;JMP\n", funcLabel);		// goto function

	fprintf(assemblyFile, "\n // putting the return address\n");
	fprintf(assemblyFile, "(%s.%d)\n", returnLabelString, i);			// (returnLabel)

	fprintf(assemblyFile, "\n\t\t\t\t\t // End of  call instruction\n");
	i++;

}

	
	
